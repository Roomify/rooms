<?php

/**
 * @file
 * A pricing event represent a price over a set of continious dates. As soon as the
 * price changes that is a different pricing event
 */

class PricingEvent {

  // The room the event is relevant to
  public $unit_id;

  // The start date for the event
  public $start_date;

  // The end date for the event
  public $end_date;

  // The amount for this period
  public $amount;

  // The operation to perform
  public $operation;

  public function __construct($unit_id, $amount, $start_date, $end_date, $operation = '') {
    $this->unit_id = $unit_id;
    $this->amount = $amount;
    $this->start_date = $start_date;
    $this->end_date = $end_date;
    $this->operation = $operation;
  }

  public function startDay($format = 'j') {
    return $this->start_date->format($format);
  }

  public function startMonth($format = 'n') {
    return $this->start_date->format($format);
  }

  public function startYear($format = 'Y') {
    return $this->start_date->format($format);
  }

  public function endDay($format = 'j') {
    return $this->end_date->format($format);
  }

  public function endMonth($format = 'n') {
    return $this->end_date->format($format);
  }

  public function endYear($format = 'Y') {
    return $this->end_date->format($format);
  }

  /**
   * Returns the months involved in the events
   */
  public function months() {
  }

  public function diff() {
    $interval = $this->start_date->diff($this->end_date);
    return $interval;
  }

  public function sameMonth() {
    if (($this->startMonth() == $this->endMonth()) && ($this->startYear() == $this->endYear())) {
      return TRUE;
    }
    return FALSE;
  }

  public function sameYear() {
    if ($this->startYear() == $this->endYear()) {
      return TRUE;
    }
    return FALSE;
  }


  public function applyOperation($amount, $operation) {
    switch ($operation) {
      case ROOMS_REPLACE:
        $this->amount = $amount;
        break;
      case ROOMS_ADD:
        $this->amount = $this->amount + $amount;
        break;
      case ROOMS_SUB:
        $this->amount = $this->amount - $amount;
        break;
      case ROOMS_INCREASE:
        $this->amount = $this->amount + (($this->amount) * ($amount/100));
        break;
      case ROOMS_DECREASE:
        $this->amount = $this->amount - (($this->amount) * ($amount/100));
        break;
      default:
        break;
    }

  }

  /**
   * Takes a single event that spans several years and transforms it to
   * yearly events
   */
  public function transformToYearlyEvents() {
    // If same year return the event
    if ($this->sameYear()) {
      $sd = new DateTime();
      $sd->setDate($this->startYear(), $this->startMonth(), $this->startDay());
      $ed = new DateTime();
      $ed->setDate($this->endYear(), $this->endMonth(), $this->endDay());
      $be = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      return array($be);
    }

    // Else split into years
    $events = array();
    for ($i = $this->startYear(); $i <= $this->endYear(); $i++) {
      $sd = new DateTime();
      $ed = new DateTime();
      if ($i == $this->startYear()) {
        $sd->setDate($i, $this->startMonth(), $this->startDay());
        $ed->setDate($i, 12, 31);
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
      elseif ($i == $this->endYear()) {
        $sd->setDate($i, 1, 1);
        $ed->setDate($i, $this->endMonth(), $this->endDay());
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
      else {
        $sd->setDate($i, 1, 1);
        $ed->setDate($i, 12, 31);
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
    }

    return $events;
  }

  /**
   * Takes a single event that spans several months and transforms it to
   * monthly events - this assumes that the event is contained within a year
   */
  public function transformToMonthlyEvents() {
    $events = array();
    //First we need to split into events in separate years
    if (!$this->sameYear()) {
      return FALSE;
    }
    if ($this->sameMonth()) {
      $sd = new DateTime();
      $sd->setDate($this->startYear(), $this->startMonth(), $this->startDay());
      $ed = new DateTime();
      $ed->setDate($this->endYear(), $this->endMonth(), $this->endDay());
      $be = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      return array($be);
    }
    $months = rooms_end_of_month_dates($this->startYear());

    for ($i = $this->startMonth(); $i <= $this->endMonth(); $i++) {
      $sd = new DateTime();
      $ed = new DateTime();
      if ($i == $this->startMonth()) {
        $sd->setDate($this->startYear() , $i, $this->startDay());
        $ed->setDate($this->startYear(), $i, $months[$i]);
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
      elseif ($i == $this->endMonth()) {
        $sd->setDate($this->startYear(), $i, 1);
        $ed->setDate($this->startYear(), $i, $this->endDay());
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
      else{
        $sd->setDate($this->startYear(), $i, 1);
        $ed->setDate($this->startYear(), $i, $months[$i]);
        $events[$i] = new PricingEvent($this->unit_id, $this->amount, $sd, $ed, $this->operation);
      }
    }
    return $events;
  }


  /**
   * Return event in a format amenable to FullCalendar display or generally
   * sensible json
   */
  public function formatJson() {
    //print $this->amount;
    $amount = $this->amount;

    $event = array(
      "id" => $amount,
      //'title' => 'Event ID: ' . $this->amount . ' - Unit ID: ' . $this->unit_id,
      "start" => $this->startYear() . '-' . $this->startMonth('m') . '-' . $this->startDay('d'),
      "end" => $this->endYear() . '-' . $this->endMonth('m') . '-' . $this->endDay('d'),
    );

    //set the color
    if ($this->amount < 100) {
      $event['color']  = "orange";
      $event['title'] = "$this->amount";
    }
    elseif ($this->amount >= 100) {
      $event['color'] = "green";
      $event['title'] = "$amount";
    }

    return $event;
  }

}

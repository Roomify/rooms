<?php

class BookingEvent {
  
  // The room the event is relevant to
  public $unit_id;
  
  // The start date for the event
  public $start_date;
  
  // The end date for the event
  public $end_date;
  
  // The type of event
  public $id;
  
  
  public function __construct($unit_id, $event_id, $start_date, $end_date) {
    $this->unit_id = $unit_id;
    $this->id = $event_id;
    $this->start_date = $start_date;
    $this->end_date = $end_date;
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
  
  /**
   * Lock event - updates the rooms_booking_locks table to indicate that this event
   * is locked
   */
  public function lock() {   
    $fields = array(
      'unit_id' => $this->unit_id,
      'state' => $this->id,
      'locked' => 1,
    );
    $lock = db_insert('rooms_booking_locks')->fields($fields);
    $lock->execute();
  }
  
  
  /**
   * Takes an event that spans several years and transforms it to
   * yearly events
   */
  public function transformToYearlyEvents() {
    // If same year return the event
    if ($this->sameYear()) {
      $sd = new DateTime();
      $sd->setDate($this->startYear(), $this->startMonth(), $this->startDay());
      $ed = new DateTime();
      $ed->setDate($this->endYear(), $this->endMonth(), $this->endDay());
      $be = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
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
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      }
      elseif ($i == $this->endYear()) {
        $sd->setDate($i, 1, 1);
        $ed->setDate($i, $this->endMonth(), $this->endDay());
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      }
      else {
        $sd->setDate($i, 1, 1);
        $ed->setDate($i, 12, 31);
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
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
      $be = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      return array($be);      
    }
    $months = rooms_end_of_month_dates($this->startYear());

    for ($i = $this->startMonth(); $i <= $this->endMonth(); $i++) {
      $sd = new DateTime();
      $ed = new DateTime();
      if ($i == $this->startMonth()) {
        $sd->setDate($this->startYear() , $i, $this->startDay());
        $ed->setDate($this->startYear(), $i, $months[$i]);
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      }
      elseif ($i == $this->endMonth()) {
        $sd->setDate($this->startYear(), $i, 1);
        $ed->setDate($this->startYear(), $i, $this->endDay());
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      }
      else{
        $sd->setDate($this->startYear(), $i, 1);
        $ed->setDate($this->startYear(), $i, $months[$i]);
        $events[$i] = new BookingEvent($this->unit_id, $this->id, $sd, $ed);
      }
    }
    return $events;
  }
  
    
  /**
   * Return event in a format amenable to FullCalendar display or generally
   * sensible json
   */
  public function formatJson() {
    $event = array(
      'id' => $this->id,
      'title' => $this->id,
      'start' => $this->startYear() . '-' . $this->startMonth('m') . '-' . $this->startDay('d'),
      'end' => $this->endYear() . '-' . $this->endMonth('m') . '-' . $this->endDay('d'),
    );
    
    //set the color
    switch ($this->id) {
      case ROOMS_NOT_AVAILABLE:
        $event['color']  = 'red';
        $event['title'] = 'N/A';
        break;
      case ROOMS_AVAILABLE:
        $event['color'] = '#8CBF62';
        $event['borderColor'] = '#04711B';
        //$event['backgroundColor'] = 'green';
        $event['title'] = 'AV';
        break;
      case ROOMS_ON_REQUEST:
        $event['color'] = '#C5C5C5';
        $event['title'] = 'ON-REQ';
        break;        
      case ROOMS_ANON_BOOKED:
        $event['color'] = '#481600';
        $event['title'] = 'A-B';
        break;        
      default:
        $event['color'] = 'blue';
        break;
    }
    
    return $event;
  }
  
}
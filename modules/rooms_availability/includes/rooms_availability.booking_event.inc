<?php

class BookingEvent {

  // The booking unit the event is relevant to
  public $unit_id;

  // The start date for the event
  public $start_date;

  // The end date for the event
  public $end_date;

  // The type of event
  public $id;


  /**
   * Init a BookingEvent instance.
   *
   * @param int $unit_id
   *   The bookable unit.
   * @param int $event_id
   *   The event ID.
   * @param DateTime $start_date
   *   The start date of the event.
   * @param DateTime $end_date
   *   The end date of the event.
   */
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
    // Check that the event is not already locked
    $query = db_select('rooms_booking_locks', 'l');
    $query->addField('l', 'unit_id');
    $query->addField('l', 'state');
    $query->addField('l', 'locked');
    $query->condition('l.unit_id', $this->unit_id);
    $query->condition('l.state', $this->id);
    $query->condition('l.locked', 1);
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (count($result) == 1) {
      return FALSE;
    }
    else {
      $fields = array(
        'unit_id' => $this->unit_id,
        'state' => $this->id,
        'locked' => 1,
      );
      $lock = db_insert('rooms_booking_locks')->fields($fields);
      $lock->execute();
    }
    return TRUE;
  }

  public function unlock() {
    db_delete('rooms_booking_locks')
    ->condition(db_or()->condition('state', $this->id)->condition('state', -($this->id)))
    ->execute();
  }


  /**
   * Takes an event that spans several years and transforms it to yearly events
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
  public function formatJson($style = ROOMS_AVAILABILITY_ADMIN_STYLE, $unit_name = '') {
    $event = array(
      'id' => $this->id,
      'start' => $this->startYear() . '-' . $this->startMonth('m') . '-' . $this->startDay('d') . 'T13:00:00Z',
      'end' => $this->endYear() . '-' . $this->endMonth('m') . '-' . $this->endDay('d') . 'T13:00:00Z',
      'title' => $this->id,
    );

    // Check if we are dealing with a booking
    if ($this->id > 10 || $this->id < -10) {
      // Get the actual booking id
      $booking_id = rooms_availability_return_id($this->id);
      $booking = rooms_booking_load($booking_id);
      if ($style == ROOMS_AVAILABILITY_ADMIN_STYLE) {
        $name = $booking->name;
        $interval = $this->diff();
        if ((strlen($name) > 7) && ($interval->d < 1)) {
          $event['title'] = substr($booking->name, 0 , 6) . '...';
        }
        else {
          $event['title'] = $booking->name;
        }
      }
      elseif ($style == ROOMS_AVAILABILITY_GENERIC_STYLE) {
        $this->id = ROOMS_NOT_AVAILABLE;
      }
    }

    $view_unit_name = array_filter(variable_get('rooms_view_unit_name', array('')));

    //set the color
    switch ($this->id) {
      case ROOMS_NOT_AVAILABLE:
        $event['color']  = variable_get('rooms_not_available_color', 'red');
        $event['title'] = variable_get('rooms_not_available_text', 'N/A');
        break;
      case ROOMS_AVAILABLE:
        $event['color'] = variable_get('rooms_available_color', '#8CBF62');
        $event['borderColor'] = '#04711B';
        $event['title'] = variable_get('rooms_available_text', 'AV');
        break;
      case ROOMS_ON_REQUEST:
        $event['color'] = variable_get('rooms_on_request_color', '#C5C5C5');
        $event['title'] = variable_get('rooms_on_request_text', 'ON-REQ');
        break;
      case ($this->id < 0):
        $event['color'] = variable_get('rooms_unconfirmed_booking_color', '#8CBF62');
        break;
      case ROOMS_ANON_BOOKED:
        if ($style == 'admin') {
          $event['color'] = variable_get('rooms_anon_booked_color', '#481600');
          $event['title'] = variable_get('rooms_anon_booking_text', 'A-B');
        }
        elseif ($style == ROOMS_AVAILABILITY_GENERIC_STYLE) {
          $event['color']  = variable_get('rooms_not_available_color', 'red');
          $event['title'] = variable_get('rooms_not_available_text', 'N/A');
        }
        break;
      default:
        $event['color'] = 'blue';
        break;
    }

    if (!empty($view_unit_name) && $style == ROOMS_AVAILABILITY_GENERIC_STYLE) {
      $event['title'] = $unit_name;
    }

    return $event;
  }

}

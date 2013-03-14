<?php

/**
 * Handles quering and updating the availability information
 * relative to a single bookable unit.
 */
class UnitCalendar {

  // The bookable unit the Calendar is relevant to
  protected $unit_id;

  // The default state for the room if it has no specific booking
  protected $default_state;


  public function __construct($unit_id) {
    $this->unit_id = $unit_id;
    // Load the booking unit
    $unit = rooms_unit_load($unit_id);
    $this->default_state = $unit->default_state;
  }


  /**
   * Given an array of events removes events from the calendar setting the value to the default
   *
   * @param $events
   *    The events to remove from the database - an array of Booking Events
   *
   * @return
   *   The array of ids of events that were found and removed
   */
  public function removeEvents($events) {

    $events_to_delete = array();
    foreach ($events as $event) {
      // Break any locks
      $event->unlock();
      // Set the events to the default state
      $event->id = $this->default_state;

      $events_to_delete[] = $event;
    }
    $this->updateCalendar($events_to_delete);
  }


  /**
   * Given a set of states (e.g. the desired states to accept a booking) we compare against
   * the states the unit is actually in.
   *
   * If the unit is in any state that is not in the list of desired states it means there is a
   * mismatch - hence no availability.
   *
   * @param $start_date
   * The starting date for the search
   *
   * @param $end_date
   * The end date for the search
   *
   * @param $states
   * The states we are interested in
   *
   * @return
   * Returns true if the date range provided does not have states other than the ones we are
   * interested in
   */
  public function stateAvailability(DateTime $start_date, DateTime $end_date, $states = array()) {

    $valid = TRUE;

    // Get all states in the date range
    $existing_states = $this->getStates($start_date, $end_date);
    // Look at the difference between existing states and states to check against -
    $diff = array_diff($existing_states, $states);
    $valid = (count($diff) > 0) ? FALSE : TRUE;
    return $valid;
  }


  /**
   * Given a date range returns an array of BookingEvents. The heavy lifting really takes place in
   * the getRawDayData function - here we are simply acting as a factory for event objects
   *
   * @param $start_date
   * The starting date
   *
   * @param $end_date
   * The end date of our range
   *
   * @return
   * An array of BookingEvent objects
   */
  public function getEvents(DateTime $start_date, DateTime $end_date) {
    // Format dates for use later on
    $start_day = $start_date->format('j');
    $end_day = $end_date->format('j');

    // Get the raw day results
    $results = $this->getRawDayData($start_date, $end_date);
    $events = array();
    foreach ($results[$this->unit_id] as $year => $months) {
      $eod = rooms_end_of_month_dates($year);
      foreach ($months as $mid => $month) {
        // The event array gives us the start days for each event within a month
        $start_days = array_keys($month['states']);
        foreach ($month['states'] as $state) {
          // Create a booking event
          $start = $state['start_day'];
          $end= $state['end_day'];
          $sd = new DateTime("$year-$mid-$start");
          $ed = new DateTime("$year-$mid-$end");

          $event = new BookingEvent($this->unit_id,
                                    $state['state'],
                                    $sd,
                                    $ed);
          $events[] = $event;
        }
      }
    }
    return $events;
  }


  /**
   * Given a date range returns all states in that range - useful when we are not interested
   * in starting and ending dates but simply in states
   *
   * @param $start_date
   * The start day of the range
   *
   * @param $end_date
   * The end date of our range
   *
   * @return
   * An array of states within that range
   */
  public function getStates(DateTime $start_date, DateTime $end_date, $confirmed = FALSE) {

    $states = array();
    // Get the raw day results
    $results = $this->getRawDayData($start_date, $end_date);
    foreach ($results[$this->unit_id] as $year => $months) {
      foreach ($months as $mid => $month) {
        foreach ($month['states'] as $state) {
          if ($state['state'] < 0 && !$confirmed) {
            $states[] = -1;
          }
          else {
            $states[] = $state['state'];
          }
        }
      }
    }
    $states = array_unique($states);
    return $states;
  }


  /**
   * Given a date range it returns all data within that range including the start and
   * end dates of states. The MySQL queries are kept simple and then the data is cleared up
   *
   * @param $start_date
   * The starting date
   *
   * @param $end_date
   * The end date of our range
   *
   * @return
   * An array of the structure data[unitid][year][month][days][d1]..[d31]
   * as wekk as data[unitid][year][month][unique_states]
   */
  public function getRawDayData(DateTime $start_date, DateTime $end_date) {

    // Create a dummy BookingEvent to represent the range we are searching over
    // This gives us access to handy functions that BookingEvents have
    $s = new BookingEvent($this->unit_id, 0, $start_date, $end_date);

    $results = array();

    // Start by doing a query to the db to get any info stored there

    // If search across the same year do a single query
    if ($s->sameYear()) {
      $query = db_select('rooms_availability', 'a');
      $query->fields('a');
      $query->condition('a.unit_id', $this->unit_id);
      $query->condition('a.year', $s->startYear());
      $query->condition('a.month', $s->startMonth(), '>=');
      $query->condition('a.month', $s->endMonth(), '<=');
      $months = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
      if (count($months) > 0) {
        foreach ($months as $month) {
          $m = $month['month'];
          $y = $month['year'];
          $id = $month['unit_id'];
          // Remove the three first rows and just keep the days
          unset($month['month']);
          unset($month['year']);
          unset($month['unit_id']);
          $results[$id][$y][$m]['days'] = $month;
        }
      }
    }
    // For multiple years do a query for each year
    else {
      for ($j = $s->startYear(); $j <= $s->endYear(); $j++) {
        $query = db_select('rooms_availability', 'a');
        $query->fields('a');
        $query->condition('a.unit_id', $this->unit_id);
        $query->condition('a.year', $j);
        if ($j == $s->startYear()) {
          $query->condition('a.month', $s->startMonth(), '>=');
        }
        elseif ($j == $s->endYear()) {
          $query->condition('a.month', $s->endMonth(), '<=');
        }
        $months = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        if (count($months) > 0) {
          foreach ($months as $month) {
            $m = $month['month'];
            $y = $month['year'];
            $id = $month['unit_id'];
            unset($month['month']);
            unset($month['year']);
            unset($month['unit_id']);
            $results[$id][$y][$m]['days'] = $month;
          }
        }
      }
    }


    // With the results from the db in place fill in any missing months
    // with the default state for the unit
    for ($j = $s->startYear(); $j <= $s->endYear(); $j++) {
      $eod = rooms_end_of_month_dates($j);

      // We start by setting the expected start and end months for each year
      if ($s->sameYear()) {
        $expected_months = $s->endMonth() - $s->startMonth() +1;
        $sm = $s->startMonth();
        $em = $s->endMonth();
      }
      elseif ($j == $s->endYear()) {
        $expected_months = $s->endMonth();
        $sm = 1;
        $em = $s->endMonth();
      }
      elseif ($j == $s->startYear()) {
        $expected_months = 12 - $s->startMonth() +1;
        $em = 12;
        $sm = $s->startMonth();
      }
      else {
        $expected_months = 12;
        $sm = 1;
        $em = 12;
      }

      // We then check to see if the months we have already fit our expectations
      $actual_months =  isset($result[$this->unit_id][$j]) ? count($results[$id][$j]) : 0;
      if ($expected_months>$actual_months) {
        // We have missing months so lets go fill them
        for ($i = $sm; $i <= $em; $i++) {
          if (!isset($results[$this->unit_id][$j][$i])) {
            $last_day = $eod[$i];
            $month = $this->prepareFullMonthArray(new BookingEvent($this->unit_id,
                                                                   $this->default_state,
                                                                   new DateTime("$j-$i-1"),
                                                                   new DateTime("$j-$i-$last_day")));
            // Add the month in its rightful position
            $results[$this->unit_id][$j][$i]['days'] = $month;
            // And sort months
            ksort($results[$this->unit_id][$j]);
          }
        }
      }
    }

    // With all the months in place we now need to clean results to set the right start and end date
    // for each month - this will save code downstream from having to worry about it
    foreach ($results[$this->unit_id] as $year => $months) {
      foreach ($months as $mid => $days) {
        // Get the end of month values again to make sure we have the right year because it might change
        // for queries spanning years
        $eod = rooms_end_of_month_dates($year);
        // There is undoubtetly a smarter way to do the clean up below - but will live with this for now
        if (count($days['days']) != $eod[$mid]) {
          switch ($eod[$mid]) {
            case 30:
              unset($results[$this->unit_id][$year][$mid]['days']['d31']);
              break;
            case 29:
              unset($results[$this->unit_id][$year][$mid]['days']['d31']);
              unset($results[$this->unit_id][$year][$mid]['days']['d30']);
              break;
            case 28:
              unset($results[$this->unit_id][$year][$mid]['days']['d31']);
              unset($results[$this->unit_id][$year][$mid]['days']['d30']);
              unset($results[$this->unit_id][$year][$mid]['days']['d29']);
              break;
          }
        }
        if (($year == $s->startYear()) && ($mid == $s->startMonth())) {
          // We know we have the entire months over the range so we just unset all the dates
          // from the start of the month to the actual start day
          for ($i = 1; $i < $s->startDay(); $i++) {
            unset($results[$this->unit_id][$year][$mid]['days']['d' . $i]);
          }
        }
        if (($year == $s->endYear()) && ($mid == $s->endMonth())) {
          // and from the end of the month back to the actual end day
          for ($i = $s->endDay()+1; $i <= $eod[$mid]; $i++) {
            unset($results[$this->unit_id][$year][$mid]['days']['d' . $i]);
          }
        }

      }
    }

    // With the results in place we do a states array with the start and
    // end dates of each event
    foreach ($results[$this->unit_id] as $year => $months) {
      foreach ($months as $mid => $days) {
        // The number of days in the month we are interested in eventing
        $j = count($days);
        // The start date (in case we are not starting from the first day of the month)
        $i = substr(key($days['days']), 1);
        $start_day = $i;
        $end_day = NULL;
        $unique_states = array();
        $old_state = $days['days']['d' . $i];
        $state = $days['days']['d' . $i];
        while ($j <= count($days['days'])) {
          $state = $days['days']['d' . $i];
          if ($state != $old_state) {
            $unique_states[] = array('state' => $old_state,
                                               'start_day' => $start_day,
                                               'end_day' => $i-1);
            $end_day = $i - 1;
            $start_day = $i;
            $old_state = $state;
          }
          $i++;
          $j++;
        }
        // Get the last event in
        $unique_states[] = array('state' => $state,
                                   'start_day' => isset($end_day) ? $end_day+1 : $start_day,
                                   'end_day' => $i-1);
        $results[$this->unit_id][$year][$mid]['states'] = $unique_states;
      }
    }

    return $results;
  }



  /**
   * Checks if an event is blocked, i.e. cannot be updated. This happens
   * when the event id is in the rooms_booking_locks table and the new event_id
   * is not the same as the one that is locked.
   *
   * @param int $existing_state - The state to check if locked
   *
   * @param int $new_state - The state we want to move to
   *
   * @return boolean TRUE if not blocked
   */
  public function eventBlocked($existing_state, $new_state) {

    $blocked = FALSE;

    // Query the locks table to see if event is blocked
    $query = db_select('rooms_booking_locks', 'l');
    $query->addField('l', 'unit_id');
    $query->addField('l', 'state');
    $query->addField('l', 'locked');
    $query->condition('l.unit_id', $this->unit_id);
    $query->condition('l.state', $existing_state);
    $query->condition('l.locked', 1);
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    // Only block if we are trying to update an event that is not locked
    if ((count($result) > 0) && $result[0]['state'] != $new_state) {
      $blocked = TRUE;
    }
    return $blocked;
  }


  /**
   * Given an array of BookingEvents the calendar is updated with regards to the
   * events that are relevant to the Unit this calendar refers to
   *
   * @param $events
   * An array of events to update the calendar with
   *
   * @return array - An array of response on whether event updates were succesful or not
   */
  public function updateCalendar(array $events) {

    $responses = array();
    // First check that none of the events supplied are blocked by an existing event with a locked status
    $monthly_events = array();

    foreach ($events as $event) {
      // Make sure event refers to the unit for this calendar
      if ($event->unit_id == $this->unit_id) {

        // Check if event is not blocked by a locked event
        $states = $this->getStates($event->start_date, $event->end_date);
        $blocked = FALSE;
        foreach ($states as $state) {
          if ($this->eventBlocked($state, $event->id)) {
            $blocked = TRUE;
          }
        }

        if (!$blocked) {
          // If the event is in the same month span just queue to be added
          if ($event->sameMonth()) {
            $monthly_events[] = $event;
          }
          else {
            // Check if multi-year - if not just create monthly events
            if ($event->sameYear()) {
              $monthly_events_tmp = array();
              $monthly_events_tmp = $event->transformToMonthlyEvents();
              $monthly_events =  array_merge($monthly_events, $monthly_events_tmp);
            }
            else {
              // else transform to single years and then to monthly
              $yearly_events = $event->transformToYearlyEvents();
              foreach ($yearly_events as $ye) {
                $monthly_events_tmp = array();
                $monthly_events_tmp = $ye->transformToMonthlyEvents();
                $monthly_events =  array_merge($monthly_events, $monthly_events_tmp);
              }
            }
          }
        }
        else {
          $response[$event->id] = ROOMS_BLOCKED;
        }
      }
      else {
        $response[$event->id] = ROOMS_WRONG_UNIT;
      }
    }

    foreach ($monthly_events as $event) {
      $this->addMonthEvent($event);
      $response[$event->id] = ROOMS_UPDATED;
    }

    return $response;
  }


  /**
   * Adds an event to the calendar
   *
   * @param $event
   *   An an event of type BookingEvent
   *
   * @return
   *   TRUE if events added, FALSE if some event failed
   */
  public function addMonthEvent($event) {
    // First check if the month exists and do an update if so
    if ($this->monthDefined($event->startMonth(), $event->startYear())) {
      $partial_month_row = $this->preparePartialMonthArray($event);
      $update = db_update('rooms_availability')
        ->condition('unit_id', $this->unit_id)
        ->condition('month', $event->startMonth())
        ->condition('year', $event->startYear())
        ->fields($partial_month_row)
        ->execute();
    }
    // Do an insert for a new month
    else {
      // Prepare the days array
      $days = $this->prepareFullMonthArray($event);
      $month_row = array(
        'unit_id' => $this->unit_id,
        'year' => $event->startYear(),
        'month' => $event->startMonth(),
      );
      $month_row = array_merge($month_row, $days);

      $insert = db_insert('rooms_availability')->fields($month_row);
      $insert->execute();
    }
  }


  /**
   * Given an event it prepares the entire month array for it
   * assuming no other events in the month and days where there
   * is no event get set to the default state;
   */
  protected function prepareFullMonthArray($event) {
    $days = array();
    $eod = rooms_end_of_month_dates($event->startYear());
    $last_day = $eod[$event->startMonth()];

    for ($i = 1; $i<=$last_day; $i++) {
      if (($i >= $event->startDay()) && ($i <= $event->endDay())) {
        $days['d' . $i] = $event->id;
      }
      else {
        $days['d' . $i] = $this->default_state; //replace with default state
      }
    }
    return $days;
  }


  /**
   * Given an event it prepares a partial array covering just the days
   * for which the event is involved
   */
  protected function preparePartialMonthArray($event) {
    $days = array();
    for ($i = $event->startDay(); $i<=$event->endDay(); $i++) {
        $days['d' . $i] = $event->id;
    }
    return $days;
  }

  /**
   * Returns the default state
   */
  public function getDefaultState() {
    return $this->default_state;
  }

  /**
   * Check if a month exists
   *
   * @return true - if the month is defined
   */
  public function monthDefined($month, $year) {
    $query = db_select('rooms_availability', 'a');
    $query->addField('a', 'unit_id');
    $query->addField('a', 'year');
    $query->addField('a', 'month');
    $query->condition('a.unit_id', $this->unit_id);
    $query->condition('a.year', $year);
    $query->condition('a.month', $month);
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    if (count($result) > 0) {
      return TRUE;
    }
    return FALSE;
  }


}

<?php


class UnitWeeklyPricingCalendar extends UnitPricingCalendar {

  /**
   * Constructs a UnitPricingCalendar instance.
   *
   * @param int $unit_id
   *   The unit ID.
   * @param array $price_modifiers
   *   The price modifiers to apply.
   */
  public function __construct($unit_id, $price_modifiers = array()) {
    $this->unit_id = $unit_id;
    // Load the booking unit.
    $this->unit = rooms_unit_load($unit_id);
    $this->default_state = $this->unit->default_state;

    $unit_type = rooms_unit_type_load($this->unit->type);
    if (isset($unit_type->data['pricing_weekly_field'])) {
      $field_price = $unit_type->data['pricing_weekly_field'];
      if (isset($this->unit->{$field_price}[LANGUAGE_NONE][0]['amount'])) {
        $this->default_price = $this->unit->{$field_price}[LANGUAGE_NONE][0]['amount'] / 100;
      }
    }

    $this->price_modifiers = $price_modifiers;

    $this->base_table = 'rooms_weekly_pricing';
  }

  public function calculatePrice(DateTime $start_date, DateTime $end_date, $persons = 0, $children = 0, $children_ages = array()) {
  }

  /**
   * {@inheritdoc}
   */
  public function getEvents(DateTime $start_date, DateTime $end_date) {
    // Get the raw day results.
    $results = $this->getRawDayData($start_date, $end_date);
    $events = array();
    foreach ($results[$this->unit_id] as $year => $months) {
      foreach ($months as $mid => $month) {
        // Event array gives us the start days for each event within a month.
        $start_days = array_keys($month['states']);
        foreach ($month['states'] as $state) {
          // Create a booking event.
          $start = $state['start_day'];
          $end = $state['end_day'];
          $sd = new DateTime("$year-$mid-$start");
          $ed = new DateTime("$year-$mid-$end");
          $amount = commerce_currency_amount_to_decimal($state['state'], commerce_default_currency());
          $event = new PricingEvent($this->unit_id, $amount, $sd, $ed);
          $events[] = $event;
        }
      }
    }
    return $events;
  }

  /**
   * {@inheritdoc}
   */
  public function getRawDayData(DateTime $start_date, DateTime $end_date) {
    // To handle single-day bookings (Tours) we pretend that they are overnight
    // bookings.
    if ($end_date < $start_date) {
      $end_date->add(new DateInterval('P1D'));
    }

    // Create a dummy PricingEvent to represent the range we are searching over.
    // This gives us access to handy functions that PricingEvents have.
    $s = new PricingEvent($this->unit_id, 0, $start_date, $end_date);

    $results = array();

    // If search across the same year do a single query.
    if ($s->sameYear()) {
      $query = db_select('rooms_weekly_pricing', 'a');
      $query->fields('a');
      $query->condition('a.unit_id', $this->unit_id);
      $query->condition('a.year', $s->startYear());
      $years = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
      if (count($years) > 0) {
        foreach ($years as $year) {
          $y = $year['year'];
          $id = $year['unit_id'];
          // Remove the three first rows and just keep the weeks.
          unset($year['year']);
          unset($year['unit_id']);
          $results[$id][$y]['weeks'] = $year;
        }
      }
    }
    // For multiple years do a query for each year.
    else {
      for ($j = $s->startYear(); $j <= $s->endYear(); $j++) {
        $query = db_select('rooms_weekly_pricing', 'a');
        $query->fields('a');
        $query->condition('a.unit_id', $this->unit_id);
        $query->condition('a.year', $j);
        $years = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
        if (count($years) > 0) {
          foreach ($years as $year) {
            $y = $year['year'];
            $id = $year['unit_id'];
            unset($year['year']);
            unset($year['unit_id']);
            $results[$id][$y]['weeks'] = $year;
          }
        }
      }
    }

    // With the results from the db in place fill in any missing months
    // with the default state for the unit.
    for ($j = $s->startYear(); $j <= $s->endYear(); $j++) {
      if (!isset($results[$this->unit_id][$j])) {
        $results[$this->unit_id][$j]['weeks'] = array();
        for ($w = 1; $w <= 53; $w++) {
          $results[$this->unit_id][$j]['weeks']['w' . $w] = '-1';
        }
      }
    }

    // With all the months in place we now need to clean results to set the
    // right start and end date for each month - this will save code downstream
    // from having to worry about it.
    foreach ($results[$this->unit_id] as $year => $wekks) {
      if ($year == $s->startYear()) {
        $mid = $s->startWeek();

        for ($i = 1; $i < $mid; $i++) {
          unset($results[$this->unit_id][$year]['weeks']['w' . $i]);
        }
      }
      if ($year == $s->endYear()) {
        $mid = $s->endWeek();

        for ($i = $mid + 1; $i <= 53; $i++) {
          unset($results[$this->unit_id][$year]['weeks']['w' . $i]);
        }
      }
    }

    // We store -1 instead of the default price in the DB so this is our chance to get the default price back
    // cycling through the data and replace -1 with the current default price of the unit.
    foreach ($results[$this->unit_id] as $year => $weeks) {
      foreach ($weeks['weeks'] as $week => $price) {
        if ($results[$this->unit_id][$year]['weeks'][$week] == '-1') {
          $results[$this->unit_id][$year]['weeks'][$week] = commerce_currency_decimal_to_amount($this->default_price, commerce_default_currency());
        }
      }
    }

    return $results;
  }

  /**
   * {@inheritdoc}
   */
  public function updateCalendar($events) {

    foreach ($events as $event) {
      // Make sure event refers to the unit for this calendar.
      if ($event->unit_id == $this->unit_id) {
        // Get all the pricing events that fit within this event.
        $affected_events = $this->getEvents($event->start_date, $event->end_date);
        $monthly_events = array();

        foreach ($affected_events as $a_event) {
          /** @var PricingEventInterface $a_event */
          // Apply the operation.
          $a_event->applyOperation($event->amount, $event->operation);
          // If the event is in the same month span just queue to be added.
          if ($a_event->sameMonth()) {
            $monthly_events[] = $a_event;
          }
          else {
            // Check if multi-year - if not just create monthly events.
            if ($a_event->sameYear()) {
              $monthly_events_tmp = $a_event->transformToMonthlyEvents();
              $monthly_events = array_merge($monthly_events, $monthly_events_tmp);
            }
            else {
              // Else transform to single years and then to monthly.
              $yearly_events = $a_event->transformToYearlyEvents();
              foreach ($yearly_events as $ye) {
                $monthly_events_tmp = $ye->transformToMonthlyEvents();
                $monthly_events = array_merge($monthly_events, $monthly_events_tmp);
              }
            }
          }
        }

        foreach ($monthly_events as $event) {
          $this->addWeeklyEvent($event);
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function preparePartialMonthArray(RoomsEventInterface $event) {
    $days = array();
    for ($i = $event->startWeek(); $i <= $event->endWeek(); $i++) {
      $days['w' . $i] = commerce_currency_decimal_to_amount($event->amount, commerce_default_currency());
    }
    return $days;
  }

  /**
   * {@inheritdoc}
   */
  public function YearDefined($year) {
    $query = db_select($this->base_table, 'a');
    $query->addField('a', 'unit_id');
    $query->addField('a', 'year');
    $query->condition('a.unit_id', $this->unit_id);
    $query->condition('a.year', $year);
    $result = $query->execute()->fetchAll(PDO::FETCH_ASSOC);
    if (count($result) > 0) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function addWeeklyEvent(RoomsEventInterface $event) {
    // First check if the month exists and do an update if so
    if ($this->YearDefined($event->startYear())) {
      $partial_month_row = $this->preparePartialMonthArray($event);
      $update = db_update($this->base_table)
        ->condition('unit_id', $this->unit_id)
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
      );
      $month_row = array_merge($month_row, $days);
      $insert = db_insert($this->base_table)->fields($month_row);
      $insert->execute();
    }
  }
}

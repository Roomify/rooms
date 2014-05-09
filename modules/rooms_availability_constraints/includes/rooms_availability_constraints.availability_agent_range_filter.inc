<?php

/**
 * Filter by group_size and group_size_children.
 */
class AvailabilityAgentDurationFilter extends AvailabilityAgentCommonFilter {

  const MINIMUM_DURATION_FILTER = 'minimum';
  const MAXIMUM_DURATION_FILTER = 'maximum';

  const ROOMS_UNIT_TYPE_LEVEL = 'rooms_unit_type';
  const ROOMS_UNIT_LEVEL = 'rooms_unit';

  /**
   * The start date datetime.
   *
   * @var DateTime
   */
  protected $start_date;

  /**
   * The end date datetime.
   *
   * @var DateTime
   */
  protected $end_date;

  /**
   * The interval between start and end dates.
   *
   * @var DateInterval
   */
  protected $interval;

  /**
   * Track of discarded units to exclude it form nest queries.
   *
   * @var array
   */
  protected $excluded_units;

  protected $excluded_unit_types;

  public function __construct($units, $parameters) {
    parent::__construct($units, $parameters);
    $this->start_date = $this->parameters['start_date'];

    $this->end_date = $this->parameters['end_date'];
    $this->end_date = $this->end_date->modify('+1 days');

    $this->interval = date_diff($this->start_date, $this->end_date);

    $this->excluded_units = array();
    $this->excluded_unit_types = array();
  }

  /**
   * @return array
   */
  public function applyFilter() {

    $this->applyMustDayFilter(static::ROOMS_UNIT_TYPE_LEVEL);
    if (empty($this->units)) {
      return array();
    }

    $this->applyIfDayDurationFilter(static::ROOMS_UNIT_TYPE_LEVEL);
    if (empty($this->units)) {
      return array();
    }

    $this->applyDurationFilter(static::ROOMS_UNIT_TYPE_LEVEL);
    if (empty($this->units)) {
      return array();
    }

    $this->applyMustDayFilter(static::ROOMS_UNIT_LEVEL);
    if (empty($this->units)) {
      return array();
    }

    $this->applyIfDayDurationFilter(static::ROOMS_UNIT_LEVEL);
    if (empty($this->units)) {
      return array();
    }

    $this->applyDurationFilter(static::ROOMS_UNIT_LEVEL);

    return $this->units;
  }

  /**
   * Filters the units that does not match the must day filter.
   *
   * @param $entity_type
   */
  protected function applyMustDayFilter($entity_type) {
    $query = $this->basicFilter($entity_type);

    $query->fieldCondition('rooms_constraints_range', 'constraint_type', ROOMS_AVAILABILITY_CONSTRAINT_MUST_START, '=', 'rooms_availability_constraints');
    $query->fieldCondition('rooms_constraints_range', 'start_day', $this->start_date->format('N'), '!=', 'rooms_availability_constraints');

    $this->filterResults($entity_type, $query);
  }

  /**
   * Filters the units that does not match the if day filter.
   *
   * @param $entity_type
   */
  protected function applyIfDayDurationFilter($entity_type) {
    $this->applyIfDayRangeFilter($entity_type);
    $this->applyIfDayRangeFilter($entity_type, self::MAXIMUM_DURATION_FILTER);

  }

  /**
   * Filters the units that does not match the simple duration filter.
   *
   * @param $entity_type
   */
  protected function applyDurationFilter($entity_type) {
    $this->applyRangeFilter($entity_type);
    $this->applyRangeFilter($entity_type, self::MAXIMUM_DURATION_FILTER);
  }

  /**
   * Returns the basic filter query.
   *
   * @param $entity_type
   *
   * @return EntityFieldQuery
   *   The most basic filter.
   */
  protected function basicFilter($entity_type) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type)
      ->fieldCondition('rooms_constraints_range', 'start_date', $this->start_date->format('Y-m-d'), '<=', 'rooms_availability_constraints')
      ->fieldCondition('rooms_constraints_range', 'end_date', $this->end_date->format('Y-m-d'), '>=', 'rooms_availability_constraints');

    if ($entity_type == static::ROOMS_UNIT_LEVEL) {
      $this->basicUnitFilterConditions($query);
    }
    else {
       $this->basicUnitTypeFilterConditions($query);
    }

    return $query;
  }

  /**
   * Returns specific conditions for basic filter at unit level.
   *
   * @param EntityFieldQuery $query
   */
  protected function basicUnitFilterConditions($query) {
    $query->propertyCondition('bookable', 1);
    if (!empty($this->excluded_units)) {
      $query->propertyCondition('unit_id', $this->excluded_units, 'NOT IN');
      if (!empty($this->excluded_unit_types)) {
        $query->propertyCondition('bundle', $this->excluded_unit_types, 'NOT IN');
      }
    }
  }

  /**
   * Returns specific conditions for basic filter at unit type level.
   *
   * @param EntityFieldQuery $query
   */
  protected function basicUnitTypeFilterConditions($query) {
    if (!empty($this->excluded_unit_types)) {
      $query->propertyCondition('type', $this->excluded_unit_types, 'NOT IN');
    }
  }

  /**
   * Filters the unit list given entity type and filter query.
   *
   * @param $entity_type
   * @param EntityFieldQuery $query
   */
  protected function filterResults($entity_type, $query) {
    if ($entity_type == static::ROOMS_UNIT_LEVEL) {
      $this->filterUnits($query);
    }
    else {
      $this->filterUnitTypes($query);
    }
  }

  /**
   * Filters the unit list at unit level given a filter query.
   *
   * @param EntityFieldQuery $query
   */
  protected function filterUnits($query) {
    $excluded_values = $query->execute();

    if (isset($excluded_values['rooms_unit'])) {
      foreach ($excluded_values['rooms_unit'] as $unit_id => $unit) {
        unset($this->units[$unit_id]);
        $this->excluded_units[$unit_id] = $unit_id;
      }
    }
  }

  /**
   * Filters the unit list at unit type level given a filter query.
   *
   * @param EntityFieldQuery $query
   */
  protected function filterUnitTypes($query) {
    $excluded_values = $query->execute();

    if (isset($excluded_values['rooms_unit_type'])) {
      $unit_types = entity_load('rooms_unit_type', array_keys($excluded_values['rooms_unit_type']));
      foreach ($unit_types as $unit_type) {
        foreach ($this->units as $unit_id => $unit) {
          if ($unit->type == $unit_type->type) {
            unset($this->units[$unit_id]);
            $this->excluded_units[$unit_id] = $unit_id;
          }
        }
        $this->excluded_unit_types[$unit_type->type] = $unit_type->type;
      }
    }
  }

  /**
   * Returns the duration filter query.
   *
   * @param $entity_type
   * @param $duration_filter
   *
   * @return EntityFieldQuery
   */
  protected function durationFilter($entity_type, $duration_filter) {
    $query = $this->basicFilter($entity_type);

    if ($duration_filter == static::MINIMUM_DURATION_FILTER) {
      $query->fieldCondition('rooms_constraints_range', 'minimum_stay', $this->interval->days, '>', 'rooms_availability_constraints');
    }
    else {
      $query->fieldCondition('rooms_constraints_range', 'maximum_stay', $this->interval->days, '<', 'rooms_availability_constraints');
    }

    return $query;
  }

  /**
   * Applies the range filter for a given starting day.
   *
   * @param $entity_type
   * @param $duration_filter
   */
  protected function applyIfDayRangeFilter($entity_type, $duration_filter = self::MINIMUM_DURATION_FILTER) {
    $query = $this->durationFilter($entity_type, $duration_filter);

    $query->fieldCondition('rooms_constraints_range', 'constraint_type', ROOMS_AVAILABILITY_CONSTRAINT_IF_STARTS, '=', 'rooms_availability_constraints');
    $query->fieldCondition('rooms_constraints_range', 'start_day', $this->start_date->format('N'), '=', 'rooms_availability_constraints');

    $this->filterResults($entity_type, $query);
  }

  /**
   * Applies the generic range filter.
   *
   * @param $entity_type
   * @param $duration_filter
   */
  protected function applyRangeFilter($entity_type, $duration_filter = self::MINIMUM_DURATION_FILTER) {
    $query = $this->durationFilter($entity_type, $duration_filter);
    $query->fieldCondition('rooms_constraints_range', 'start_day', 0, '<', 'rooms_availability_constraints');
    $this->filterResults($entity_type, $query);
  }

}
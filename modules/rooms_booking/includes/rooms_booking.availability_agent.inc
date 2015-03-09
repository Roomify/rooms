<?php

/**
 * @file
 * Contains the AvailabilityAgent.
 */

/**
 * An AvailabilityAgent provides access to the availability functionality of
 * Rooms and lets you query for availability, get pricing information and create
 * products that can be bought.
 *
 * The Agent is essentially a factory creating the appropriate responses for us
 * as needed based on the requests and the current status of our bookable units.
 *
 * An Agent reasons over a single set of information regarding a booking which
 * are exposed as public variables to make it easy for us to set and or change them.
 */
class AvailabilityAgent {

  /**
   * The start date for availability search.
   *
   * @var DateTime
   */
  public $start_date;

  /**
   * The departure date
   *
   * @var DateTime
   */
  public $end_date;

  /**
   * How many people we are looking to accommodate.
   *
   * @var array
   */
  public $booking_parameters;

  /**
   * How many booking units are we looking for.
   *
   * @var int
   */
  public $booking_units;

  /**
   * The states to consider valid for an availability search.
   *
   * @var array
   */
  public $valid_states;

  /**
   * What unit types we are looking for.
   *
   * @var array
   */
  public $unit_types;

  /**
   * Stores available rooms for each booking_parameters.
   *
   * @var array
   */
  public $rooms_results = array();

  /**
   * Stores first valid rooms combination for booking_parameters in input.
   *
   * @var array
   */
  public $valid_rooms_combination = array();


  /**
   * Construct the AvailabilityAgent instance.
   *
   * @param DateTime $start_date
   *   The start date.
   * @param DateTime $end_date
   *   The end date.
   * @param array $booking_parameters
   *   Parameters to include in the search.
   * @param int $booking_units
   *   Number of units to book.
   * @param array $valid_states
   *   Valid states to perform the availability search.
   * @param array $unit_types
   *   Unit types to perform the search.
   */
  public function __construct($start_date, $end_date, $booking_parameters = array(), $booking_units = 1, $valid_states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST, ROOMS_UNCONFIRMED_BOOKINGS), $unit_types = array()) {
    $this->valid_states = $valid_states;
    $this->start_date = $start_date;
    // For availability purposes the end date is a day earlier than checkout.
    $this->end_date = clone($end_date);
    $this->end_date->sub(new DateInterval('P1D'));
    $this->booking_parameters = $booking_parameters;
    $this->booking_units = $booking_units;
    $this->unit_types = $unit_types;
  }


  /**
   * Sets the valid states for an availability search.
   *
   * Defaults are "ROOMS_AVAILABLE" and "ROOMS_ON_REQUEST"
   *
   * @param array $states
   *   The valid states to perform the search.
   */
  public function setValidStates($states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST, ROOMS_UNCONFIRMED_BOOKINGS)) {
    $this->valid_states = $states;
  }


  /**
   * Searches for availability inside a set of bookable units.
   *
   * This function is used to recursively iterate over sets of rooms identifying
   * whether there is a solution across the sets that has at least one option in
   * each set.
   *
   * @param array $rooms_results
   *   Bookable units to perform the search.
   */
  private function searchForAvailability($rooms_results) {
    $rooms_results_keys = array_keys($rooms_results);
    $el_key = array_shift($rooms_results_keys);

    if (!isset($rooms_results[$el_key])) {
      return 0;
    }

    $candidate_keys = array_keys($rooms_results[$el_key]);
    if (empty($candidate_keys)) {
      return 0;
    }

    foreach ($candidate_keys as $c_key) {
      $tmp_rooms_results = $rooms_results;

      foreach ($tmp_rooms_results as $key => $value) {
        if (isset($tmp_rooms_results[$key][$c_key]) && $key != $el_key) {
          unset($tmp_rooms_results[$key][$c_key]);
        }
      }

      // Combination fails, rollback and try a new combination.
      if (empty($tmp_rooms_results[$el_key])) {
        return 0;
      }

      $this->valid_rooms_combination[] = $tmp_rooms_results[$el_key][$c_key];

      unset($tmp_rooms_results[$el_key]);

      if (empty($tmp_rooms_results)) {
        return 1;
      }

      // Call recursively this function.
      $return = $this->searchForAvailability($tmp_rooms_results);

      if ($return == 1) {
        return $return;
      }
    }
  }

  /**
   * Checks the availability.
   *
   * If valid units exist an array keyed by valid unit ids containing unit and
   * the states it holds during the requested period or a message as to what
   * caused the failure.
   *
   * @param bool $confirmed
   *   Whether include confirmed states or not.
   *
   * @return array|int
   *   Bookable units remaining after the filter, error code otherwise.
   */
  public function checkAvailability($confirmed = FALSE) {
    // Determine the types of rooms that qualify - the sleeping potential of the
    // sum of the rooms should satisfy the group size.
    // If no booking_parameters or no group size get all available units.
    if ($this->booking_parameters == array() || $this->booking_units == 0) {
      $results = $this->applyAvailabilityFilter();

      if ($results == ROOMS_NO_ROOMS) {
        return ROOMS_NO_ROOMS;
      }
    }
    else {
      $this->rooms_results = array();

      foreach ($this->booking_parameters as $key => $parameter) {
        $adults = 0;
        $children = 0;

        if (isset($parameter['adults'])) {
          $adults = $parameter['adults'];
        }
        if (isset($parameter['children'])) {
          $children = $parameter['children'];
        }

        $this->rooms_results[$key] = $this->applyAvailabilityFilter(array(), $adults, $children, $confirmed);
      }

      if (!empty($this->rooms_results)) {
        $this->valid_rooms_combination = array();

        // If a valid combination exist for booking request.
        if ($this->searchForAvailability($this->rooms_results) == 1) {
          $results = array();

          foreach ($this->rooms_results as $result) {
            $results = $results + $result;
          }
        }
        else {
          return ROOMS_NO_ROOMS;
        }
      }
      else {
        return ROOMS_NO_ROOMS;
      }
    }

    // Of the rooms that fit the criteria lets see what availability we have.
    $units = $this->getUnitsByPriceType($results);

    if (count($units) == 0) {
      return ROOMS_NO_ROOMS;
    }
    else {
      return $units;
    }
  }

  /**
   * Returns availability for a specific unit.
   *
   * @param int $unit_id
   *   Bookable unit to check availability for.
   * @param array $price_modifiers
   *   Price modifiers to apply.
   *
   * @return array|int
   *   Bookable unit if available, error code otherwise.
   */
  public function checkAvailabilityForUnit($unit_id, $price_modifiers = array()) {
    // Load the unit.
    $unit = rooms_unit_load($unit_id);

    $units = $this->getUnitsByPriceType(array($unit_id => $unit), $price_modifiers);
    $units = array_pop($units);
    $units = array_pop($units);

    if (count($units) == 0) {
      return ROOMS_NO_ROOMS;
    }
    else {
      return $units;
    }
  }

  /**
   * Applies the availability filter against a set of bookable units.
   *
   * @param array $units
   *   Set of bookable units to filter.
   * @param int $adults
   *   Number of adults.
   * @param int $children
   *   Number of children.
   * @param bool $confirmed
   *   Whether include confirmed states or not.
   *
   * @return array|int
   *   bookable units remaining after the filter, error code otherwise.
   */
  protected function applyAvailabilityFilter($units = array(), $adults = 0, $children = 0, $confirmed = FALSE) {
    // Apply AvailabilityAgentSizeFilter.
    $av_sizefilter = new AvailabilityAgentSizeFilter($units, array('group_size' => $adults, 'group_size_children' => $children, 'unit_types' => $this->unit_types));
    $units = $av_sizefilter->applyFilter();

    if ($units == ROOMS_SIZE_FAILURE) {
      return array();
    }

    // Apply AvailabilityAgentSingleUnitFilter.
    $av_singleunitfilter = new AvailabilityAgentSingleUnitFilter($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date));
    $units = $av_singleunitfilter->applyFilter();

    // Apply AvailabilityAgentDateFilter.
    $av_datefilter = new AvailabilityAgentDateFilter($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date, 'valid_states' => $this->valid_states, 'confirmed' => $confirmed));
    $units = $av_datefilter->applyFilter();

    if (empty($units)) {
      return array();
    }

    // Apply AvailabilityAgentCommerceFilter.
    $av_commercefilter = new AvailabilityAgentCommerceFilter($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date));
    $units = $av_commercefilter->applyFilter();

    ctools_include('plugins');
    $filters = ctools_get_plugins('rooms_booking', 'availabilityagent_filter');

    foreach ($filters as $filter) {
      $class = ctools_plugin_get_class($filter, 'handler');
      $object_filter = new $class($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date, 'group_size' => $adults, 'group_size_children' => $children, 'unit_types' => $this->unit_types, 'valid_states' => $this->valid_states, 'confirmed' => $confirmed));

      $units = $object_filter->applyFilter();
    }

    return $units;
  }

  /**
   * Returns the units array in a specific format based on price.
   *
   * @param array $results
   *   Units to sort.
   * @param array $price_modifiers
   *   Price modifiers.
   *
   * @return array
   *   Units in a price based structure.
   */
  protected function getUnitsByPriceType($results, $price_modifiers = array()) {
    $units = array();

    if (count($results) > 0) {
      foreach ($results as $unit) {
        // Get the actual entity.
        $unit = rooms_unit_load($unit->unit_id);

        // Get a calendar and check availability.
        $rc = new UnitCalendar($unit->unit_id);
        // We need to make this based on user-set vars.
        // Rather than using $rc->stateAvailability we will get the states check
        // directly as different states will impact on what products we create.
        $states = $rc->getStates($this->start_date, $this->end_date);

        // Calculate the price as well to add to the array.
        $temp_end_date = clone($this->end_date);
        $temp_end_date->add(new DateInterval('P1D'));

        $booking_info = array(
          'start_date' => clone($this->start_date),
          'end_date' => $temp_end_date,
          'unit' => $unit,
          'booking_parameters' => $this->booking_parameters,
        );

        // Give other modules a chance to change the price modifiers.
        drupal_alter('rooms_price_modifier', $price_modifiers, $booking_info);

        $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);

        if (variable_get('rooms_price_calculation', ROOMS_PER_NIGHT) == ROOMS_PER_PERSON && count($this->booking_parameters) == 1 && isset($this->booking_parameters[0]) && is_array($this->booking_parameters)) {
          $price_log = $price_calendar->calculatePrice($this->start_date,
            $this->end_date, $this->booking_parameters[0]['adults'], $this->booking_parameters[0]['children'], $this->booking_parameters[0]['childrens_age']);
        }
        else {
          $price_log = $price_calendar->calculatePrice($this->start_date,
            $this->end_date);
        }
        $full_price = $price_log['full_price'];

        $units[$unit->type][$full_price][$unit->unit_id]['unit'] = $unit;
        $units[$unit->type][$full_price][$unit->unit_id]['price'] = $full_price;
        $units[$unit->type][$full_price][$unit->unit_id]['booking_price'] = $price_log['booking_price'];
        $units[$unit->type][$full_price][$unit->unit_id]['price_log'] = $price_log['log'];


        if (in_array(ROOMS_ON_REQUEST, $states)) {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_ON_REQUEST;
        }
        else {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_AVAILABLE;
        }
      }
    }

    // We order units by optional items to ensure that units with options are
    // the first to be picked by a user.
    $units = $this->orderByOptionals($units);

    return $units;
  }


  /**
   * Ordering units by the optional items that are available.
   *
   * @param array $units
   *   Units to sort.
   *
   * @return array
   *   Sorted units by number of options.
   */
  protected function orderByOptionals($units) {
    foreach ($units as $type => $v) {
      foreach ($v as $price => $value) {
        uasort($value, array(get_class($this), 'compareByOptionals'));
        $units[$type][$price] = $value;
      }
    }

    return $units;
  }

  /**
   * Compares two bookable units based on the number of available options.
   *
   * @param array $unit_a
   *   First unit.
   * @param array $unit_b
   *   Second unit.
   *
   * @return int
   *   Comparison result.
   */
  protected static function compareByOptionals($unit_a, $unit_b) {
    $a_items = rooms_unit_get_unit_options($unit_a['unit']);
    $b_items = rooms_unit_get_unit_options($unit_b['unit']);

    if (count($a_items) == count($b_items)) {
      return $unit_a['unit']->unit_id < $unit_b['unit']->unit_id ? 1 : -1;
    }
    else {
      return count($a_items) < count($b_items) ? 1 : -1;
    }
  }

}

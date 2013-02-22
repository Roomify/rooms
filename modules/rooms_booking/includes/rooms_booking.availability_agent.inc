<?php

/**
 * An AvailabilityAgent provides access to the availability functionality of Rooms and
 * lets you query for availability, get pricing information and create products that can be bought.
 *
 * The Agent is essentially a factory cretaing the appropriate responses for us as needed based on the
 * requests and the current status of our bookable units.
 *
 * An Agent reasons over a single set of information regarding a booking which are exposed as public
 * variables to make it easy for us to set and or change them.
 */
class AvailabilityAgent {

  // The states to consider valid for an availability search
  public $valid_states;

  // The start date for availability search
  public $start_date;

  // The departure date
  public $end_date;

  // How many people we are looking to accommodate
  public $booking_parameters;

  // How many booking units are we looking for
  public $booking_units;

  // What unit types we are looking for
  public $unit_types;

  // Store available rooms for each booking_parameters
  public $rooms_results = array();

  // Store first valid rooms combination for booking_parameters in input
  public $valid_rooms_combination = array();
  

  public function __construct($start_date, $end_date, $booking_parameters = array(), $booking_units = 1, $valid_states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST, ROOMS_UNCONFIRMED_BOOKINGS), $unit_types = array()) {
    $this->valid_states = $valid_states;

    $date_format = str_replace('-', '/', variable_get('rooms_date_format', 'd-m-Y'));
    $this->start_date = DateTime::createFromFormat($date_format, str_replace('-', '/',$start_date));
    $checkout_date = DateTime::createFromFormat($date_format, str_replace('-', '/', $end_date));

    // For availability purposes the end date is a day earlier than the checkout date
    $this->end_date = $checkout_date->sub(new DateInterval('P1D'));

    $this->booking_parameters = $booking_parameters;
    $this->booking_units = $booking_units;
    $this->unit_types = $unit_types;
  }


  /**
   * Sets the valid states for an availability search - default is "ROOMS_AVAILABLE" and "ROOMS_ON_REQUEST"
   */
  public function setValidStates($states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST, ROOMS_UNCONFIRMED_BOOKINGS)) {
    $this->valid_states = $states;
  }


  /**
   * This function is used to recursively iterate over sets of rooms identifying
   * whether there is a solution across the sets that has at least one option in 
   * each set.
   */
  private function searchForAvailability($rooms_results) {
    $el_key = array_shift(array_keys($rooms_results));

    if (!isset($rooms_results[$el_key]['rooms_unit'])) return 0;

    $candidate_keys = array_keys($rooms_results[$el_key]['rooms_unit']);
    if (empty($candidate_keys)) return 0;

    foreach ($candidate_keys as $c_key) {
      $tmp_rooms_results = $rooms_results;

      foreach ($tmp_rooms_results as $key => $value) {
        if (isset($tmp_rooms_results[$key]['rooms_unit'][$c_key]) && $key != $el_key) {
          unset($tmp_rooms_results[$key]['rooms_unit'][$c_key]);
        }
      }

      // Combination fails, roolback and try a new combination
      if (empty($tmp_rooms_results[$el_key]['rooms_unit'])) return 0;

      $this->valid_rooms_combination[] = $tmp_rooms_results[$el_key]['rooms_unit'][$c_key];

      unset($tmp_rooms_results[$el_key]);

      if (empty($tmp_rooms_results)) return 1;

      // Call recursively this function
      $return = $this->searchForAvailability($tmp_rooms_results);

      if ($return == 1) return $return;
    }
  }

  /**
   * If valid units exist an array keyed by valid unit ids containing unit and the states it holds during
   * the requested period or a message as to what caused the failure.
   */
  public function checkAvailability() {

    global $user;

    $units = array();

    $results = array();

    // Determine the types of rooms that qualify - the sleeping potential of the sum
    // of the rooms should satisfy the group size

    // If no booking_parameters or no group size get all available units
    if ($this->booking_parameters == array() || $this->booking_units == 0) {
      $results = applyAvailabilityFilter();

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

        $this->rooms_results[$key] = $this->applyAvailabilityFilter(array(), $adults, $children);
      }

      if (!empty($this->rooms_results)) {
        $this->valid_rooms_combination = array();

        // If a valid combination exist for booking request
        if ($this->searchForAvailability($this->rooms_results) == 1) {
          $results = array('rooms_unit' => array());

          foreach ($this->rooms_results as $result) {
            $results['rooms_unit'] = $results['rooms_unit'] + $result['rooms_unit'];
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

    // Of the rooms that fit the criteria lets see what availability we have
    $units = $this->getUnitsByPriceType($results);

    if (count($units) == 0) {
      return ROOMS_NO_ROOMS;
    }
    else {
     return $units;
    }
  }

  /**
   * Return availability for a specific unit given the date ranges setup previously
   *
   * TODO: This code repeats what is above - we can shove off into a different function
   * that both make use of.
   */
  function checkAvailabilityForUnit($unit_id) {

    $units = array();

    // Load the unit
    $unit = rooms_unit_load($unit_id);

    $units = array_pop(array_pop($this->getUnitsByPriceType(array('rooms_unit' => array($unit_id => $unit)))));
    
    if (count($units) == 0) {
      return ROOMS_NO_ROOMS;
    }
    else {
     return $units;
    }
  }


  function applyAvailabilityFilter($units = array(), $adults = 0, $children = 0) {
    // Apply AvailabilityAgentSizeFilter
    $av_sizefilter = new AvailabilityAgentSizeFilter($units, array('group_size' => $adults, 'group_size_children' => $children));
    $units = $av_sizefilter->applyFilter();

    // Apply AvailabilityAgentDateFilter
    $av_datefilter = new AvailabilityAgentDateFilter($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date, 'valid_states' => $this->valid_states));
    $units = $av_datefilter->applyFilter();

    // Apply AvailabilityAgentCommerceFilter
    $av_commercefilter = new AvailabilityAgentCommerceFilter($units, array('start_date' => $this->start_date, 'end_date' => $this->end_date));
    $results = $av_commercefilter->applyFilter();

    return $results;
  }


  function getUnitsByPriceType($results) {
    $units = array();

    if (count($results) > 0) {
      foreach ($results['rooms_unit'] as $unit) {
        // Get the actual entity
        $unit = rooms_unit_load($unit->unit_id);

        // Get a calendar and check availability
        $rc = new UnitCalendar($unit->unit_id);
        // We need to make this based on user-set vars

        // Rather than using $rc->stateAvailability we will get the states check directly
        // as different states will impact on what products we create.
        $states = $rc->getStates($this->start_date, $this->end_date);

        // Calculate the price as well to add to the array
          
        // First set up price modifiers
        $price_modifiers = array();

        $booking_info = array(
          'start_date' => $this->start_date,
          'end_date' => $this->end_date,
          'unit' => $unit,
          'booking_parameters' => $this->booking_parameters
        );

        // Give other modules a chance to change the price modifiers
        drupal_alter('rooms_price_modifier', $price_modifiers, $booking_info);

        $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);
        $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
        $full_price = $price['full_price'];

        $units[$unit->type][$full_price][$unit->unit_id]['unit'] = $unit;
        $units[$unit->type][$full_price][$unit->unit_id]['price'] = $full_price;
        $units[$unit->type][$full_price][$unit->unit_id]['booking_price'] = $price['booking_price'];

        if ($unit->data['singlediscount'] != '') {
          if (is_numeric($unit->data['singlediscount'])) {
            // First set up price modifiers
            $price_modifiers = array('single' => array('#type' => ROOMS_PRICE_SINGLE_OCCUPANCY));

            $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);
            $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
            $price_singlediscount = $price['full_price'];

            $units[$unit->type][$full_price][$unit->unit_id]['singlediscount'] = $price_singlediscount;
          }
        }

        if (in_array(ROOMS_ON_REQUEST, $states)) {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_ON_REQUEST;
        }
        else {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_AVAILABLE;
        }
      }
    }

    return $units;
  }

}

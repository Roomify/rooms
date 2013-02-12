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

  public $units = array();

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
   * Sets the valid states for an availability search - default is "ROOMS_AVAILABLE" and
   * "ROOMS_ON_REQUEST"
   */
  public function setValidStates($states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST, ROOMS_UNCONFIRMED_BOOKINGS)) {
    $this->valid_states = $states;
  }

  private function getAvailabilityRooms($group_size, $group_size_children) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'rooms_unit')
          ->propertyOrderBy('sleeps', 'ASC')
          ->propertyCondition('sleeps', $group_size, '>=')
           ->propertyCondition('bookable', 1);

    if ($group_size_children != PHP_INT_MAX) {
      $query->propertyCondition('children', $group_size_children, '>=');
    }

    // Add a unit type condition if this has been defined
    if (count($this->unit_types) > 0) {
      $query->propertyCondition('type', $this->unit_types);
    }

    // Execute the query and collect the results
    $results = $query->execute();

    if (count($results) == 0) {
      return ROOMS_SIZE_FAILURE;
    }

    return $results;
  }

  private function searchForAvailability($rooms_results) {
    $tmp = array_shift(array_values($rooms_results));
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

      if (empty($tmp_rooms_results[$el_key]['rooms_unit'])) return 0;

      unset($tmp_rooms_results[$el_key]);

      if (empty($tmp_rooms_results)) return 1;

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
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'rooms_unit')
            ->propertyCondition('bookable', 1);

      // Add a unit type condition if this has been defined
      if (count($this->unit_types) > 0) {
        $query->propertyCondition('type', $this->unit_types);
      }

      $results = $query->execute();
      if (count($results) == 0) {
        return ROOMS_NO_ROOMS;
      }
    }

    else {
      $rooms_results = array();

      $min_group_size = PHP_INT_MAX;
      $min_group_size_children = PHP_INT_MAX;
      foreach ($this->booking_parameters as $key => $parameter) {
        $adults = 0;
        $children = 0;

        if (isset($parameter['adults'])) {
          if ($min_group_size > $parameter['adults']) $min_group_size = $parameter['adults'];
          $adults = $parameter['adults'];
        }
        if (isset($parameter['children'])) {
          if ($min_group_size_children > $parameter['children']) $min_group_size_children = $parameter['children'];
          $children = $parameter['children'];
        }
        else {
          $min_group_size_children = 0;
        }

        $rooms_results[$key] = $this->getAvailabilityRooms($adults, $children);

        if (in_array(COMMERCE_UNCORFIMED, variable_get('rooms_commerce_uncorfirmed', array(COMMERCE_UNCORFIMED)))) {
          $rooms_results = $this->filterByLineItems($rooms_results);
        }
      }

      if (!empty($rooms_results)) {
        if ($this->searchForAvailability($rooms_results) == 1) {
          $results = $this->getAvailabilityRooms($min_group_size, $min_group_size_children);

          if (in_array(COMMERCE_UNCORFIMED, variable_get('rooms_commerce_uncorfirmed', array(COMMERCE_UNCORFIMED)))) {
            $results = $this->filterByLineItems($results);
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
    if (count($results) > 0) {
      // Check for rooms currently in the logged in user's cart, so that they will not
      // be able to add them again.
      $units_in_cart = array();
      if (variable_get('rooms_checkout_style', ROOMS_COMMERCE_CHECKOUT) == ROOMS_COMMERCE_CHECKOUT) {
        if ($order = commerce_cart_order_load($user->uid)) {
          $wrapper = entity_metadata_wrapper('commerce_order', $order);
  
          // See if there are any product line items.
          if (commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types()) > 0) {
            // Get the unit IDs already in the cart.
            foreach ($wrapper->commerce_line_items as $line_item) {
              //Need to check if line_item_type = 'rooms_booking', otherwise errors if cart contains non-rooms product(s)
              if ($line_item->type == 'rooms_booking') {
                $line_item_values = $line_item->value();
                // Add this unit's ID to the list of units in the cart.
                $units_in_cart[] = $line_item_values->rooms_booked_unit_id['und'][0]['value'];
              }
            }
          }
        }
      }

      foreach ($results['rooms_unit'] as $unit) {
        // Check if this unit is in the user's cart, and skip adding
        // it to the available units if so.
        if (array_search($unit->unit_id, $units_in_cart) !== FALSE) {
          // @TODO: make displaying this message optional?
          drupal_set_message(t('A unit or units was not displayed, as it is already in your cart.'), 'status', FALSE);
          continue;
        }

        // Get the actual entity
        $unit = rooms_unit_load($unit->unit_id);

        // Get a calendar and check availability
        $rc = new UnitCalendar($unit->unit_id);
        // We need to make this based on user-set vars

        // Rather than using $rc->stateAvailability we will get the states check directly
        // as different states will impact on what products we create.
        $states = $rc->getStates($this->start_date, $this->end_date);
        $state_diff = array_diff($states, $this->valid_states);
        if (count($state_diff) == 0) {
          // Calculate the price as well to add to the array

          // First set up price modifiers
          $price_modifiers = array();

          /*if ($this->group_size == 1) {
            $price_modifiers['single'] = array('#type' => ROOMS_PRICE_SINGLE_OCCUPANCY);
          }

          if ($this->group_size_children > 0) {
            $price_modifiers['children'] = array(
              '#type' => ROOMS_DYNAMIC_MODIFIER,
              '#op_type' => ROOMS_ADD,
              '#amount' => $unit->cot_surcharge * ($this->start_date->diff($this->end_date)->days + 1),
            );
          }*/

          // Give other modules a chance to change the price modifiers
          drupal_alter('rooms_price_modifier', $price_modifiers);

          $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);
          $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
          $full_price = $price['full_price'];
          $units[$unit->type][$full_price][$unit->unit_id]['unit'] = $unit;
          $units[$unit->type][$full_price][$unit->unit_id]['price'] = $full_price;
          $units[$unit->type][$full_price][$unit->unit_id]['booking_price'] = $price['booking_price'];
          if (in_array(ROOMS_ON_REQUEST, $states)) {
            $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_ON_REQUEST;
          }
          else {
            $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_AVAILABLE;
          }

        }
      }
    }

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

    // Get a calendar and check availability
    $rc = new UnitCalendar($unit_id);

    // We need to make this based on user-set vars

    // Rather than using $rc->stateAvailability we will get the states check directly
    // as different states will impact on what products we create.
    $states = $rc->getStates($this->start_date, $this->end_date);
    $state_diff = array_diff($states, $this->valid_states);
    if (count($state_diff) == 0) {
      // Calculate the price as well to add to the array

      // First set up price modifiers
      $price_modifiers = array();

      /*if ($this->group_size == 1) {
        $price_modifiers['single'] = array('#type' => ROOMS_PRICE_SINGLE_OCCUPANCY);
      }

      if ($this->group_size_children > 0) {
        $price_modifiers['children'] = array(
          '#type' => ROOMS_DYNAMIC_MODIFIER,
          '#op_type' => ROOMS_ADD,
          '#amount' => $unit->cot_surcharge * ($this->start_date->diff($this->end_date)->days + 1),
        );
      }*/

      // Give other modules a chance to change the price modifiers
      drupal_alter('rooms_price_modifier', $price_modifiers);

      $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);
      $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
      $units[$unit->unit_id]['unit'] = $unit;
      $units[$unit->unit_id]['price'] = $price['full_price'];
      $units[$unit->unit_id]['booking_price'] = $price['booking_price'];
      if (in_array(ROOMS_ON_REQUEST, $states)) {
        $units[$unit->unit_id]['state'] = ROOMS_ON_REQUEST;
      }
      else {
        $units[$unit->unit_id]['state'] = ROOMS_AVAILABLE;
      }
    }

    if (count($units) == 0) {
      return ROOMS_NO_ROOMS;
    }
    else {
     return $units;
    }
  }

  /**
   */
  function filterByLineItems($rooms_results) {
    global $user;

    $order = commerce_cart_order_load($user->uid);

    if (isset($order->commerce_line_items['und'])) {
      foreach ($order->commerce_line_items['und'] as $key => $value) {

        $line_item = commerce_line_item_load($value['line_item_id']);

        $s_date = new DateTime($line_item->rooms_booking_dates['und'][0]['value']);
        $e_date = new DateTime($line_item->rooms_booking_dates['und'][0]['value2']);

        if ( ($s_date < $this->start_date && $e_date > $this->end_date) ||
             ($s_date > $this->start_date && $e_date < $this->end_date) ||
             ($s_date < $this->start_date && $e_date > $this->start_date) ||
             ($s_date < $this->end_date && $e_date > $this->end_date) ) {

          foreach ($rooms_results as $key2 => $res) {
            if (isset($rooms_results[$key2]['rooms_unit'][$line_item->rooms_booked_unit_id['und'][0]['value']])) {
              unset($rooms_results[$key2]['rooms_unit'][$line_item->rooms_booked_unit_id['und'][0]['value']]);
            }
          }

          if (isset($rooms_results['rooms_unit'])) {
            unset($rooms_results['rooms_unit'][$line_item->rooms_booked_unit_id['und'][0]['value']]);
          }
        }
      }
    }

    return $rooms_results;
  }

}

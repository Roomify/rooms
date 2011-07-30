<?php

/**
 * An AvailabilityAgent provides access to the availability functionality of DrupalRooms and
 * lets you query for availability, get pricing information and create products that can be bought.
 *
 * The Agent essentially acts like a factory creating the appropriate responses for us when needed.
 */
class AvailabilityAgent {
  
  // The states to consider valid for an availability search
  public $valid_states;
  
  public $start_date;
  
  public $end_date;
  
  public $group_size;
  
  public $booking_units;
  
  public $units = array();
  
  public function __construct($start_date, $end_date, $group_size = 2, $booking_units = 1, $valid_states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST)) {
    $this->valid_states = $valid_states;
    $this->start_date = new DateTime($start_date);
    $this->end_date = new DateTime($end_date);
    $this->group_size = $group_size;
    $this->booking_units = $booking_units;
  }
  
  /**
   * Creates products for each booking unit
   *
   * @param $unit_id
   * The id of the booking unit for which we are creating a product
   *
   * @param $status
   * The status of the room - this will determine the order status
   *
   * @returns A Commerce product
   */
  public function createProduct($unit_id, $status) {

    $unit = rooms_unit_load($unit_id);
    
    // Set the correct departure date for the product name (checkout day)
    $dd = new DateTime($this->end_date->format('Y-m-d'));
    $dd->add(new DateInterval('P1D'));
  
    // Create product
    $room_booking = commerce_product_new('rooms_booking');
    // SKU's need to be unique so we are adding system time to each product
    $room_booking->sku = 'UNIT' . $unit_id . ' - ' . time();
    // Product name - we set something that will make sense to the user here - this needs to be parametrised of course
    $room_booking->title = 'Booking for ' . $unit->type . ' Arrival: ' . $this->start_date->format('Y-m-d') . ' Departure: ' . $dd->format('Y-m-d');
    // Save the product and then we will add fields to it.
    commerce_product_save($room_booking);
  
    // Calculate the price
    $price_calendar = new UnitPricingCalendar($unit_id);
    $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
    // Convert to integer value as this is what Commerce expects
    $price = commerce_currency_decimal_to_amount($price, commerce_default_currency());
        
    $commerce_price = array(
      'und' => array(
          array( 
          'amount' => $price,
          'currency_code' => commerce_default_currency(),
          'data' => array(''),
        ),
      ),
    );
    
    $commerce_dates = array(
      'und' => array( array( 
        'value' => $this->start_date->format('Y-m-d'),
        'value2' => $this->end_date->format('Y-m-d'),
      )),
    );
  
    $commerce_unit = array(
      'und' => array( array(
        'value' => $unit->unit_id,
      )),
    );
  
    $commerce_status = array(
      'und' => array(array(
        'value' => $status,
      )),
    );
    
    $room_booking->commerce_price = $commerce_price;
    $room_booking->rooms_booking_dates = $commerce_dates;
    $room_booking->rooms_booked_unit_id = $commerce_unit;
    $room_booking->rooms_booked_status = $commerce_status;

    // Save the fields as well and return the product
    field_attach_update('commerce_product', $room_booking);
    
    return $room_booking;  
  }
  
  /**
   * Given a date range and some condition determine which rooms are available
   *
   * @param $start_date
   * The starting date for the search
   *
   * @param $end_date
   * The end date for the search
   *
   * @param $group_size
   * How many people we need to accommodate
   *
   * @param $rooms
   * In how many booking units can we accommodate the people
   *
   * @return
   * If valid units exist an array keyed by valid unit ids containing unit and the states it holds during
   * the requested period or a message as to what caused the failure.
   */
  public function checkAvailability() {
    
    $units = array();
    
    $results = array();

    // Determine the types of rooms that qualify - the sleeping potential of the sum
    // of the rooms should satisfy the group size
    
    // If BUs == 0 and no group size get all available units
    if ($this->group_size == 0 && $this->booking_units == 0) {
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'rooms_unit')
        ->propertyCondition('bookable', 1)
        ->execute();
      if (count($result) == 0) {
        return ROOMS_NO_ROOMS;
      }
    }
    
    // If BUs = 1 then we want rooms that can sleep as big as the group size
    elseif ($this->booking_units == 1) {
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'rooms_unit')
        ->propertyCondition('sleeps', $this->group_size, '>=')
        ->propertyCondition('bookable', 1)
        ->execute();
      if (count($result) == 0) {
        return ROOMS_SIZE_FAILURE;
      }
    }
    
    
    if (count($result) > 0) {
      //dpm($result['rooms_unit'], 'about to check availability');
      foreach ($result['rooms_unit'] as $unit) {
        // Get a calendar and check availability
        $rc = new UnitCalendar($unit->unit_id);
        // We need to make this based on user-set vars
        
        // Rather than using $rc->stateAvailability we will get the states check directly
        // as different states will impact on what products we create.
        $states = $rc->getStates($this->start_date, $this->end_date);
        $state_diff = array_diff($states, $this->valid_states);
        if (count($state_diff) == 0) {
          // Calculate the price as well to add to the array
          $price_calendar = new UnitPricingCalendar($unit->unit_id);
          $price = $price_calendar->calculatePrice($this->start_date, $this->end_date);
          $units[$unit->type][$price][$unit->unit_id]['unit'] = $unit;
          $units[$unit->type][$price][$unit->unit_id]['price'] = $price; 
          if (in_array(ROOMS_ON_REQUEST, $states)) {
            $units[$unit->type][$price][$unit->unit_id]['state'] = ROOMS_ON_REQUEST;  
          }
          else {
            $units[$unit->type][$price][$unit->unit_id]['state'] = ROOMS_AVAILABLE;  
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
  
}
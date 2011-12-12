<?php

/**
 * An AvailabilityAgent provides access to the availability functionality of Rooms and
 * lets you query for availability, get pricing information and create products that can be bought.
 *
 * The Agent is essentially a factory creting the appropriate responses for us as needed based on the
 * requests and the current status of our bookable units.
 *
 * An Agent reasons over a single set of information regarding a booking which are exposes as public
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
  public $group_size;
  
  // How many booking units are we looking for
  public $booking_units;
  
  // What unit types we are looking for
  public $unit_types;
  
  public $units = array();
  
  public function __construct($start_date, $end_date, $group_size = 2, $booking_units = 1, $valid_states = array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST), $unit_types = array()) {
    $this->valid_states = $valid_states;
    $this->start_date = new DateTime($start_date);
    $checkout_date = new DateTime($end_date);
    
    // For availability purposes the end date is a day earlier than the checkout date
    $this->end_date = $checkout_date->sub(new DateInterval('P1D'));
    
    $this->group_size = $group_size;
    $this->booking_units = $booking_units;
    $this->unit_types = $unit_types;
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
   * @returns A Commerce Product
   */
  public function createProduct($unit_id, $status) {

    $unit = rooms_unit_load($unit_id);
    
    $unit_type = rooms_unit_type_load($unit->type);
    
    // Set the correct departure date for the product name (checkout day)
    $dd = new DateTime($this->end_date->format('Y-m-d'));
    $dd->add(new DateInterval('P1D'));
  
    // Create product
    $room_booking = commerce_product_new('rooms_booking');
    // SKU's need to be unique so we are adding system time to each product
    $room_booking->sku = 'UNIT' . $unit_id . ' - ' . time();
    // Product name - we set something that will make sense to the user here - this needs to be parametrised of course
    $room_booking->title = 'Booking for ' . $unit_type->label . ' Arrival: ' . $this->start_date->format('Y-m-d') . ' Departure: ' . $dd->format('Y-m-d');
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
   * If valid units exist an array keyed by valid unit ids containing unit and the states it holds during
   * the requested period or a message as to what caused the failure.
   */
  public function checkAvailability() {
    
    $units = array();
    
    $results = array();

    // Determine the types of rooms that qualify - the sleeping potential of the sum
    // of the rooms should satisfy the group size
    
    // IF BUs > 1 then the sum of available rooms should accommodate the group - for now
    // just show all rooms - so we set to zero and go through default below    
    if ($this->booking_units > 1) {
      $this->booking_units = 0;
      $this->group_size = 0;
    }

    
    // If BUs == 0 and no group size get all available units
    if ($this->group_size == 0 && $this->booking_units == 0) {
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
    
    // If BUs = 1 then we want rooms that can sleep as big as the group size
    elseif ($this->booking_units == 1) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'rooms_unit')
            ->propertyCondition('sleeps', $this->group_size, '>=')
            ->propertyCondition('bookable', 1);
      
      // Add a unit type condition if this has been defined
      if (count($this->unit_types) > 0) {
        $query->propertyCondition('type', $this->unit_types);
      }
      
      // Execute the query and collect the results  
      $results = $query->execute();
      
      if (count($results) == 0) {
        return ROOMS_SIZE_FAILURE;
      }
    }
    
    
    // Of the rooms that fit the criteria lets see what availability we have
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
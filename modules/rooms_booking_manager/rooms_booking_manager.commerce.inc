<?php

/**
 * @file
 * Contains the main integration points with Drupal Commerce - eventually all commerce-related functions should move here
 * here.
 */

 
function rooms_create_line_item($unit_to_book, AvailabilityAgent $agent) {
    
    $unit_id = $unit_to_book['unit']->unit_id;
    $status = $unit_to_book['state'];

    $unit = rooms_unit_load($unit_id);

    $unit_type = rooms_unit_type_load($unit->type);

    // Set the correct departure date for the product name (checkout day)
    $dd = new DateTime($agent->end_date->format('Y-m-d'));
    $dd->add(new DateInterval('P1D'));

    $product_id = variable_get('rooms_booking_manager_booking_product_id', 0);
    $room_product = commerce_product_load($product_id);
    $line_item = commerce_product_line_item_new($room_product, 1, 0, array(), 'rooms_booking');

    // Product name - we set something that will make sense to the user here - this needs to be parametrised of course
    $line_item_label = 'Booking for ' . $unit_type->label . ' Arrival: ' . $agent->start_date->format('Y-m-d') . ' Departure: ' . $dd->format('Y-m-d');
    $line_item->line_item_label = $line_item_label;

    // Going to calculate the price now
    
    // First set up price modifiers
    $price_modifiers = array();
    
    if ($agent->group_size == 1) {
      $price_modifiers['single'] = array('#type' => ROOMS_PRICE_SINGLE_OCCUPANCY);
    }    
    
    // Give other modules a chance to change the price modifiers
    drupal_alter('rooms_price_modifier', $price_modifiers);

    $price_calendar = new UnitPricingCalendar($unit->unit_id, $price_modifiers);
    $price = $price_calendar->calculatePrice($agent->start_date, $agent->end_date);
    
    // Convert to integer value as this is what Commerce expects
    $price = commerce_currency_decimal_to_amount($price, commerce_default_currency());

    $commerce_unit_price = array(
      LANGUAGE_NONE => array(
          array(
          'amount' => $price,
          'currency_code' => commerce_default_currency(),
          'data' => array(''),
        ),
      ),
    );

    $commerce_dates = array(
      LANGUAGE_NONE => array( array(
        'value' => $agent->start_date->format('Y-m-d'),
        'value2' => $agent->end_date->format('Y-m-d'),
      )),
    );

    $commerce_unit = array(
      LANGUAGE_NONE => array( array(
        'value' => $unit->unit_id,
      )),
    );

    $commerce_status = array(
      LANGUAGE_NONE => array(array(
        'value' => $status,
      )),
    );

    $line_item->commerce_unit_price = $commerce_unit_price;
    $line_item->rooms_booking_dates = $commerce_dates;
    $line_item->rooms_booked_unit_id = $commerce_unit;
    $line_item->rooms_booked_status = $commerce_status;
    
    // After we added the price to the line item we rebase (!) it to make
    // sure it gets picked up in the order data and the totals are correct.
    commerce_line_item_rebase_unit_price(&$line_item);
    
    return $line_item;
}
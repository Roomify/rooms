<?php

/**
 * @file
 * Default rule configurations for Rooms Booking Manager.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rooms_booking_manager_default_rules_configuration() {
  $rules = array();

  $rule = '{ "rooms" : {
    "LABEL" : "Rooms",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "-100",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rooms_booking_manager", "commerce_product_reference" ],
    "ON" : { "commerce_product_calculate_sell_price" : [] },
    "DO" : [
      { "rooms_booking_manager_price_apply" : { "commerce_line_item" : [ "commerce-line-item" ] } }
    ]
  }}';

  $rules['rooms'] = rules_import($rule);

  return $rules;
}

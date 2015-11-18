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

  $rule = '{ "rooms_booking_manager_deposit" : {
    "LABEL" : "Rooms deposit payment",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : "0",
    "WEIGHT" : "10",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "commerce_line_item", "commerce_product_reference" ],
    "ON" : { "commerce_product_calculate_sell_price" : [] },
    "IF" : [
      { "entity_is_of_bundle" : {
        "entity" : [ "commerce-line-item" ],
          "type" : "commerce_line_item",
          "bundle" : { "value" : { "rooms_booking" : "rooms_booking" } }
        }
      },
      { "data_is" : {
        "data" : [ "commerce-line-item:commerce-product:type" ],
          "value" : "rooms_product"
        }
      }
    ],
    "DO" : []
  }}';

  $rules['rooms_booking_manager_deposit'] = rules_import($rule);

  $rule = '{ "rules_rooms_booking_manager_deposit_checkout" : {
    "LABEL" : "Adjust line item price following checkout with deposit",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : "0",
    "WEIGHT" : "10",
    "OWNER" : "rules",
    "REQUIRES" : [
      "commerce_order",
      "rooms_booking_manager",
      "rules",
      "commerce_checkout"
    ],
    "ON" : { "commerce_checkout_complete" : [] },
    "IF" : [
      { "commerce_order_contains_product" : {
          "commerce_order" : [ "commerce_order" ],
          "product_id" : "ROOMS-BASIC-BOOKING",
          "operator" : "\u003E=",
          "value" : "1"
        }
      }
    ],
    "DO" : [
      { "LOOP" : {
          "USING" : { "list" : [ "commerce-order:commerce-line-items" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "rooms_booking_manager_booked_price" : { "commerce_line_item" : [ "list-item" ] } },
            { "entity_save" : { "data" : [ "list-item" ], "immediate" : "1" } }
          ]
        }
      },
      { "entity_save" : { "data" : [ "commerce-order" ], "immediate" : "1" } }
    ]
  }}';

  $rules['rules_rooms_booking_manager_deposit_checkout'] = rules_import($rule);

  return $rules;
}

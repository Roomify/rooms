<?php

/**
 * @file
 * Default rule configurations for Rooms Package.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rooms_package_default_rules_configuration() {
  $rules = array();

  $component_export = '{ "rules_rooms_package_check_package_availability" : {
    "LABEL" : "Rooms package: Check package availability",
    "PLUGIN" : "and",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "rooms_package" ],
    "USES VARIABLES" : { "product" : { "label" : "Product", "type" : "commerce_product" } },
    "AND" : [
      { "entity_is_of_bundle" : {
          "entity" : [ "product" ],
          "type" : "commerce_product",
          "bundle" : { "value" : { "rooms_package" : "rooms_package" } }
        }
      },
      { "NOT rooms_package_has_stock" : { "commerce_product" : [ "product" ] } }
    ]
  }}';

  $rules['rules_rooms_package_check_package_availability'] = rules_import($component_export);

  $rule_export = '{ "rules_rooms_package_disable_add_to_cart" : {
    "LABEL" : "Rooms Package: Disable add to cart",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "commerce_stock" ],
    "ON" : { "commerce_stock_check_add_to_cart_form_state" : [] },
    "IF" : [
      { "component_rules_rooms_package_check_package_availability" : { "product" : [ "commerce-product" ] } }
    ],
    "DO" : [
      { "commerce_stock_set_add_to_cart_form_state" : { "disabled" : 1, "text" : "Out of stock", "class_name" : "out-of-stock" } }
    ]
  }}';

  $rules['rules_rooms_package_disable_add_to_cart'] = rules_import($rule_export);

  $rule_export = '{ "rules_rooms_package_validate_add_to_cart" : {
    "LABEL" : "Rooms package:Validate add to cart",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "commerce_stock" ],
    "ON" : { "commerce_stock_add_to_cart_check_product" : [] },
    "IF" : [
      { "component_rules_rooms_package_check_package_availability" : { "product" : [ "commerce-product" ] } }
    ],
    "DO" : [
      { "commerce_stock_add_to_cart_set_state" : {
          "stock_action" : "1",
          "message" : "The package [commerce-product:title] is not available now.",
          "approved_quantity" : "0"
        }
      }
    ]
  }}';

  $rules['rules_rooms_package_validate_add_to_cart'] = rules_import($rule_export);

  $rule_export = '{ "rules_rooms_package_validate_checkout" : {
    "LABEL" : "Rooms package:Validate checkout",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "commerce_stock" ],
    "ON" : { "commerce_stock_check_product_checkout" : [] },
    "IF" : [
      { "component_rules_rooms_package_check_package_availability" : { "product" : [ "commerce-product" ] } }
    ],
    "DO" : [
      { "commerce_stock_checkout_state" : {
          "stock_action" : "1",
          "message" : "The package [commerce-product:title] is not available now.",
          "approved_quantity" : "0"
        }
      }
    ]
  }}';

  $rules['rules_rooms_package_validate_checkout'] = rules_import($rule_export);

  $rule_export = '{ "rules_rooms_package_assign_unit" : {
    "LABEL" : "Rooms package: Assign unit to vacational package",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "Rooms" ],
    "REQUIRES" : [ "rules", "rooms_package", "commerce_cart" ],
    "ON" : { "commerce_cart_product_add" : [] },
    "IF" : [
      { "entity_is_of_bundle" : {
          "entity" : [ "commerce-product" ],
          "type" : "commerce_product",
          "bundle" : { "value" : { "rooms_package" : "rooms_package" } }
        }
      },
      { "entity_is_of_bundle" : {
          "entity" : [ "commerce-line-item" ],
          "type" : "commerce_line_item",
          "bundle" : { "value" : { "rooms_booking" : "rooms_booking" } }
        }
      }
    ],
    "DO" : [
      { "rooms_package_assign_unit_id" : { "commerce_line_item" : [ "commerce_line_item" ] } }
    ]
  }}';

  $rules['rules_rooms_package_assign_unit'] = rules_import($rule_export);

  return $rules;
}

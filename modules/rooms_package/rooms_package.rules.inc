<?php

/**
 * @file
 * Rules integration for Rooms Package.
 */

/**
 * Implements hook_rules_condition_info().
 */
function rooms_package_rules_condition_info() {
  $conditions = array();

  $conditions['rooms_package_has_stock'] = array(
    'label' => t('Rooms package is available'),
    'parameter' => array(
      'commerce_product' => array(
        'type' => 'commerce_product',
        'label' => t('product'),
      ),
    ),
    'group' => t('Rooms Package'),
    'callbacks' => array(
      'execute' => 'rooms_package_check_availability',
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function rooms_package_rules_action_info() {
  $actions['rooms_package_assign_unit_id'] = array(
    'label' => t('Assign bookable unit to package'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('line item'),
      ),
    ),
    'group' => t('Rooms Package'),
    'callbacks' => array(
      'execute' => 'rooms_package_assign_unit_id',
    ),
  );

  return $actions;
}

/**
 * Checks if the selected package is available or not.
 *
 * @param $product
 *   The Rooms package to check availability
 *
 * @return bool
 *   TRUE if it is available, FALSE otherwise.
 */
function rooms_package_check_availability($product) {
  $unit_id = &drupal_static(__FUNCTION__);
  if (isset($unit_id)) {
    return $unit_id;
  }
  $all_units = field_get_items('commerce_product', $product, 'rooms_package_all_units');
  $unit_types = field_get_items('commerce_product', $product, 'rooms_package_unit_types');
  $units = field_get_items('commerce_product', $product, 'rooms_package_units');
  $dates = field_get_items('commerce_product', $product, 'rooms_package_dates');

  $start_date = new DateTime($dates[0]['value']);
  $end_date = new DateTime($dates[0]['value2']);

  $agent = new AvailabilityAgent($start_date, $end_date);
  $available_units = $agent->checkAvailability(TRUE);

  if (is_array($available_units)) {
    if (!empty($all_units[0]['value'])) {
      $unit_id = key(current(current($available_units)));
      return $unit_id;
    }
    elseif (is_array($unit_types)) {
      $types = rooms_unit_get_types();
      foreach ($unit_types as $unit) {
        foreach ($available_units as $unit_type => $price_units) {
          if ($types[$unit_type]->id == $unit['target_id']) {
            $unit_id = key(reset($available_units[$unit_type]));
            return $unit_id;
          }
        }
      }
    }
    elseif (is_array($units)) {
      foreach ($available_units as $types) {
        foreach ($types as $price => $price_units) {
          foreach ($price_units as $unit_id => $unit_definition) {
            foreach ($units as $unit) {
              if ($unit_id == $unit['target_id']) {
                return $unit_id;
              }
            }
          }
        }
      }
    }
  }

  $unit_id = FALSE;
  return $unit_id;
}

/**
 * Assigns a unit to the package when adding it to the cart.
 *
 * @param $line_item
 *   The line item to assign the unit.
 */
function rooms_package_assign_unit_id($line_item) {
  $product_id = current(field_get_items('commerce_line_item', $line_item, 'commerce_product'));
  $product = commerce_product_load($product_id['product_id']);
  $dates = field_get_items('commerce_product', $product, 'rooms_package_dates');
  $start_date = new DateTime($dates[0]['value']);
  $end_date = new DateTime($dates[0]['value2']);
  $unit_id = rooms_package_check_availability($product);
  $unit = rooms_unit_load($unit_id);
  $line_item->rooms_booking_dates['und'] = array(
    array(
      'value' => $start_date->format('Y-m-d'),
      'value2' => $end_date->format('Y-m-d'),
    ),
  );
  $line_item->rooms_booked_unit_id[LANGUAGE_NONE][0]['value'] = $unit_id;
  $line_item->label = t('Booking for package @package start date: @start_date end date: @end_date for unit @unit', array(
    '@package' => $product->title,
    '@start_date' => $start_date->format('Y-m-d'),
    '@end_date' => $end_date->format('Y-m-d'),
    '@unit' => $unit->name,
  ));
  commerce_line_item_save($line_item);
}

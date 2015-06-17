<?php
/**
 * @file
 * Variable API module. Definition for Rooms Booking variables
 */

/**
 * Implements hook_variable_info().
 */
function rooms_booking_variable_info($options) {
  $variables['rooms_booking_price'] = array(
    'type' => 'string',
    'title' => t('Price'),
    'default' => t('Price'),
    'description' => t('Change text : Price'),
    'localize' => TRUE,
    'group' => 'rooms_booking',
  );
  $variables['rooms_booking_subtotal'] = array(
    'type' => 'string',
    'title' => t('Subtotal'),
    'default' => t('Subtotal'),
    'description' => t('Change text : Subtotal'),
    'localize' => TRUE,
    'group' => 'rooms_booking',
  );
  $variables['rooms_booking_age_of_child'] = array(
    'type' => 'string',
    'title' => t('Age of Child'),
    'default' => t('Age of Child'),
    'description' => t('Change text : Age of Child'),
    'localize' => TRUE,
    'group' => 'rooms_booking',
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */

function rooms_booking_variable_group_info() {
  $groups['rooms_booking'] = array(
    'title' => t('Rooms Booking'),
    'description' => t('Rooms Booking variables.'),
    'access' => 'administer site configuration',
  );
  return $groups;
}

<?php
/**
 * @file
 * Variable API module. Definition for some xample variables
 */

/**
 * Implements hook_variable_info().
 */
function rooms_booking_manager_variable_info($options) {
  // Simple text
  $variables['review_your_reservation'] = array(
    'type' => 'string',
    'title' => t('Review Your Reservation'),
    'default' => 'Review Your Reservation',
    'description' => t('Change text : Review Your Reservation'),
    'group' => 'rooms_booking_manager',
  );

  $variables['create_your_booking'] = array(
    'type' => 'string',
    'title' => t('Create Your Booking'),
    'default' => 'Create your booking',
    'description' => t('Change text : Create your booking'),
    'group' => 'rooms_booking_manager',
  );
  
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */

function rooms_booking_manager_variable_group_info() {
  $groups['rooms_booking_manager'] = array(
    'title' => t('Rooms Booking Manager'),
    'description' => t('Variable examples of different types.'),
    'access' => 'administer site configuration',
    'path' => array('admin/rooms/config/terminology'),
  );
  return $groups;
}


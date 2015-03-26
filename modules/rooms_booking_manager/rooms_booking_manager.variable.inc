<?php
/**
 * @file
 * Variable API module. Definition for some xample variables
 */

/**
 * Implements hook_variable_info().
 */
function rooms_booking_manager_variable_info($options) {
  $variables['rooms_booking_manager_arrival_date'] = array(
    'type' => 'string',
    'title' => t('Arrival Date (text in "Current search legend")'),
    'default' => 'Arrival Date',
    'description' => t('Change text : Arrival Date (text in "Current search legend")'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );
  
  $variables['rooms_booking_manager_departure_date'] = array(
    'type' => 'string',
    'title' => t('Departure Date (text in "Current search legend")'),
    'default' => 'Departure Date',
    'description' => t('Change text : Departure Date (text in "Current search legend")'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_review_your_reservation'] = array(
    'type' => 'string',
    'title' => t('Review your reservation'),
    'default' => 'Review your reservation',
    'description' => t('Change text : Review your reservation'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_create_your_booking'] = array(
    'type' => 'string',
    'title' => t('Create your booking'),
    'default' => 'Create your booking',
    'description' => t('Change text : Create your booking'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_units'] = array(
    'type' => 'string',
    'title' => t('Units'),
    'default' => 'Units',
    'description' => t('Change text : Units'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_unit_type'] = array(
    'type' => 'string',
    'title' => t('Unit type'),
    'default' => 'Unit type',
    'description' => t('Change text : Unit type'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_group_size'] = array(
    'type' => 'string',
    'title' => t('Group size'),
    'default' => 'Group size',
    'description' => t('Change text : Group size'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_children'] = array(
    'type' => 'string',
    'title' => t('Children'),
    'default' => 'Children',
    'description' => t('Change text : Children'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_select_type'] = array(
    'type' => 'string',
    'title' => t('Select Type'),
    'default' => 'Select Type',
    'description' => t('Change text : Select Type'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_unit_available_from'] = array(
    'type' => 'string',
    'title' => t('Unit available from'),
    'default' => 'unit available from',
    'description' => t('Change text : unit available from'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_units_chosen'] = array(
    'type' => 'string',
    'title' => t('Units chosen'),
    'default' => 'Units Chosen',
    'description' => t('Change text : Units chosen'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_warning_no_units_available'] = array(
    'type' => 'text',
    'title' => t('Warning Message : No units Available'),
    'default' => 'Unfortunately no units are available - try different dates if possible.',
    'description' => t('Change text : Unfortunately no units are available - try different dates if possible.'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_select_your_stay'] = array(
    'type' => 'string',
    'title' => t('Select your stay'),
    'default' => 'Select your stay',
    'description' => t('Change text : Select your stay'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_your_current_search'] = array(
    'type' => 'string',
    'title' => t('Your current search'),
    'default' => 'Your current search',
    'description' => t('Change text : Your current search'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_nights'] = array(
    'type' => 'string',
    'title' => t('Nights'),
    'default' => 'Nights',
    'description' => t('Change text : Nights'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );
 
  $variables['rooms_booking_manager_error_select_unit'] = array(
    'type' => 'text',
    'title' => t('Error Message : Select a Unit'),
    'default' => 'Please select a unit in order to continue with booking.',
    'description' => t('Change text : Please select a unit in order to continue with booking.'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_button_search_for_availability'] = array(
    'type' => 'string',
    'title' => t('Button : Search for Availability'),
    'default' => 'Search for Availability',
    'description' => t('Change button : Search for Availability'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_button_change_search'] = array(
    'type' => 'string',
    'title' => t('Button : Change search'),
    'default' => 'Change search',
    'description' => t('Change button : Change search'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_button_place_booking'] = array(
    'type' => 'string',
    'title' => t('Button : Place Booking'),
    'default' => 'Place Booking',
    'description' => t('Change button : Place Booking'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_button_checkout'] = array(
    'type' => 'string',
    'title' => t('Button : Checkout'),
    'default' => 'Checkout',
    'description' => t('Change button : Checkout'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_button_remove'] = array(
    'type' => 'string',
    'title' => t('Button : Remove'),
    'default' => 'Remove',
    'description' => t('Change button : Remove'),
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
  );

  $variables['rooms_booking_manager_enquiry_form_confirmation'] = array(
    'type' => 'text',
    'title' => t('Enquiry form confirmation', array(), $options),
    'description' => t('Enquiry form confirmation message', array(), $options),
    'default' => 'Thank you - a reservation has been created in our system for this unit and your enquiry has been sent successfully.',
    'localize' => TRUE,
    'group' => 'rooms_booking_manager',
    'required' => TRUE,
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
  );
  return $groups;
}


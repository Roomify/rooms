<?php

function rooms_booking_manager_search_availability() {

  // Add css to align date boxes
  drupal_add_css(drupal_get_path('module', 'rooms_booking_manager') . '/css/booking_search.css');
 
  $output = drupal_get_form('rooms_booking_availability_search_form');
  return $output;
}


function rooms_booking_availability_search_form($form, $form_state) {

  $form['rooms_date_range'] = rooms_date_range_fields();

  $form['conditions']['rooms'] = array(
    '#type' => 'select',
    '#title' => t('Rooms'),
    '#options' => array(
      '1' => '1',
      '2' => '2',
      '3' => '3',
      '4' => '4',
      '5' => '5',
      '6' => '6',
    ),
    '#default_value' =>  '1',
  );

  $form['conditions']['group_size_adults'] = array(
    '#type' => 'select',
    '#title' => t('Adults'),
    '#options' => array(
      '1' => '1',
      '2' => '2',
      '3' => '3',
      '4' => '4',
      '5' => '5',
      '6' => '6',
      '7' => '7',
      '8' => '8',
      '9' => '9',
      '10' => '10',
    ),
    '#default_value' =>  '2',
  );

  if (in_array(ROOMS_CHILDREN_FEATURES, array_filter(variable_get('rooms_experimental_features', array(''))))) {
    $form['conditions']['group_size_children'] = array(
      '#type' => 'select',
      '#title' => t('Children'),
      '#options' => array(
        '0' => '0',
        '1' => '1',
        '2' => '2',
        '3' => '3',
        '4' => '4',
        '5' => '5',
        '6' => '6',
        '7' => '7',
        '8' => '8',
        '9' => '9',
        '10' => '10',
      ),
      '#default_value' =>  '0',
      '#suffix' => '<div style="clear:both"></div>',
    );
  }
  else {
    $form['conditions']['group_size_adults']['#suffix'] = '<div style="clear:both"></div>';
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search for Availabiliy'),
    '#submit' => $submit + array('rooms_booking_availability_search_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_booking_availability_search_form_validate';

  if (isset($_GET['group_size'])) {
    $group_size = check_plain($_GET['group_size']);
    if ($group_size != '') $form['conditions']['group_size_adults']['#default_value'] = $group_size;
  }
  if (isset($_GET['group_size_children'])) {
    $group_size_children = check_plain($_GET['group_size_children']);
    if ($group_size_children != '') $form['conditions']['group_size_children']['#default_value'] = $group_size_children;
  }
  if (isset($_GET['b_units'])) {
    $b_units = check_plain($_GET['b_units']);
    if ($b_units != '') $form['conditions']['rooms']['#default_value'] = $b_units;
  }

  if (isset($_GET['start_date'])) {
    $start_date = check_plain($_GET['start_date']);
    if ($start_date != '') $form['rooms_date_range']['rooms_start_date']['#default_value'] = $start_date;
  }
  if (isset($_GET['end_date'])) {
    $end_date = check_plain($_GET['end_date']);
    if ($end_date != '') $form['rooms_date_range']['rooms_end_date']['#default_value'] = $end_date;
  }

  return $form;

}


function rooms_booking_availability_search_form_validate(&$form, &$form_state) {

  $date1 = $form_state['values']['rooms_start_date'];
  $date2 = $form_state['values']['rooms_end_date'];

  // First check that the dates are set as we would expect them
  if ($date1['date'] == '' || $date2['date'] == '') {
    form_set_error('rooms_date_range', t('Please choose dates.'));
    return TRUE;
  }

  // Dates should be coming through with some value so lets try and load them
  $date1 = start_date_load($form_state['values']['rooms_start_date']);
  $date2 = end_date_load($form_state['values']['rooms_end_date']);
  if (!is_object($date1) || !is_object($date2)){
    form_set_error('rooms_date_range', t('Please choose dates.'));
    return TRUE;
  }

  $now = new DateTime();

  // Ensure start date is after today
  $diff1 = $now->diff($date1);
  if ($diff1->invert) {
    form_set_error('rooms_date_range', t('Start date must be after today'));
  }

  // Ensure end date is after start date
  $diff2 = $date1->diff($date2);

  // If date1 > date2
  if ($diff2->invert) {
    form_set_error('rooms_date_range', t('End date must be after start date'));
  }

  // Ensure end date and start date are not equal
  if ($diff2->d == 0 && $diff2->m == 0) {
    form_set_error('rooms_date_range', t('Start and end date cannot be on the same day'));
  }
}


function rooms_booking_availability_search_form_submit(&$form, &$form_state) {

  $start_date = new DateTime($form_state['values']['rooms_start_date']);
  $end_date = new DateTime($form_state['values']['rooms_end_date']);

  $group_size_adults = $form_state['values']['group_size_adults'];

  $group_size_children = 0;
  if (isset($form_state['values']['group_size_children'])) {
    $group_size_children = $form_state['values']['group_size_children'];
  }

  $units = $form_state['values']['rooms'];

  drupal_goto('booking/' . $start_date->format('Y-m-d') . '/' . $end_date->format('Y-m-d') . '/' . $group_size_adults . '/' . $group_size_children . '/' . $units);
}

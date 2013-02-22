<?php

function rooms_booking_manager_search_availability() {

  // Add css to align date boxes
  drupal_add_css(drupal_get_path('module', 'rooms_booking_manager') . '/css/booking_search.css');
 
  $output = drupal_get_form('rooms_booking_availability_search_form');
  return $output;
}


function rooms_booking_availability_search_form($form, $form_state) {

  $rooms_number = 1;
 
  if (isset($form_state['values']['rooms'])) {
    $rooms_number = $form_state['values']['rooms'];
  }

  if (isset($_GET['rooms_b_units'])) {
    $b_units = check_plain($_GET['rooms_b_units']);
    if ($b_units != '') $rooms_number = $b_units;

    $booking_parameters = rooms_booking_manager_retrieve_booking_parameters($b_units, $_GET);
  }


  $form['rooms_date_range'] = rooms_date_range_fields();
  
  if (isset($_GET['rooms_start_date'])) {
    $start_date = check_plain($_GET['rooms_start_date']);
    if ($start_date != '') $form['rooms_date_range']['rooms_start_date']['#default_value'] = $start_date;
  }
  if (isset($_GET['rooms_end_date'])) {
    $end_date = check_plain($_GET['rooms_end_date']);
    if ($end_date != '') $form['rooms_date_range']['rooms_end_date']['#default_value'] = $end_date;
  }
  
  
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
    '#default_value' =>  $rooms_number,
    '#ajax' => array(
      'callback' => 'rooms_booking_availability_search_form_callback',
      'wrapper' => 'rooms-wrapper',
    ),
  );

  $form['rooms_fieldset'] = array(
    '#prefix' => '<div id="rooms-wrapper">',
    '#suffix' => '</div>',
  );

  if (variable_get('rooms_presentation_style', ROOMS_PER_TYPE) == ROOMS_PER_TYPE) {
    $form['rooms_fieldset']['table_header'] = array(
      '#prefix' => '<table><thead>',
      '#markup' => '<th></th><th>' . t('Group size') . '</th>',
      '#suffix' => '</thead>',
    );

    if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN) {
      $form['rooms_fieldset']['table_header']['#markup'] .= '<th>' . t('Children') . '</th>';
    }
   
    $form['rooms_fieldset']['table_body'] = array(
      '#markup' => '<tbody>',
    );
   
    for ($i = 1; $i <= $rooms_number; $i++) {
      $form['rooms_fieldset']['rooms_label:' . $i] = array(
        '#prefix' => '<tr><td>',
        '#markup' => ($rooms_number > 1) ? t('Room') . ' ' . $i . ':' : '',
        '#suffix' => '</td>',
      );
   
      $form['rooms_fieldset']['group_size_adults:' . $i] = array(
        '#type' => 'select',
        '#options' => array(
          '1' => '1',
          '2' => '2',
          '3' => '3',
          '4' => '4',
          '5' => '5',
          '6' => '6',
          '7' => '7',
          '8' => '8',
        ),
        '#default_value' => isset($booking_parameters[$i]['adults']) ? $booking_parameters[$i]['adults'] : '2',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
      );
   
      if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN_NO) {
        $form['rooms_fieldset']['group_size_children:' . $i] = array(
        '#type' => 'hidden',
        '#value' => 0,
        );
      } else {
        $form['rooms_fieldset']['group_size_children:' . $i] = array(
          '#type' => 'select',
          '#options' => array(
            '0' => '0',
            '1' => '1',
            '2' => '2',
            '3' => '3',
          ),
          '#default_value' =>  isset($booking_parameters[$i]['children']) ? $booking_parameters[$i]['children'] : '0',
          '#prefix' => '<td>',
          '#suffix' => '</td>',
        );
      }
   
      $form['rooms_fieldset']['table_row:' . $i] = array(
        '#markup' => '</tr>',
      );
    }
    $form['rooms_fieldset']['table_end'] = array(
      '#markup' => '</tbody></table>',
    );
  }
  elseif (variable_get('rooms_presentation_style', ROOMS_PER_TYPE) == ROOMS_INDIVIDUAL) {
    $form['rooms_fieldset']['group_size_adults:1'] = array(
      '#type' => 'select',
      '#title' => t('Group size'),
      '#options' => array(
        '1' => '1',
        '2' => '2',
        '3' => '3',
        '4' => '4',
        '5' => '5',
        '6' => '6',
        '7' => '7',
        '8' => '8',
      ),
      '#default_value' => isset($booking_parameters[1]['adults']) ? $booking_parameters[1]['adults'] : 2,
    );

    if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN_NO) {
      $form['rooms_fieldset']['group_size_children:1'] = array(
        '#type' => 'hidden',
        '#value' => 0,
      );
    } else {
      $form['rooms_fieldset']['group_size_children:1'] = array(
        '#type' => 'select',
        '#title' => t('Children'),
        '#options' => array(
          '0' => '0',
          '1' => '1',
          '2' => '2',
          '3' => '3',
        ),
        '#default_value' =>  isset($booking_parameters[1]['children']) ? $booking_parameters[1]['children'] : 0,
      );
    }
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

  return $form;

}


function rooms_booking_availability_search_form_callback(&$form, $form_state) {
  return $form['rooms_fieldset'];
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

  // Ensure start date is appropriate based on the rooms_booking_start_date variable
  $now = new DateTime();
  $diff1 = $now->diff($date1);
  $rooms_booking_start_date = variable_get('rooms_booking_start_date', 1);

  // Check the difference in number of days
  if ($diff1->days < $rooms_booking_start_date) {
    form_set_error('rooms_date_range', t('Bookings need to be at least ' . $rooms_booking_start_date . ' day(s) in the future'));
  }
  
  // Make sure that 
  if ($diff1->invert) {
    form_set_error('rooms_date_range', t('Start date must be current or in the future'));
  }

  // Ensure end date is after start date
  $diff2 = $date1->diff($date2);

  // If date1 > date2
  if ($diff2->invert) {
    form_set_error('rooms_date_range', t('End date must be after start date'));
  }

  // Ensure end date and start date are not equal
  if ($diff2->days == 0) {
    form_set_error('rooms_date_range', t('Start and end date cannot be on the same day'));
  }
}


function rooms_booking_availability_search_form_submit(&$form, &$form_state) {

  $start_date = new DateTime($form_state['values']['rooms_start_date']);
  $end_date = new DateTime($form_state['values']['rooms_end_date']);

  $units = $form_state['values']['rooms'];
 
  $rooms_parameters = array();
  
  // Collect the values for booking parameters
  foreach ($form_state['values'] as $key => $value) {
    $keys = explode(':', $key);

    if (count($keys) == 2) {
      if ($keys[0] == 'group_size_adults') $rooms_parameters['rooms_group_size' . $keys[1]] = $value;
      if ($keys[0] == 'group_size_children') $rooms_parameters['rooms_children' . $keys[1]] = $value;
    }
  }

  drupal_goto('booking/' . $start_date->format('Y-m-d') . '/' . $end_date->format('Y-m-d') . '/' . $units, array('query' => $rooms_parameters));
}

<?php

/**
 * @file
 * Contains the rooms_booking_availability_search_form builder.
 */

/**
 * Form callback for rooms_booking_availability_search_form_builder.
 */
function rooms_booking_availability_search_form_builder($form, &$form_state) {
  // Use form_load_include() instead of module_load_include() as it ensures that
  // the include file is loaded whenever the form is processed. In contrast to
  // using module_load_include() directly, form_load_include() makes sure the
  // include file is correctly loaded also if the form is cached.
  form_load_include($form_state, 'inc', 'rooms_booking_manager', 'rooms_booking_manager.availability_search');

  $form['#attributes']['class'][] = 'rooms-management-form rooms-availability-search';
  $form['#attached']['css'] = array(drupal_get_path('module', 'rooms_booking_manager') . '/css/rooms_booking_search.css');

  $rooms_number = 1;

  if (isset($form_state['values']['rooms'])) {
    $rooms_number = $form_state['values']['rooms'];
  }

  if (isset($_GET['rooms_b_units'])) {
    $b_units = check_plain($_GET['rooms_b_units']);
    if ($b_units != '') {
      $rooms_number = $b_units;
    }
    $booking_parameters = rooms_booking_manager_retrieve_booking_parameters($b_units, $_GET);
  }

  $form['rooms_date_range'] = rooms_date_range_fields();

  if (isset($_GET['rooms_start_date']) && isset($_GET['rooms_end_date'])) {
    $start_date = check_plain($_GET['rooms_start_date']);
    $end_date = check_plain($_GET['rooms_end_date']);
    if ($start_date != '' && $end_date != '') {
      $form['rooms_date_range']['rooms_start_date']['#default_value'] = $start_date;
      $form['rooms_date_range']['rooms_end_date']['#default_value'] = $end_date;
    }
  }

  $form['conditions']['rooms'] = array(
    '#type' => 'select',
    '#title' => variable_get_value('rooms_booking_manager_units'),
    '#options' => rooms_assoc_range(1, variable_get('rooms_booking_manager_search_form_units', 6)),
    '#default_value' => $rooms_number,
  );

  // Create an AJAX wrapper id depending on the form caller to avoid duplicates.
  $ajax_wrapper_id = 'rooms-group-' . substr($form_state['build_info']['form_id'], strlen('rooms_booking_availability_search_form_'));
  $form['rooms_fieldset'] = array(
    '#prefix' => '<div class="form-wrapper rooms-group" id="' . $ajax_wrapper_id . '">',
    '#suffix' => '</div>',
  );

  if (variable_get('rooms_presentation_style', ROOMS_PER_TYPE) == ROOMS_PER_TYPE) {
    $form['conditions']['rooms']['#ajax'] = array(
      'callback' => 'rooms_booking_availability_search_form_callback',
      'wrapper' => $ajax_wrapper_id,
    );

    $classes = 'rooms-group-size ';
    $classes .= ($rooms_number > 1) ? 'rooms-group-size-multiple-units' : 'rooms-group-size-one-unit';
    $form['rooms_fieldset']['table_header'] = array(
      '#prefix' => '<table class="' . $classes . '"><thead>',
      '#markup' => '<th class="rooms-group-size__people" colspan="2">' . variable_get_value('rooms_booking_manager_group_size') . '</th>',
      '#suffix' => '</thead>',
    );

    if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN) {
      $form['rooms_fieldset']['table_header']['#markup'] .= '<th class="rooms-group-size__children">' . variable_get_value('rooms_booking_manager_children') . '</th>';
    }

    $form['rooms_fieldset']['table_body'] = array(
      '#markup' => '<tbody>',
    );

    for ($i = 1; $i <= $rooms_number; $i++) {
      $form['rooms_fieldset']['rooms_label:' . $i] = array(
        '#prefix' => '<tr><th class="rooms-group-size__unit">',
        '#markup' => ($rooms_number > 1) ? '<label>' . t('Unit') . ' ' . $i . ':&nbsp;</label>' : '',
        '#suffix' => '</th>',
      );

      $form['rooms_fieldset']['group_size_adults:' . $i] = array(
        '#type' => 'select',
        '#options' => rooms_assoc_range(1, variable_get('rooms_booking_manager_search_form_max_group_size', 8)),
        '#default_value' => isset($booking_parameters[$i]['adults']) ? $booking_parameters[$i]['adults'] : '2',
        '#prefix' => '<td class="rooms-group-size__people">',
        '#suffix' => '</td>',
      );

      if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN) {
        $form['rooms_fieldset']['group_size_children:' . $i] = array(
          '#type' => 'select',
          '#options' => rooms_assoc_range(0, 3),
          '#default_value' => isset($booking_parameters[$i]['children']) ? $booking_parameters[$i]['children'] : '0',
          '#prefix' => '<td class="rooms-group-size__children">',
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
      '#title' => variable_get_value('rooms_booking_manager_group_size'),
      '#options' => rooms_assoc_range(1, variable_get('rooms_booking_manager_search_form_max_group_size', 8)),
      '#default_value' => isset($booking_parameters[1]['adults']) ? $booking_parameters[1]['adults'] : 2,
    );

    if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN) {
      $form['rooms_fieldset']['group_size_children:1'] = array(
        '#type' => 'select',
        '#title' => t('Children'),
        '#options' => rooms_assoc_range(0, 3),
        '#default_value' => isset($booking_parameters[1]['children']) ? $booking_parameters[1]['children'] : 0,
      );
    }
  }

  if (variable_get('rooms_booking_manager_type_selector', ROOMS_DISPLAY_TYPE_SELECTOR_NO) == ROOMS_DISPLAY_TYPE_SELECTOR) {
    $searchable_unit_types = variable_get('rooms_unit_type_selector', array());
    if (!empty($searchable_unit_types)) {

      $form['unit_type'] = array(
        '#title' => variable_get_value('rooms_booking_manager_select_type'),
        '#type' => 'select',
        '#empty_option' => t('Select All'),
        '#options' => _rooms_booking_manager_get_unit_type_labels($searchable_unit_types),
        '#default_value' => 0,
      );

      if (isset($_GET['type'])) {
        $type = check_plain($_GET['type']);
        if ($type != '' && rooms_unit_type_load($type) !== FALSE) {
          $form['unit_type']['#default_value'] = $type;
        }
      }
    }
  }

  // Include form elements provided by other availability search plugins.
  ctools_include('plugins');
  $filters = ctools_get_plugins('rooms_booking', 'availabilityagent_filter');
  foreach ($filters as $filter) {
    $class = ctools_plugin_get_class($filter, 'handler');
    $class::availabilitySearchForm($form, $form_state);
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
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
    '#value' => variable_get_value('rooms_booking_manager_button_search_for_availability'),
    '#submit' => $submit + array('rooms_booking_availability_search_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_booking_availability_search_form_validate';
  $form['#validate'][] = 'rooms_form_start_end_dates_validate';

  return $form;
}

function rooms_booking_availability_search_form_callback(&$form, $form_state) {
  return $form['rooms_fieldset'];
}

/**
 * Validate callback form rooms_booking_availability_search_form form.
 */
function rooms_booking_availability_search_form_validate(&$form, &$form_state) {
  list($start_date, $end_date) = rooms_form_input_get_start_end_dates($form_state);
  if (empty($start_date)) {
    return;
  }
  // This indicate to the rooms_form_start_end_dates_validate handler to
  // enable today greater validation.
  $form_state['today_greater_validation'] = TRUE;
  // Ensure start date is good based on the rooms_booking_start_date variable.
  $now = new DateTime();
  $diff1 = $now->setTime(0, 0, 0)->diff($start_date);
  $rooms_booking_start_date = variable_get('rooms_booking_start_date', 1);
  // Check the difference in number of days.
  if ($diff1->days < $rooms_booking_start_date) {
    form_set_error('rooms_date_range', t('Bookings must be made at least @count_days in advance.', array('@count_days' => format_plural($rooms_booking_start_date, '1 day', '@count days'))));
  }

  // Validate form elements provided by other availability search plugins.
  ctools_include('plugins');
  $filters = ctools_get_plugins('rooms_booking', 'availabilityagent_filter');
  foreach ($filters as $filter) {
    $class = ctools_plugin_get_class($filter, 'handler');
    $class::availabilitySearchFormValidate($form, $form_state);
  }
}

/**
 * Submit callback for rooms_booking_availability_search_form form.
 */
function rooms_booking_availability_search_form_submit(&$form, &$form_state) {
  list($start_date, $end_date) = rooms_form_input_get_start_end_dates($form_state);

  $units = $form_state['values']['rooms'];

  $rooms_query = array();

  if (isset($form_state['values']['unit_type'])) {
    if (!empty($form_state['values']['unit_type'])) {
      $rooms_query['type'] = $form_state['values']['unit_type'];
    }
    else {
      $rooms_query['type'] = 'all';
    }
  }

  // Get parameter for AvailabilityAgentSingleUnitFilter.
  if (isset($form_state['values']['rooms_id'])) {
    $rooms_query['rooms_id'] = $form_state['values']['rooms_id'];
  }

  $rooms_query += rooms_booking_manager_get_plugin_parameters($form_state);

  // Collect the values for booking parameters.
  foreach ($form_state['values'] as $key => $value) {
    $keys = explode(':', $key);

    if (count($keys) == 2) {
      if ($keys[0] == 'group_size_adults') {
        $rooms_query['rooms_group_size' . $keys[1]] = $value;
      }
      if ($keys[0] == 'group_size_children') {
        $rooms_query['rooms_children' . $keys[1]] = $value;
      }
    }
  }

  $form_state['redirect'] = array(
    'booking/' . $start_date->format('Y-m-d') . '/' . $end_date->format('Y-m-d') . '/' . $units,
    array(
      'query' => $rooms_query,
    ),
  );
}

/**
 * Returns the actual labels of for the given rooms unit types.
 *
 * @param array $unit_type_ids
 *   A list of machine names to get the labels for.
 *
 * @return array
 *   A list of labels keys by machine name.
 */
function _rooms_booking_manager_get_unit_type_labels($unit_type_ids) {
  // Get all labels
  $unit_types = rooms_unit_get_types();

  $unit_type_labels = array();
  foreach ($unit_type_ids as $label) {
    $unit_type_labels[$label] = $unit_types[$label]->label;
  }

  return $unit_type_labels;
}

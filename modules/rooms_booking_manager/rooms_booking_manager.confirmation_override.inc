<?php

/**
 * @file
 * Override confirmation page logic.
 */

/**
 * Constructs the booking override confirmation page
 *
 * @param DateTime $start_date
 *   Start date for the search.
 * @param DateTime $end_date
 *   End date for the search.
 * @param int $booking_units
 *   In how many units are we to accommodate them.
 *
 * @return string
 *   The themed page string.
 */
function rooms_booking_manager_override_confirmation_page(DateTime $start_date, DateTime $end_date, $booking_units = 1, $single_day_bookings = FALSE) {

  $booking_parameters = array(
    'unit_id' => $_GET['unit_id'],
    'group_size' => $_GET['group_size'],
  );
  if (isset($_GET['options'])) {
    $booking_parameters['options'] = $_GET['options'];
  }

  $type = '';
  if (isset($_GET['type'])) {
    $type = check_plain($_GET['type']);
    if ($type == '' || rooms_unit_type_load($type) === FALSE) {
      $type = '';
    }
  }

  $content['change_search'] = drupal_get_form('rooms_booking_manager_change_search_form', $start_date, $end_date, $booking_parameters, $booking_units, $type);

  $current_path = array(
    'path' => 'booking/' . $start_date->format('Y-m-d') . '/' . $end_date->format('Y-m-d') . '/' . $booking_units,
    'query' => array('rooms_group_size1' => $booking_parameters['group_size']),
  );
  $content['confirm_form'] = drupal_get_form('rooms_booking_manager_override_confirmation_form', $start_date, $end_date, $booking_parameters, t('You have a previous booking request in your cart, continuing will replace it.'), $current_path, '', t('Continue'), t('Go Back To Search'));

  return theme('rooms_booking_override_confirmation_page', $content);
}

/**
 * Provide a confirmation form.
 */
function rooms_booking_manager_override_confirmation_form($form, &$form_state, $start_date, $end_date, $booking_parameters, $question, $path, $description = NULL, $yes = NULL, $no = NULL) {

  $form['#attached']['css'][] = array(
    'data' => '.rooms-current-search__info { float: none; }',
    'type' => 'inline',
  );

  $form['unit_id'] = array(
    '#type' => 'hidden',
    '#value' => $booking_parameters['unit_id'],
  );

  $form['start_date'] = array(
    '#type' => 'hidden',
    '#value' => $start_date->format('Y-m-d'),
  );

  $form['end_date'] = array(
    '#type' => 'hidden',
    '#value' => $end_date->format('Y-m-d'),
  );

  $form['rooms_group_size'] = array(
    '#type' => 'hidden',
    '#value' => $booking_parameters['group_size'],
  );

  if (isset($booking_parameters['options'])) {
    $form['options'] = array(
      '#type' => 'hidden',
      '#value' => serialize($booking_parameters['options']),
    );
  }

  return confirm_form($form, $question, $path, $description, $yes, $no);
}

/**
 * Submit hanlder for the confirmation form.
 */
function rooms_booking_manager_override_confirmation_form_submit($form, &$form_state) {
  module_load_include('inc', 'rooms_booking_manager', 'rooms_booking_manager.commerce');

  global $user;

  $unit_id = $form_state['values']['unit_id'];
  $start_date = $form_state['values']['start_date'];
  $end_date = $form_state['values']['end_date'];
  $group_size = (isset($form_state['values']['persons'])) ? $form_state['values']['persons'] : $form_state['values']['rooms_group_size'];

  // This is very inefficient right now but we need to create date objects
  // reconvert them back to strings to recreate them in the Availability Agent.
  $sd = start_date_load($start_date);
  $ed = end_date_load($end_date);

  // Let us get available rooms again and match the order against actual rooms.
  $agent = new AvailabilityAgent($sd, $ed, $group_size);
  $agent->setValidStates(array_keys(array_filter(variable_get('rooms_valid_availability_states', drupal_map_assoc(array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST))))));

  // Let us make sure our bookable unit is still available.
  $available_units = $agent->checkAvailabilityForUnit($unit_id);

  if (count($available_units) > 0) {
    $unit = array_pop($available_units);

    $price_modifiers = array();
    if (isset($form_state['values']['options'])) {
      $options = unserialize($form_state['values']['options']);

      // Convert Price options in Price modifiers.
      foreach (rooms_unit_get_unit_options($unit['unit']) as $option) {
        $option_name = rooms_options_machine_name($option['name']);
        if (isset($options[$option_name])) {
          if ($options[$option_name] == 1) {
            $quantity = 1;
            if (isset($options[$option_name . ':quantity']) && $option['operation'] != ROOMS_REPLACE) {
              $quantity = $options[$option_name . ':quantity'] + 1;
            }
            $price_modifiers[$option_name] = array(
              '#name' => $option['name'],
              '#type' => ROOMS_DYNAMIC_MODIFIER,
              '#op_type' => $option['operation'],
              '#amount' => $option['value'],
              '#quantity' => $quantity,
            );
          }
        }
      }
    }

    // Check if there are line_items with overlapping dates.
    $line_items = rooms_booking_manager_find_overlapping_line_items($sd, $ed, $unit['unit']->unit_id);
    if (!empty($line_items)) {
      $commerce_order = commerce_cart_order_load($user->uid);

      foreach ($line_items as $line_item_id) {
        commerce_cart_order_product_line_item_delete($commerce_order, $line_item_id);
      }
    }

    // Create line item.
    if (isset($form_state['values']['children'])) {
      $children = $form_state['values']['children'];
      if (isset($form_state['values']['childrens_age'])) {
        $childrens_age = $form_state['values']['childrens_age'];
        $line_item = rooms_create_line_item($unit, $agent, array('adults' => $group_size, 'children' => $children, 'childrens_age' => $childrens_age), $price_modifiers);
      }
      else {
        $line_item = rooms_create_line_item($unit, $agent, array('adults' => $group_size, 'children' => $children), $price_modifiers);
      }
    }
    else {
      $line_item = rooms_create_line_item($unit, $agent, array('adults' => $group_size, 'children' => 0), $price_modifiers);
    }

    // Add line item to cart.
    $line_item = commerce_cart_product_add($user->uid, $line_item, FALSE);

    // Refresh line items price and redirect to bookings page.
    commerce_cart_order_refresh(commerce_cart_order_load($user->uid));
    $form_state['redirect'] = 'bookings';
  }
  else {
    drupal_set_message(t('We apologize for the inconvenience; this unit is no longer available.'));
    $form_state['redirect'] = '<front>';
  }
}

/**
 * Find overlapping line_items from cart using start_date, end_date, unit_id.
 */
function rooms_booking_manager_find_overlapping_line_items($start_date, $end_date, $unit_id) {
  global $user;

  $commerce_order = commerce_cart_order_load($user->uid);

  $line_items = array();

  if ($commerce_order !== FALSE) {
    foreach ($commerce_order->commerce_line_items as $lang => $item) {
      foreach ($item as $item_id) {
        $line_item = commerce_line_item_load($item_id['line_item_id']);

        if ($line_item->type == 'rooms_booking') {

          $line_item_start_date = DateTime::createFromFormat('Y-m-d', $line_item->rooms_booking_dates[LANGUAGE_NONE][0]['value']);
          $line_item_end_date = DateTime::createFromFormat('Y-m-d', $line_item->rooms_booking_dates[LANGUAGE_NONE][0]['value2']);

          if ($line_item_start_date < $end_date && $line_item_end_date > $start_date &&
              $line_item->rooms_booked_unit_id[LANGUAGE_NONE][0]['value'] == $unit_id) {
            $line_items[] = $item_id['line_item_id'];
          }
        }
      }
    }
  }

  return $line_items;
}

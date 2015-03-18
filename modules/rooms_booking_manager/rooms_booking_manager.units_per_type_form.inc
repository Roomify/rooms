<?php

/**
 * @file
 * Helper functions to clear up the units per type form and make the logic
 * easier to follow.
 */

/**
 * Set up hidden fields with information that is required throughout the form.
 *
 * @param $form
 * @param $start_date
 * @param $end_date
 * @param $booking_parameters
 * @param $b_units
 *
 * @return mixed
 */
function _rooms_booking_manager_setup_hidden_fields($form, $start_date, $end_date, $booking_parameters, $b_units) {
  $form['start_date'] = array(
    '#type' => 'hidden',
    '#value' => $start_date,
  );
  $form['end_date'] = array(
    '#type' => 'hidden',
    '#value' => $end_date,
  );

  $form['booking_parameters'] = array(
    '#type' => 'hidden',
    '#value' => serialize($booking_parameters),
  );
  $form['info']['params']['b_units'] = array(
    '#type' => 'hidden',
    '#value' => check_plain($b_units),
  );

  return $form;
}

/**
 * Checks to see if an entity type has a node associated with it and loads the
 * description of that node.
 *
 * @param $form
 * @param $type_obj
 * @param $type
 * @param $price
 *
 * @return mixed
 */
function _rooms_booking_manager_load_description($form, $type_obj, $type, $price) {

  // Check if a description source is loaded and render it if so.
  if (isset($type_obj->data['rooms_description_source'])) {
    if ($type_obj->data['rooms_description_source'] != '') {
      $source_ref = explode(':', $type_obj->data['rooms_description_source']);
      if (isset($source_ref[1])) {
        $node_id = $source_ref[1];

        if (module_exists('translation')) {
          $node_translations = translation_node_get_translations($node_id);
          if (!empty($node_translations)) {
            $node_id = $node_translations[$GLOBALS['language']->language]->nid;
          }
        }

        if ($node = node_load($node_id)) {
          $node_view = node_view($node, 'rooms_list');
          $node_html = render($node_view);

          $form[$type][$price]['description'] = array(
            '#prefix' => '<tr class="rooms-search-result__unit_details_wrapper"><td class="rooms-search-result__room-description">',
            '#markup' => $node_html,
            '#suffix' => '</td><td class="empty"></td><td class="empty"></td></tr>',
          );
        }
      }
    }
  }

  return $form;
}

/**
 * Loads the price information for a given unit type/price level combination.
 *
 * @param $form
 * @param $type_obj
 * @param $type
 * @param $price
 * @param $currency_symbol
 * @param $units_per_price
 * @param $units
 *
 * @return mixed
 */
function _rooms_booking_manager_load_price_info($form, $type_obj, $type, $price, $currency_symbol, $units_per_price, $units) {

  // Calculate the period.
  $nights = $form['end_date']['#value']->diff($form['start_date']['#value'])->days;
  // Element to display price.
  $form[$type][$price]['price'] = array(
    '#prefix' => '<tr class="rooms-search-result__unit_summary_wrapper"><td class="rooms-search-result__base_price">',
    '#markup' => rooms_string(
      format_plural(count($units_per_price), '1 @unit_type '
        . variable_get_value('rooms_booking_manager_unit_available_from')
        . ' @price', '@count @unit_type '
        . variable_get_value('rooms_booking_manager_unit_available_from')
        . ' @price',
        array(
          '@unit_type' => $type_obj->label,
          '@price' => t('@currency_symbol @amount', array( // TODO: replace with a proper currency formatter
              '@currency_symbol' => $currency_symbol,
              '@amount' => $price,
            )
          )
        )
      ),
      $context = array(
        '#component' => 'units_per_type_form',
        '#purpose' => 'display_base_price',
        '#data' => array(
          'unit_type' => $type_obj->label,
          'price' => $price,
          'currency_symbol' => $currency_symbol,
          'amount' => $price,
          'units' => $units_per_price,
          'nights' => $nights,
          'arrival' => $form['start_date']['#value'],
          'departure' => $form['end_date']['#value'],
        ),
      )
    ),
  );

  if (variable_get('rooms_price_calculation', ROOMS_PER_NIGHT) == ROOMS_PER_PERSON) {
    $units_keys = array_keys($units);

    // Calculate the price per person for the booking period.
    $base_price = $units[$units_keys[0]]['price'] / $units[$units_keys[0]]['unit']->max_sleeps;
    $form[$type][$price]['price']['#markup'] = format_plural(count
    ($units), '1 @unit_type unit available starting at @price per
        person', '@count @unit_type units available starting at @price per person', array(
        '@unit_type' => $type_obj->label,
        '@price' => t('@currency_symbol @amount', array( // TODO: replace with a proper currency formatter
          '@currency_symbol' => $currency_symbol,
          '@amount' => $base_price))
      )
    );
  }

  // Add form element to hold the price.
  $form[$type][$price]['price_value'] = array(
    '#type' => 'hidden',
    '#value' => $price,
    '#suffix' => '</td>',
  );

  // Dropdown element to select unit quantity.
  $options = array();
  for ($i = 0; $i <= count($units); $i++) {
    $options[$i] = $i;
  }

  // AJAX wrapper id.
  $id = 'rooms_' . $type . '_' . $price;
  $form[$type][$price]['quantity'] = array(
    '#prefix' => '<td class="rooms-search-result__select-units">',
    '#field_suffix' => variable_get_value('rooms_booking_manager_units_chosen'),
    '#type' => 'select',
    '#options' => $options,
    '#suffix' => '</td>',
    '#ajax' => array(
      'callback' => 'rooms_booking_manager_quantity_change_callback',
      'wrapper' => $type . '_' . $price . '_container',
    ),
    '#attributes' => array(
      'onchange' => "document.getElementById('" . $id . "').style.display = (this.value == 0) ? 'none' : 'table-row';",
    ),
  );

  return $form;
}

/**
 * Add "place booking" button to form.
 * @param $form
 * @param $type
 * @param $price
 *
 * @return mixed
 */
function _rooms_booking_manager_display_book_button($form, $type, $price) {

  // Show button 'Place Booking' only on the first row.
  $form[$type][$price]['submit'] = array(
    '#prefix' => '<td class="rooms-search-result__booking-legend"><div class="booking-legend booking-legend--rooms-search-result">',
    '#type' => 'submit', '#value' => variable_get_value('rooms_booking_manager_button_place_booking'),
    '#submit' => array('book_units_per_type_form_submit'),
    '#suffix' => '</div></td></tr>',
  );

  return $form;
}

/**
 * Checks if there are any options to add to units and loads them if so.
 *
 * @param $form
 * @param $type
 * @param $price
 * @param $c
 * @param $option
 *
 * @return mixed
 */
function _rooms_booking_manager_add_options($form, $type, $price, $c, $option) {
  $option_name = strtolower(str_replace(array(':', ' '), '_', $option['name']));
  $form[$type][$price]['fieldset'][$c][$option_name] = array(
    '#type' => 'checkbox',
    '#title' => t($option['name']),
    '#ajax' => array(
      'callback' => 'rooms_booking_manager_options_change_callback',
      'wrapper' => $type . '_' . $price . '_' . $c . '_price',
    ),
  );

  // Show quantity field selector if an option quantity is set.
  if (is_numeric($option['quantity'])) {
    if (isset($form_state['values'][$type][$price]['fieldset'][$c][$option_name]) &&
      $form_state['values'][$type][$price]['fieldset'][$c][$option_name] == 1 && $option['quantity'] > 1) {

      $form[$type][$price]['fieldset'][$c][$option_name . ':quantity'] = array(
        '#type' => 'select',
        '#title' => t('Quantity'),
        '#options' => range(1, $option['quantity']),
        '#ajax' => array(
          'callback' => 'rooms_booking_manager_options_change_callback',
          'wrapper' => $type . '_' . $price . '_' . $c . '_price',
        ),
        '#prefix' => '<div class="rooms-search-result__select-quantity" id="' . $type . '_' . $price . '_' . $c . '_' . $option_name . '_quantity">',
        '#suffix' => '</div>',
      );
    }
    else {
      $form[$type][$price]['fieldset'][$c][strtolower(str_replace(array(':', ' '), '_', $option['name'])) . ':quantity'] = array(
        '#prefix' => '<div class="rooms-search-result__select-quantity" id="' . $type . '_' . $price . '_' . $c . '_' . $option_name . '_quantity">',
        '#suffix' => '</div>',
      );
    }
  }

  return $form;
}

/**
 * Handles display for per person pricing.
 *
 * @param $form
 * @param $form_state
 * @param $tmp_unit
 * @param $units
 * @param $units_keys
 * @param $c
 * @param $type
 * @param $price
 * @return mixed
 */
function _rooms_booking_manager_handle_per_person_pricing($form, &$form_state, $tmp_unit, $units, $units_keys, $c, $type, $price) {

  // Show Guests and Children selectors if price calculation is set
  // to 'Price per person per night'.
  $form[$type][$price]['fieldset'][$c]['persons'] = array(
    '#type' => 'select',
    '#field_suffix' => t('Guests'),
    '#options' => array_combine(
      range($tmp_unit->min_sleeps, $tmp_unit->max_sleeps),
      range($tmp_unit->min_sleeps, $tmp_unit->max_sleeps)),
    '#default_value' => $tmp_unit->max_sleeps,
    '#ajax' => array(
      'callback' => 'rooms_booking_manager_quantity_change_callback',
      'wrapper' => $type . '_' . $price . '_container',
    ),
    '#title' => t('How many people in this unit (including adults and children)?'),
    '#prefix' => '<div class="rooms-search-result__select-guests">',
    '#suffix' => '</div>',
  );

  $max_children = $tmp_unit->max_children;
  if (isset($form_state['values'][$type][$price]['fieldset'][$c]['persons'])) {
    $persons = $form_state['values'][$type][$price]['fieldset'][$c]['persons'];

    if ($persons < $tmp_unit->max_children) {
      $max_children = $persons;
    }
  }

  if (variable_get('rooms_display_children', ROOMS_DISPLAY_CHILDREN_NO) == ROOMS_DISPLAY_CHILDREN) {
    if ($tmp_unit->max_children > 0) {
      $form[$type][$price]['fieldset'][$c]['children'] = array(
        '#type' => 'select',
        '#title' => t('How many of the guests are children?'),
        '#field_suffix' => t('Children'),
        '#options' => array_combine(
          range($tmp_unit->min_children, $max_children),
          range($tmp_unit->min_children, $max_children)),
        '#default_value' => 0,
        '#ajax' => array(
          'callback' => 'rooms_booking_manager_children_change_callback',
          'wrapper' => $type . '_' . $price . '_' . $c . '_childrensage',
        ),
        '#prefix' => '<div class="rooms-search-result__select-children">',
        '#suffix' => '</div>',
      );
    }

    $form[$type][$price]['fieldset'][$c]['childrens_age'] = array(
      '#prefix' => '<div class="rooms-search-result__select-childrenage" id="' . $type . '_' . $price . '_' . $c . '_childrensage">',
      '#suffix' => '</div>',
    );

    $children_number = 0;
    if (isset($form_state['values'][$type][$price]['fieldset'][$c]['children'])) {
      if ($form_state['values'][$type][$price]['fieldset'][$c]['children'] > 0) {
        $children_number = $form_state['values'][$type][$price]['fieldset'][$c]['children'];
      }
    }
    else {
      $children_number = $tmp_unit->min_children;
    }

    for ($t = 1; $t <= $children_number; $t++) {
      $form[$type][$price]['fieldset'][$c]['childrens_age'][$t] = array(
        '#type' => 'select',
        '#field_prefix' => t('Age of child @num', array('@num' => $t)),
        '#options' => range(0, 18),
        '#ajax' => array(
          'callback' => 'rooms_booking_manager_options_change_callback',
          'wrapper' => $type . '_' . $price . '_' . $c . '_price',
        ),
        '#attributes' => array('class' => array('rooms-search-result__childrens-age')),
      );
    }
  }

  return $form;
}

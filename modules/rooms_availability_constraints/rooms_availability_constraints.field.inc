<?php

/**
 * Implements hook_field_info().
 */
function rooms_availability_constraints_field_info() {
  return array(
    'rooms_availability_constraints_range' => array(
      'label' => t('Availability ranges'),
      'description' => t('Bookable unit availability options.'),
      'settings' => array(),
      'default_widget' => 'rooms_availability_constraints_range',
      'default_formatter' => 'rooms_availability_constraints_range',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function rooms_availability_constraints_field_is_empty($item, $field) {
  return !is_string($item['start_date']) || !is_string($item['start_date']);
}

/**
 * Implements hook_field_widget_info().
 */
function rooms_availability_constraints_field_widget_info() {
  return array(
    'rooms_availability_constraints_range' => array(
      'label' => t('Availability range'),
      'description' => t('Availability range widget'),
      'field types' => array('rooms_availability_constraints_range'),
      'settings' => array(),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function rooms_availability_constraints_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($instance['widget']['type'] == 'rooms_availability_constraints_range') {
    $element += array(
      '#type' => 'rooms_availability_range',
      '#default_value' => isset($items[$delta]) ? $items[$delta] : '',
    );

    if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED) {
      $element['remove'] = array(
        '#type' => 'submit',
        '#value' => t('Remove'),
        '#weight' => 100,
        '#attributes' => array(
          'class' => array('remove-booking-option'),
        ),
        '#attached' => array(
          'js' => array(
            drupal_get_path('module', 'rooms_availability_constraints') . '/js/rooms_availability_constraints_range.js'
          )
        ),
      );
    }
    return $element;
  }
}

/**
 * Implements hook_field_validate().
 */
function rooms_availability_constraints_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (is_array($item) && isset($item['start_date']) && isset($item['end_date'])) {
      if ($item['start_date'] >= $item['end_date']) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'rooms_availability_constraints_end_date',
          'message' => t('End date must be greater than start date'),
        );
      }
      $minimum_set = $maximum_set = FALSE;
      if (!empty($item['minimum_stay'])) {
        if (!((int) $item['minimum_stay'] == $item['minimum_stay'] && (int) $item['minimum_stay'] > 0)) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'rooms_availability_constraints_minimum_stay',
            'message' => t('Minimum stay value must be a positive integer value'),
          );
        }
        else {
          $minimum_set = TRUE;
        }
      }

      if (!empty($item['maximum_stay'])){
        if (!((int) $item['maximum_stay'] == $item['maximum_stay'] && (int) $item['maximum_stay'] > 0)) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'rooms_availability_constraints_maximum_stay',
            'message' => t('Maximum stay value must be a positive integer value'),
          );
        }
        else {
          $maximum_set = TRUE;
        }
      }

      if ($maximum_set && $minimum_set && ($item['minimum_stay'] > $item['maximum_stay'])) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error' => 'rooms_availability_constraints_range',
          'message' => t('Minimum stay can\'t be longer than maximum stay'),
        );
      }
    }
  }
}

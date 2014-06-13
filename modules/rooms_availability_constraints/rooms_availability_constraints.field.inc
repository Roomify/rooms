<?php

/**
 * Implements hook_field_info().
 */
function rooms_availability_constraints_field_info() {
  return array(
    'rooms_availability_constraints_range' => array(
      'label' => t('Rooms availability ranges'),
      'description' => t('Bookable unit availability options.'),
      'settings' => array(),
      'default_widget' => 'rooms_availability_constraints_range',
      'default_formatter' => 'rooms_availability_constraints_range',
      'property_type' => 'rooms_availability_constraints_range',
      'property_callbacks' => array('rooms_availability_constraints_range_property_info_callback'),
    ),
  );
}

/**
 * Property callback for the Entity Metadata framework.
 */
function rooms_availability_constraints_range_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  // Apply the default.
  entity_metadata_field_default_property_callback($info, $entity_type, $field, $instance, $field_type);

  // Finally add in instance specific property info.
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  $property['type'] = ($field['cardinality'] != 1) ? 'list<rooms_availability_constraints_range>' : 'rooms_availability_constraints_range';
  $property['property info'] = rooms_availability_constraints_range_data_property_info('Rooms availability constraints');
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
}

/**
 * Defines info for the properties of the video_embed_field data structure.
 */
function rooms_availability_constraints_range_data_property_info($name = NULL) {
  // Build an array of basic property information for video_embed_field.
  $properties = array(
    'start_date' => array(
      'label' => 'Start date',
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'end_date' => array(
      'label' => 'End date',
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'always' => array(
      'label' => 'Always enabled',
      'type' => 'boolean',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'constraint_type' => array(
      'label' => 'Constraint type',
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'start_day' => array(
      'label' => 'Starting weekday',
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'minumum_stay' => array(
      'label' => 'Minimum stay',
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'maximum_stay' => array(
      'label' => 'Maximum stay',
      'type' => 'integer',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );

  // Add the default values for each of the video_embed_field properties.
  foreach ($properties as &$value) {
    $value += array(
      'description' => !empty($name) ? t('!label of field %name', array('!label' => $value['label'], '%name' => $name)) : '',
    );
  }

  return $properties;
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

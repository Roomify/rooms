<?php

/**
 * @file
 */

/**
 * Create Room fields.
 */
function rooms_create_rooms_fields() {
  $field_bases = array();

  if (field_read_field('field_rooms_default_state') === FALSE) {
    $field_bases['field_rooms_default_state'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_rooms_default_state',
      'indexes' => array(
        'state_id' => array(
          0 => 'state_id',
        ),
      ),
      'locked' => 0,
      'module' => 'bat_event',
      'settings' => array(
        'event_type' => '',
      ),
      'translatable' => 0,
      'type' => 'bat_event_state_reference',
    );
  }

  if (field_read_field('field_rooms_default_price') === FALSE) {
    $field_bases['field_rooms_default_price'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_rooms_default_price',
      'indexes' => array(
        'currency_price' => array(
          0 => 'amount',
          1 => 'currency_code',
        ),
      ),
      'locked' => 0,
      'module' => 'commerce_price',
      'settings' => array(),
      'translatable' => 0,
      'type' => 'commerce_price',
    );
  }

  if (field_read_field('field_rooms_amenities') === FALSE) {
    $field_bases['field_rooms_amenities'] = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_rooms_amenities',
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'amenities',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    );
  }

  if (field_read_field('field_rooms_image') === FALSE) {
    $field_bases['field_rooms_image'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_rooms_image',
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => 0,
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => 0,
      'type' => 'image',
    );
  }

  if (field_read_field('field_rooms_sleeps') === FALSE) {
    $field_bases['field_rooms_sleeps'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_rooms_sleeps',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(),
      'translatable' => 0,
      'type' => 'number_integer',
    );
  }

  foreach ($field_bases as $field) {
    field_create_field($field);
  }

  $field_instances = array();

  if (field_read_instance('bat_type', 'field_room_default_state', 'rooms') === FALSE) {
    $field_instances['bat_type-rooms-field_room_default_state'] = array(
      'bundle' => 'rooms',
      'default_value' => array(
        0 => array(
          'state_id' => 2,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'bat_event',
          'settings' => array(),
          'type' => 'bat_event_state_reference_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'bat_type',
      'field_name' => 'field_rooms_default_state',
      'label' => 'Default State',
      'required' => 0,
      'settings' => array(
        'event_type' => 'availability',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'bat_event',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'bat_event/state_event/autocomplete',
          'size' => 60,
        ),
        'type' => 'bat_event_reference_autocomplete',
        'weight' => 1,
      ),
    );
  }

  if (field_read_instance('bat_type', 'field_rooms_default_price', 'rooms') === FALSE) {
    $field_instances['bat_type-rooms-field_rooms_default_price'] = array(
      'bundle' => 'rooms',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => FALSE,
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'bat_type',
      'field_name' => 'field_rooms_default_price',
      'label' => 'Default Price',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'commerce_price',
        'settings' => array(
          'currency_code' => 'default',
        ),
        'type' => 'commerce_price_simple',
        'weight' => 2,
      ),
    );
  }

  if (field_read_instance('bat_type', 'field_rooms_amenities', 'rooms') === FALSE) {
    $field_instances['bat_type-rooms-field_rooms_amenities'] = array(
      'bundle' => 'rooms',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'bat_type',
      'field_name' => 'field_rooms_amenities',
      'label' => 'Amenities',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60,
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => 4,
      ),
    );
  }

  if (field_read_instance('bat_type', 'field_rooms_image', 'rooms') === FALSE) {
    $field_instances['bat_type-rooms-field_rooms_image'] = array(
      'bundle' => 'rooms',
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 4,
        ),
      ),
      'entity_type' => 'bat_type',
      'field_name' => 'field_rooms_image',
      'label' => 'Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
        'default_image' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => 5,
      ),
    );
  }

  if (field_read_instance('bat_type', 'field_rooms_sleeps', 'rooms') === FALSE) {
    $field_instances['bat_type-rooms-field_rooms_sleeps'] = array(
      'bundle' => 'rooms',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => '',
          ),
          'type' => 'number_integer',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'bat_type',
      'field_name' => 'field_rooms_sleeps',
      'label' => 'Sleeps',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => 3,
      ),
    );
  }

  foreach ($field_instances as $instance) {
    field_create_instance($instance);
  }
}

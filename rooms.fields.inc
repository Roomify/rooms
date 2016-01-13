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
        'event_type' => 'availability_event',
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

  foreach ($field_instances as $instance) {
    field_create_instance($instance);
  }
}

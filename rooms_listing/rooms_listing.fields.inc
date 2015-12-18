<?php

/**
 * @file
 */

/**
 * Create Listing fields.
 */
function rooms_listing_create_listing_fields() {
  $field_bases = array();

  if (field_read_field('field_address') === FALSE) {
    $field_bases['field_address'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_address',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'addressfield',
      'settings' => array(),
      'translatable' => 0,
      'type' => 'addressfield',
    );
  }

  if (field_read_field('field_amenities') === FALSE) {
    $field_bases['field_amenities'] = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_amenities',
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

  if (field_read_field('field_area') === FALSE) {
    $field_bases['field_area'] = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_area',
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
            'vocabulary' => 'location',
            'parent' => 0,
          ),
        ),
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    );
  }

  if (field_read_field('field_area_type') === FALSE) {
    $field_bases['field_area_type'] = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_area_type',
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
            'vocabulary' => 'area_type',
            'parent' => 0,
          ),
        ),
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    );
  }

  if (field_read_field('field_availability_calendar') === FALSE) {
    $field_bases['field_availability_calendar'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_availability_calendar',
      'indexes' => array(
        'unit_id' => array(
          0 => 'unit_id',
        ),
      ),
      'locked' => 0,
      'module' => 'bat_availability_reference',
      'settings' => array(
        'referenceable_unit_types' => array(
          'bnb' => 'bnb',
        ),
      ),
      'translatable' => 0,
      'type' => 'bat_availability_reference',
    );
  }

  if (field_read_field('field_how_to_reach_the_place') === FALSE) {
    $field_bases['field_how_to_reach_the_place'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_how_to_reach_the_place',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text_long',
    );
  }

  if (field_read_field('field_location') === FALSE) {
    $field_bases['field_location'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_location',
      'indexes' => array(
        'bbox' => array(
          0 => 'top',
          1 => 'bottom',
          2 => 'left',
          3 => 'right',
        ),
        'bottom' => array(
          0 => 'bottom',
        ),
        'centroid' => array(
          0 => 'lat',
          1 => 'lon',
        ),
        'geohash' => array(
          0 => 'geohash',
        ),
        'lat' => array(
          0 => 'lat',
        ),
        'left' => array(
          0 => 'left',
        ),
        'lon' => array(
          0 => 'lon',
        ),
        'right' => array(
          0 => 'right',
        ),
        'top' => array(
          0 => 'top',
        ),
      ),
      'locked' => 0,
      'module' => 'geofield',
      'settings' => array(
        'backend' => 'default',
        'srid' => 4326,
      ),
      'translatable' => 0,
      'type' => 'geofield',
    );
  }

  if (field_read_field('field_location_featured') === FALSE) {
    $field_bases['field_location_featured'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_location_featured',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 'Not Featured',
          1 => 'Is Featured',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    );
  }

  if (field_read_field('field_location_image') === FALSE) {
    $field_bases['field_location_image'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_location_image',
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => 0,
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'profile2_private' => FALSE,
        'uri_scheme' => 'public',
      ),
      'translatable' => 0,
      'type' => 'image',
    );
  }

  if (field_read_field('field_owner') === FALSE) {
    $field_bases['field_owner'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_owner',
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => 0,
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'direction' => 'ASC',
            'property' => 'name',
            'type' => 'property',
          ),
          'target_bundles' => array(),
        ),
        'target_type' => 'user',
      ),
      'translatable' => 0,
      'type' => 'entityreference',
    );
  }

  if (field_read_field('field_property_bathrooms') === FALSE) {
    $field_bases['field_property_bathrooms'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_bathrooms',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 0,
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
          7 => 7,
          8 => 8,
          9 => 9,
          10 => 10,
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    );
  }

  if (field_read_field('field_property_bedrooms') === FALSE) {
    $field_bases['field_property_bedrooms'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_bedrooms',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 0,
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
          7 => 7,
          8 => 8,
          9 => 9,
          10 => 10,
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    );
  }

  if (field_read_field('field_property_description') === FALSE) {
    $field_bases['field_property_description'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_description',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text_long',
    );
  }

  if (field_read_field('field_property_double_bed') === FALSE) {
    $field_bases['field_property_double_bed'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_double_bed',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 0,
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
          7 => 7,
          8 => 8,
          9 => 9,
          10 => 10,
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    );
  }

  if (field_read_field('field_property_essentials') === FALSE) {
    $field_bases['field_property_essentials'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_essentials',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text_long',
    );
  }

  if (field_read_field('field_property_gallery') === FALSE) {
    $field_bases['field_property_gallery'] = array(
      'active' => 1,
      'cardinality' => -1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_gallery',
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => 0,
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'profile2_private' => FALSE,
        'uri_scheme' => 'public',
      ),
      'translatable' => 0,
      'type' => 'image',
    );
  }

  if (field_read_field('field_property_image') === FALSE) {
    $field_bases['field_property_image'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_image',
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => 0,
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'profile2_private' => FALSE,
        'uri_scheme' => 'public',
      ),
      'translatable' => 0,
      'type' => 'image',
    );
  }

  if (field_read_field('field_property_max_capacity') === FALSE) {
    $field_bases['field_property_max_capacity'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_max_capacity',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'number_integer',
    );
  }

  if (field_read_field('field_property_promo') === FALSE) {
    $field_bases['field_property_promo'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_promo',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text_long',
    );
  }

  if (field_read_field('field_property_short_description') === FALSE) {
    $field_bases['field_property_short_description'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_short_description',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 255,
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text',
    );
  }

  if (field_read_field('field_property_single_bed') === FALSE) {
    $field_bases['field_property_single_bed'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_single_bed',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => 0,
          1 => 1,
          2 => 2,
          3 => 3,
          4 => 4,
          5 => 5,
          6 => 6,
          7 => 7,
          8 => 8,
          9 => 9,
          10 => 10,
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_integer',
    );
  }

  if (field_read_field('field_property_type') === FALSE) {
    $field_bases['field_property_type'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_property_type',
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
            'vocabulary' => 'property_type',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => 0,
      'type' => 'taxonomy_term_reference',
    );
  }

  if (field_read_field('field_tax_description') === FALSE) {
    $field_bases['field_tax_description'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_tax_description',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 255,
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text',
    );
  }

  if (field_read_field('field_tax_enable') === FALSE) {
    $field_bases['field_tax_enable'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_tax_enable',
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => 0,
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'list_boolean',
    );
  }

  if (field_read_field('field_tax_rate') === FALSE) {
    $field_bases['field_tax_rate'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_tax_rate',
      'indexes' => array(),
      'locked' => 0,
      'module' => 'number',
      'settings' => array(
        'decimal_separator' => '.',
        'precision' => 10,
        'profile2_private' => FALSE,
        'scale' => 2,
      ),
      'translatable' => 0,
      'type' => 'number_decimal',
    );
  }

  if (field_read_field('field_tax_rule_id') === FALSE) {
    $field_bases['field_tax_rule_id'] = array(
      'active' => 1,
      'cardinality' => 1,
      'deleted' => 0,
      'entity_types' => array(),
      'field_name' => 'field_tax_rule_id',
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => 0,
      'module' => 'text',
      'settings' => array(
        'max_length' => 255,
        'profile2_private' => FALSE,
      ),
      'translatable' => 0,
      'type' => 'text',
    );
  }

  foreach ($field_bases as $field) {
    field_create_field($field);
  }


  $field_instances = array();

  if (field_read_instance('bat_unit', 'field_address', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_address'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Please complete the address field as well - we use the information here separately to a geocoded location.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'addressfield',
          'settings' => array(
            'conditions' => array(),
            'format_handlers' => array(
              0 => 'address',
            ),
            'use_widget_handlers' => 1,
          ),
          'type' => 'addressfield_default',
          'weight' => 2,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_address',
      'label' => 'Address',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'addressfield',
        'settings' => array(
          'available_countries' => array(),
          'default_country' => '',
          'format_handlers' => array(
            'address' => 'address',
            'address-hide-postal-code' => 0,
            'address-hide-street' => 0,
            'address-hide-country' => 0,
            'organisation' => 0,
            'name-full' => 0,
            'name-oneline' => 0,
            'address-optional' => 0,
          ),
        ),
        'type' => 'addressfield_standard',
        'weight' => 11,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_amenities', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_amenities'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 13,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_amenities',
      'label' => 'Amenities',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => 12,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_area', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_area'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 3,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_area',
      'label' => 'Area',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'autocomplete_deluxe',
        'settings' => array(
          'autocomplete_deluxe_path' => 'autocomplete_deluxe/taxonomy',
          'delimiter' => '',
          'limit' => 10,
          'min_length' => 0,
          'not_found_message' => 'The area \'@term\' will be added.',
          'size' => 60,
        ),
        'type' => 'autocomplete_deluxe_taxonomy',
        'weight' => 8,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_area_type', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_area_type'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 22,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_area_type',
      'label' => 'Area Type',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'autocomplete_deluxe',
        'settings' => array(
          'autocomplete_deluxe_path' => 'autocomplete_deluxe/taxonomy',
          'delimiter' => '',
          'limit' => 10,
          'min_length' => 0,
          'not_found_message' => 'The area type \'@term\' will be added.',
          'size' => 60,
        ),
        'type' => 'autocomplete_deluxe_taxonomy',
        'weight' => 9,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_availability_calendar', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_availability_calendar'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'bat_availability_reference',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'bat_availability_reference_default',
          'weight' => 5,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_availability_calendar',
      'label' => 'Availability Calendar',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'bat_availability_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'bat_availability_reference/autocomplete',
          'size' => 60,
        ),
        'type' => 'bat_availability_reference_autocomplete',
        'weight' => 12,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_how_to_reach_the_place', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_how_to_reach_the_place'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Please provide additional written instructions about how to get to your place.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 38,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_how_to_reach_the_place',
      'label' => 'How to reach the Place',
      'required' => 0,
      'settings' => array(
        'text_processing' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => 3,
        ),
        'type' => 'text_textarea',
        'weight' => 12,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_location', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_location'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'You can enter your address here and we will attempt to locate it on the map. If the placement is not precise you can move the marker to correct the location.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 11,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_location',
      'label' => 'Location',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'geofield_gmap',
        'settings' => array(
          'click_to_place_marker' => 0,
          'confirm_center_marker' => 0,
          'html5_geolocation' => 0,
          'map_type' => 'ROADMAP',
          'zoom_after_address_change' => 1,
          'zoom_level' => 1,
        ),
        'type' => 'geofield_gmap',
        'weight' => 10,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_owner', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_owner'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'default_value_function' => '',
      'deleted' => 0,
      'description' => 'Enter the user name of this property\'s owner. This field defaults to this unit\'s author, but may be set to another user. This name is used for reporting purposes, displaying the owner\'s name on Conversations and other purposes.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'conditions' => array(),
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 12,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_owner',
      'label' => 'Owner',
      'required' => 0,
      'settings' => array(
        'behaviors' => array(
          'prepopulate' => array(
            'status' => 0,
          ),
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => 60,
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => 0,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_bathrooms', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_bathrooms'] = array(
      'bundle' => 'listing',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'list_default',
          'weight' => 32,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_bathrooms',
      'label' => 'Bathrooms',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'apply_chosen' => '',
        ),
        'type' => 'options_select',
        'weight' => 44,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_bedrooms', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_bedrooms'] = array(
      'bundle' => 'listing',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'list_default',
          'weight' => 36,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_bedrooms',
      'label' => 'Bedrooms',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'apply_chosen' => '',
        ),
        'type' => 'options_select',
        'weight' => 43,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_description', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_description'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 8,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_description',
      'label' => 'Property Description',
      'required' => 1,
      'settings' => array(
        'text_processing' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => 5,
        ),
        'type' => 'text_textarea',
        'weight' => 47,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_double_bed', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_double_bed'] = array(
      'bundle' => 'listing',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'list_default',
          'weight' => 33,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_double_bed',
      'label' => 'Double beds',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'apply_chosen' => '',
        ),
        'type' => 'options_select',
        'weight' => 42,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_essentials', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_essentials'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Please give guests information about nearby essential services such as grocery stores, pharmacies, etc. here.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 39,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_essentials',
      'label' => 'Essentials',
      'required' => 0,
      'settings' => array(
        'text_processing' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => 5,
        ),
        'type' => 'text_textarea',
        'weight' => 13,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_gallery', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_gallery'] = array(
      'bundle' => 'listing',
      'deleted' => 0,
      'description' => 'These images are displayed in the slideshow on your listing page. These should be high quality images in order to best display your listing, minimum dimensions are 960x450 pixels.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'nivo_formatter',
          'settings' => array(
            'conditions' => array(),
            'nivo_formatter_image_style' => 'large',
            'nivo_formatter_nivo_slider' => array(
              'animSpeed' => 500,
              'boxCols' => 8,
              'boxRows' => 4,
              'captionOpacity' => 0.8,
              'controlNav' => TRUE,
              'directionNav' => 1,
              'directionNavHide' => TRUE,
              'effect' => array(
                'random' => 'random',
              ),
              'keyboardNav' => TRUE,
              'manualAdvance' => 0,
              'nextText' => 'Next',
              'pauseOnHover' => TRUE,
              'pauseTime' => 3000,
              'prevText' => 'Prev',
              'slices' => 15,
              'startSlide' => 0,
            ),
            'nivo_formatter_theme' => 'default',
            'nivo_slider_thumbnail' => array(
              'controlNavThumbs' => TRUE,
              'thumbnail_style' => 'thumbnail',
            ),
          ),
          'type' => 'nivo_formatter',
          'weight' => 7,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_gallery',
      'label' => 'Listing Gallery',
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'epsacrop' => array(
          'styles' => array(
            'blog_post_image' => 0,
            'homepage_features' => 0,
            'property_slider' => 0,
            'slick_property_slider' => 'slick_property_slider',
            'square' => 0,
            'thumb_unit_search' => 0,
          ),
        ),
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '2800x2800',
        'min_resolution' => '960x450',
        'title_field' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'filefield_sources' => array(
            'filefield_sources' => array(
              'attach' => 0,
              'clipboard' => 0,
              'plupload' => 'plupload',
              'reference' => 0,
              'remote' => 0,
              'upload' => 0,
            ),
            'source_attach' => array(
              'absolute' => 0,
              'attach_mode' => 'move',
              'path' => 'file_attach',
            ),
            'source_imce' => array(
              'imce_mode' => 0,
            ),
            'source_reference' => array(
              'autocomplete' => 0,
              'search_all_fields' => 0,
            ),
          ),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => 3,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_image', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_image'] = array(
      'bundle' => 'listing',
      'deleted' => 0,
      'description' => 'This is the image used in search results and anywhere your listing may be featured.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'conditions' => array(),
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 6,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_image',
      'label' => 'Search/Featured Image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
        'default_image' => 0,
        'epsacrop' => array(
          'styles' => array(
            'availability_block_background' => 0,
            'blog_post_image' => 0,
            'homepage_features' => 'homepage_features',
            'property_slider' => 0,
            'slick_property_slider' => 0,
            'square' => 'square',
            'thumb_unit_search' => 'thumb_unit_search',
          ),
        ),
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '2800x2800',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'filefield_sources' => array(
            'filefield_sources' => array(
              'attach' => 0,
              'clipboard' => 0,
              'plupload' => 0,
              'reference' => 0,
              'remote' => 0,
              'upload' => 'upload',
            ),
            'source_attach' => array(
              'absolute' => 0,
              'attach_mode' => 'move',
              'path' => 'file_attach',
            ),
            'source_imce' => array(
              'imce_mode' => 0,
            ),
            'source_reference' => array(
              'autocomplete' => 0,
              'search_all_fields' => 0,
            ),
          ),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => 48,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_max_capacity', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_max_capacity'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'conditions' => array(),
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 35,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_max_capacity',
      'label' => 'Max Capacity',
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
        'weight' => 40,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_promo', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_promo'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 23,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_promo',
      'label' => 'Promo',
      'required' => 0,
      'settings' => array(
        'text_processing' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => 5,
        ),
        'type' => 'text_textarea',
        'weight' => 31,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_short_description', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_short_description'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Provide a short description of the listing to show under the main title.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 37,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_short_description',
      'label' => 'Short Description',
      'required' => 0,
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 255,
        ),
        'type' => 'text_textfield',
        'weight' => 45,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_single_bed', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_single_bed'] = array(
      'bundle' => 'listing',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'list_default',
          'weight' => 34,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_single_bed',
      'label' => 'Single Beds',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'apply_chosen' => '',
        ),
        'type' => 'options_select',
        'weight' => 41,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_property_type', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_property_type'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 14,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_property_type',
      'label' => 'Property type',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'apply_chosen' => '',
        ),
        'type' => 'options_select',
        'weight' => 46,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_tax_description', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_tax_description'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Enter the text to be shown to guests placing a booking for this listing, describing the taxes they are paying. Example: "Austin municipal + Texas accommodation taxes."',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 24,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_tax_description',
      'label' => 'Tax description',
      'required' => 0,
      'settings' => array(
        'text_processing' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'text_textfield',
        'weight' => 33,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_tax_enable', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_tax_enable'] = array(
      'bundle' => 'listing',
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'list_default',
          'weight' => 27,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_tax_enable',
      'label' => 'Enable Tax',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 32,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_tax_rate', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_tax_rate'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => 'Enter the total tax rate for your listing as a percentage. For instance, if you have State tax of 10% and local tax of 5%, enter 15 in this field.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'conditions' => array(),
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 2,
            'thousand_separator' => '',
          ),
          'type' => 'number_decimal',
          'weight' => 30,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_tax_rate',
      'label' => 'Tax rate',
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
        'weight' => 35,
      ),
    );
  }

  if (field_read_instance('bat_unit', 'field_tax_rule_id', 'listing') === FALSE) {
    $field_instances['bat_unit-listing-field_tax_rule_id'] = array(
      'bundle' => 'listing',
      'default_value' => NULL,
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(
            'conditions' => array(),
          ),
          'type' => 'text_default',
          'weight' => 26,
        ),
      ),
      'entity_type' => 'bat_unit',
      'field_name' => 'field_tax_rule_id',
      'label' => 'Tax rule ID',
      'required' => 0,
      'settings' => array(
        'text_processing' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => 60,
        ),
        'type' => 'text_textfield',
        'weight' => 36,
      ),
    );
  }

  foreach ($field_instances as $instance) {
    field_create_instance($instance);
  }


  $field_groups = array();

  if (field_group_load_field_group('group_amenities', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_amenities|bat_unit|listing|form';
    $field_group->group_name = 'group_amenities';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Amenities';
    $field_group->weight = '5';
    $field_group->children = array(
      0 => 'field_amenities',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    );
    $field_groups['group_amenities|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_description', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_description|bat_unit|listing|form';
    $field_group->group_name = 'group_description';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Description';
    $field_group->weight = '1';
    $field_group->children = array(
      0 => 'field_property_bathrooms',
      1 => 'field_property_bedrooms',
      2 => 'field_property_description',
      3 => 'field_property_double_bed',
      4 => 'field_property_image',
      5 => 'field_property_max_capacity',
      6 => 'field_property_short_description',
      7 => 'field_property_single_bed',
      8 => 'field_property_type',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'label' => 'Description',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Please select the number of beds, bedrooms and bathrooms in your listing. For single beds, select the number of beds in which one person would normally sleep. (Twins, Single Bunks) For double beds, select the number of beds in which two people would normally sleep. (Queens, Kings)',
      ),
      'formatter' => 'closed',
    );
    $field_groups['group_description|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_images', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_images|bat_unit|listing|form';
    $field_group->group_name = 'group_images';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Image Gallery';
    $field_group->weight = '2';
    $field_group->children = array(
      0 => 'field_property_gallery',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    );
    $field_groups['group_images|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_location', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_location|bat_unit|listing|form';
    $field_group->group_name = 'group_location';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Location';
    $field_group->weight = '3';
    $field_group->children = array(
      0 => 'field_address',
      1 => 'field_area',
      2 => 'field_area_type',
      3 => 'field_how_to_reach_the_place',
      4 => 'field_location',
      5 => 'field_property_essentials',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    );
    $field_groups['group_location|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_options', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_options|bat_unit|listing|form';
    $field_group->group_name = 'group_options';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Options';
    $field_group->weight = '4';
    $field_group->children = array(
      0 => 'field_availability_calendar',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'label' => 'Options',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => 'Use this area to add options such as cleaning fees, pet fees or other services that guests may request. If you mark an option as Mandatory, it will be charged to all guests.',
      ),
      'formatter' => 'closed',
    );
    $field_groups['group_options|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_promo', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_promo|bat_unit|listing|form';
    $field_group->group_name = 'group_promo';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Promo';
    $field_group->weight = '7';
    $field_group->children = array(
      0 => 'field_property_promo',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-promo field-group-tab',
        'required_fields' => 1,
      ),
    );
    $field_groups['group_promo|bat_unit|listing|form'] = $field_group;
  }

  if (field_group_load_field_group('group_taxes', 'bat_unit', 'listing', 'form') === NULL) {
    $field_group = new stdClass();
    $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
    $field_group->api_version = 1;
    $field_group->identifier = 'group_taxes|bat_unit|listing|form';
    $field_group->group_name = 'group_taxes';
    $field_group->entity_type = 'bat_unit';
    $field_group->bundle = 'listing';
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->label = 'Taxes';
    $field_group->weight = '8';
    $field_group->children = array(
      0 => 'field_tax_description',
      1 => 'field_tax_enable',
      2 => 'field_tax_rate',
      3 => 'field_tax_rule_id',
    );
    $field_group->format_type = 'tab';
    $field_group->format_settings = array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-taxes field-group-tab',
        'required_fields' => 1,
      ),
    );
    $field_groups['group_taxes|bat_unit|listing|form'] = $field_group;
  }

  foreach ($field_groups as $group) {
    field_group_group_save($group);
  }
}

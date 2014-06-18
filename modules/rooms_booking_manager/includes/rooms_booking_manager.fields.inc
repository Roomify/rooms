<?php

/**
 * @file
 * Field definitions for the Rooms Booking Manager.
 */

/**
 * Helper function that returns field data for the line item fields.
 */
function _rooms_booking_manager_line_item_type_fields() {
  $field_data = array(
    'fields' => array(
      'rooms_booking_dates' => array(
        'field_name' => 'rooms_booking_dates',
        'label' => t('Booking Dates'),
        'cardinality' => 1,
        'type' => 'date',
        'module' => 'date',
        'active' => '1',
        'locked' => '1',
        'settings' => array(
          'repeat' => 0,
          'todate' => 'required',
          'granularity' => array(
            'year' => 'year',
            'month' => 'month',
            'day' => 'day',
          ),
          'tz_handling' => 'none',
          'timezone_db' => '',
        ),
      ),
      'rooms_booked_unit_id' => array(
        'field_name' => 'rooms_booked_unit_id',
        'label' => t('Booked Unit ID'),
        'cardinality' => 1,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booked_status' => array(
        'field_name' => 'rooms_booked_status',
        'label' => t('Booked Unit Status'),
        'cardinality' => 1,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booked_price' => array(
        'field_name' => 'rooms_booked_price',
        'label' => t('Booked Unit Price'),
        'cardinality' => 1,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booked_bookingprice' => array(
        'field_name' => 'rooms_booked_bookingprice',
        'label' => t('Booking Price'),
        'cardinality' => 1,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booking_number_people' => array(
        'field_name' => 'rooms_booking_number_people',
        'label' => t('Booking Number People'),
        'cardinality' => 2,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booking_options' => array(
        'field_name' => 'rooms_booking_options',
        'label' => t('Booking Options'),
        'cardinality' => -1,
        'required' => 0,
        'type' => 'rooms_options',
        'module' => 'rooms',
        'settings' => array(
          'user_register_form' => FALSE,
        ),
        'widget' => array(
          'active' => 0,
          'module' => 'rooms_unit',
          'settings' => array(),
          'type' => 'rooms_options_combined',
          'weight' => '0',
        ),
      ),
      'rooms_booking_children_ages' => array(
        'field_name' => 'rooms_booking_children_ages',
        'label' => t('Booking Children Ages'),
        'cardinality' => -1,
        'type' => 'number_integer',
        'module' => 'number',
        'active' => 1,
        'locked' => 1,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
      ),
      'rooms_booking_reference' => array(
        'field_name' => 'rooms_booking_reference',
        'label' => t('Booking Reference'),
        'cardinality' => 1,
        'required' => 0,
        'type' => 'entityreference',
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
              'type' => 'none',
            ),
            'target_bundles' => array(),
          ),
          'target_type' => 'rooms_booking',
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'entityreference',
          'settings' => array(
            'match_operator' => 'CONTAINS',
            'path' => '',
            'size' => '60',
          ),
          'type' => 'entityreference_autocomplete',
          'weight' => '9',
        ),
      ),
    ),
    'instances' => array(
      'rooms_booking_dates' => array(
        'field_name' => 'rooms_booking_dates',
        'label' => t('Booking Dates'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => FALSE,
        'widget' => array(
          'type' => 'date_popup',
          'module' => 'date',
          'settings' => array(
            'default_value' => 'now',
            'default_format' => 'medium',
          ),
        ),
      ),
      'rooms_booked_unit_id' => array(
        'field_name' => 'rooms_booked_unit_id',
        'label' => t('Booked Unit ID'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => TRUE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 11,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booked_status' => array(
        'field_name' => 'rooms_booked_status',
        'label' => t('Booked Unit Status'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => TRUE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 11,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booked_price' => array(
        'field_name' => 'rooms_booked_price',
        'label' => t('Booked Unit Price'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => TRUE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 11,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booked_bookingprice' => array(
        'field_name' => 'rooms_booked_bookingprice',
        'label' => t('Booking Price'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => TRUE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 11,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booking_number_people' => array(
        'field_name' => 'rooms_booking_number_people',
        'label' => t('Booking Number People'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => TRUE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 11,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booking_options' => array(
        'field_name' => 'rooms_booking_options',
        'label' => t('Booking Options'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'default_value' => NULL,
        'deleted' => '0',
        'description' => '',
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => NULL,
            'settings' => array(),
            'type' => 'rooms_options_default',
            'weight' => 12,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE,
        ),
        'widget' => array(
          'module' => 'rooms',
          'type' => 'rooms_options_combined',
        ),
      ),
      'rooms_booking_children_ages' => array(
        'field_name' => 'rooms_booking_children_ages',
        'label' => t('Booking Children Ages'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'required' => FALSE,
        'settings' => array(
          'size' => 8,
          'max_length' => 10,
          'text_processing' => 0,
        ),
        'widget' => array(
          'type' => 'text_textfield',
        ),
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'number',
            'settings' => array(
              'decimal_separator' => '.',
              'prefix_suffix' => TRUE,
              'scale' => 0,
              'thousand_separator' => ' ',
            ),
            'type' => 'number_integer',
            'weight' => 13,
          ),
          'teaser' => array(
            'label' => 'above',
            'settings' => array(),
            'type' => 'hidden',
            'weight' => 0,
          ),
        ),
      ),
      'rooms_booking_reference' => array(
        'field_name' => 'rooms_booking_reference',
        'label' => t('Booking Reference'),
        'entity_type' => 'commerce_line_item',
        'bundle' => 'rooms_booking',
        'default_value' => NULL,
        'display' => array(
          'default' => array(
            'label' => 'above',
            'module' => 'entityreference',
            'settings' => array(
              'link' => FALSE,
            ),
            'type' => 'entityreference_label',
            'weight' => 14,
          ),
        ),
        'required' => 0,
        'settings' => array(
          'user_register_form' => FALSE,
        ),
        'widget' => array(
          'active' => 1,
          'module' => 'entityreference',
          'settings' => array(
            'match_operator' => 'CONTAINS',
            'path' => '',
            'size' => '60',
          ),
          'type' => 'entityreference_autocomplete',
          'weight' => '9',
        ),
      ),
    ),
  );

  return $field_data;
}

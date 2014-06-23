<?php

/**
 * @file
 * Features related hooks for rooms_unit module.
 */

/**
 * Implements hook_features_pipe_COMPONENT_alter() for rooms_unit_type component.
 */
function rooms_unit_features_pipe_rooms_unit_type_alter(&$pipe, $data, $export) {
  if (!empty($data)) {
    // Support the separate field base -vs- field instance structure that was
    // added in Features v7.x-2.0-beta2.
    if (function_exists('field_instance_features_export')) {
      $export_var = 'field_instance';
    }
    else {
      $export_var = 'field';
    }

    foreach ($data as $rooms_unit_type) {
      $pipe[$export_var][] = 'rooms_unit_type-rooms_unit_type-rooms_booking_unit_options';
    }
  }
}

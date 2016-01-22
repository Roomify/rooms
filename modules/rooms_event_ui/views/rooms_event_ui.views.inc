<?php

/**
 * @file
 *
 * Field handlers for Rooms Event UI.
 */

/**
 * Implements hook_views_data_alter().
 */
function rooms_event_ui_views_data_alter(&$data) {

  $data['bat_types']['calendars'] = array(
    'field' => array(
      'title' => t('Calendar links'),
      'help' => t('Display links to manage all calendars for Rooms Type.'),
      'handler' => 'rooms_event_ui_handler_type_calendars_field',
    ),
  );
}

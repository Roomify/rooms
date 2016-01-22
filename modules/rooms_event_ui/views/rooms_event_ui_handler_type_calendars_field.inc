<?php

/**
 * @file
 * This field handler aggregates calendar edit links for a Bat Type
 * under a single field.
 */

class rooms_event_ui_handler_type_calendars_field extends bat_type_handler_type_calendars_field {

  function construct() {
    parent::construct();
  }

  function query() {
    parent::construct();
  }

  function render($values) {

    if (isset($values->type_id)) {
      if ($type = bat_type_load($values->type_id)) {
        // Use a rooms custom render.
        if ($type->type == 'rooms') {
          $links = array();
          $type_bundle = bat_type_bundle_load($type->type);
          if (is_array($type_bundle->default_event_value_field_ids)) {
            foreach ($type_bundle->default_event_value_field_ids as $event => $field) {
              if (!empty($field)) {
                $event_label = bat_event_get_types($event)->label;
                $links[$field] = array(
                  'title' => 'Manage ' . $event_label,
                  'href' => 'admin/rooms/calendar/' . $type->type_id . '/' . $event,
                );
              }
            }
          }

          if (!empty($links)) {
            return theme('links', array(
              'links' => $links,
              'attributes' => array(
                'class' => array(
                  'links',
                  'inline',
                  'calendars',
                ),
              ),
            ));
          }
        }

        // Let's use the bat_unit defined handler.
        else {
          return parent::render($values);
        }
      }
    }
  }
}

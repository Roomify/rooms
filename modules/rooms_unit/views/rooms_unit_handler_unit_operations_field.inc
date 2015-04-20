<?php

/**
 * @file
 * This field handler aggregates operations that can be done on a unit
 * under a single field providing a more flexible way to present them in a view.
 */

class rooms_unit_handler_unit_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['unit_id'] = 'unit_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $links = menu_contextual_links('rooms_unit', 'admin/rooms/units/unit', array($this->get_value($values, 'unit_id')));

    // Unset the "view" link, as this is provided separately.
    if (array_key_exists('rooms_unit-view', $links)) {
      unset($links['rooms_unit-view']);
    }

    if (!empty($links)) {
      return theme('links', array(
        'links' => $links,
        'attributes' => array(
          'class' => array(
            'links',
            'inline',
            'operations',
          ),
        ),
      ));
    }
  }
}

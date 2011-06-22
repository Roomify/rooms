<?php

/**
 * This field handler aggregates operations that can be done on a room
 * under a single field providing a more flexible way to present them in a view
 */
class rooms_room_handler_room_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['room_id'] = 'room_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('rooms_room', 'admin/rooms/rooms/room', array($this->get_value($values, 'room_id')));
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}

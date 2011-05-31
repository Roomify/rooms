<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class rooms_room_handler_delete_link_field extends rooms_room_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy room to check access against
    $dummy_room = (object) array('type' => $type);
    if (!rooms_room_access('edit', $dummy_room)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $room_id = $values->{$this->aliases['room_id']};
    
    return l($text, 'admin/rooms/rooms/room/' . $room_id . '/delete');
  }
}

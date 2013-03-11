<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class rooms_unit_handler_delete_link_field extends rooms_unit_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy room to check access against
    $dummy_unit = (object) array('type' => $type);
    if (!rooms_unit_access('edit', $dummy_unit)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $unit_id = $values->{$this->aliases['unit_id']};

    return l($text, 'admin/rooms/units/unit/' . $unit_id . '/delete');
  }
}

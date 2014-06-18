<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields.
 */

class rooms_unit_handler_edit_link_field extends rooms_unit_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    // Creating a dummy unit to check access against.
    $dummy_unit = (object) array('type' => $type);
    if (!rooms_unit_access('update', $dummy_unit)) {
      return;
    }
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $unit_id = $values->{$this->aliases['unit_id']};

    return l($text, 'admin/rooms/units/unit/' . $unit_id . '/edit');
  }
}

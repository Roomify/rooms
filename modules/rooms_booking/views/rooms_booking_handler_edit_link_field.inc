<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class rooms_booking_handler_edit_link_field extends rooms_booking_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy booking to check access against
    $dummy_booking = (object) array('type' => $type);
    if (!rooms_booking_access('edit', $dummy_booking)) {
      return;
    }
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $booking_id = $values->{$this->aliases['booking_id']};

    return l($text, 'admin/rooms/bookings/booking/' . $booking_id . '/edit');
  }
}

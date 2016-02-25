<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying booking status options.
 */

class rooms_booking_handler_field_booking_status extends views_handler_field {

  function render($values) {
    $value = $this->get_value($values);

    if ($value == 0) {
      return 'Unconfirmed';
    }
    elseif ($value == 1) {
      return 'Confirmed';
    }

    return '';
  }

}

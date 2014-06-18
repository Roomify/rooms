<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying the correct label
 * for unit types.
 */


class rooms_booking_handler_booking_type_field extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function render($values) {
    $booking_type = rooms_booking_type_load($values->rooms_bookings_type);
    return $booking_type->label;
  }

}

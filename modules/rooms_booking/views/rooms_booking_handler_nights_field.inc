<?php

/**
 * @file
 */

class rooms_booking_handler_nights_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['start_date'] = array(
      'table' => 'rooms_bookings',
      'field' => 'start_date',
    );
    $this->additional_fields['end_date'] = array(
      'table' => 'rooms_bookings',
      'field' => 'end_date',
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $start_date = DateTime::createFromFormat('Y-m-d', $this->sanitize_value($values->rooms_bookings_start_date));
    $end_date = DateTime::createFromFormat('Y-m-d', $this->sanitize_value($values->rooms_bookings_end_date));

    return $start_date->diff($end_date)->d;
  }
}

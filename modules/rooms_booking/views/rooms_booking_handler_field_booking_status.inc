<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying booking status options.
 */

class rooms_booking_handler_field_booking_status extends views_handler_field {
	function render($values) {
		if ($values->rooms_bookings_booking_status == 0) return 'Unconfirmed';
		elseif ($values->rooms_bookings_booking_status == 1) return 'Confirmed';

		return '';
	}
}

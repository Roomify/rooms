<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * This hook allows to modify the search results before being rendered to the
 * end user.
 *
 * @param array $units_per_type
 *   Array containing the units from the search results. Keyed by unit type and
 *   price.
 * @param DateTime $start_date
 *   The booking start date.
 * @param DateTime $end_date
 *   The booking end date.
 * @param array $booking_parameters
 *   Array containing the parameters entered in the search form.
 */
function hook_rooms_booking_results_alter(&$units_per_type, $start_date, $end_date, $booking_parameters) {
  // Remove form results all unit of type villa and price greater than 200.
  foreach ($units_per_type['villa'] as $key => $value) {
    if ($key > 200) {
      unset($units_per_type['villa'][$key]);
    }
  }
}

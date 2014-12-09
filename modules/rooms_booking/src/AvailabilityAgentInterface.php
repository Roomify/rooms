<?php
/**
 * Created by PhpStorm.
 * User: plopesc
 * Date: 9/12/14
 * Time: 13:21
 */
namespace Drupal\rooms_booking;


/**
 * An AvailabilityAgentBase provides access to the availability functionality of
 * Rooms and lets you query for availability, get pricing information and create
 * products that can be bought.
 *
 * The Agent is essentially a factory creating the appropriate responses for us
 * as needed based on the requests and the current status of our bookable units.
 *
 * An Agent reasons over a single set of information regarding a booking which
 * are exposed as public variables to make it easy for us to set and or change them.
 */
interface AvailabilityAgentInterface {
  public function setDates(\DateTime $start_date, \DateTime $end_date);

  /**
   * Sets the valid states for an availability search.
   *
   * Defaults are "ROOMS_AVAILABLE" and "ROOMS_ON_REQUEST"
   *
   * @param array $states
   *   The valid states to perform the search.
   */
  public function setValidStates($states = array(
      ROOMS_AVAILABLE,
      ROOMS_ON_REQUEST,
      ROOMS_UNCONFIRMED_BOOKINGS
    ));

  /**
   * Checks the availability.
   *
   * If valid units exist an array keyed by valid unit ids containing unit and
   * the states it holds during the requested period or a message as to what
   * caused the failure.
   *
   * @param bool $confirmed
   *   Whether include confirmed states or not.
   *
   * @return array|int
   *   Bookable units remaining after the filter, error code otherwise.
   */
  public function checkAvailability($confirmed = FALSE);
}
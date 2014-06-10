<?php

interface BookingEventInterface extends RoomsEventInterface {
  /**
   * Lock event - updates the rooms_booking_locks table to indicate that this
   * event is locked
   *
   * @return bool
   *   TRUE if the event is locked successfully, FALSE if was already blocked.
   */
  public function lock();

  /**
   * Unlocks the event.
   */
  public function unlock();

  /**
   * Returns event in a format amenable to FullCalendar display or generally
   * sensible json
   *
   * @param int $style
   *   The visualization style.
   * @param string $unit_name
   *   The bookable unit name.
   *
   * @return array
   *   The processed event, in JSON ready format.
   */
  public function formatJson($style = ROOMS_AVAILABILITY_ADMIN_STYLE, $unit_name = '');
}
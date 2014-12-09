<?php

/**
 * @file
 * Contains \Drupal\rooms_pricing\PricingEventInterface.
 */

namespace Drupal\rooms_pricing;

use Drupal\rooms\RoomsEventInterface;

interface PricingEventInterface extends RoomsEventInterface {
  /**
   * Applies an operation against a Price event.
   *
   * @param int $amount
   *   The operation amount.
   * @param string $operation
   *   The operation type.
   * @param int $days
   *   The number of days the event lasts.
   */
  public function applyOperation($amount, $operation, $days);

  /**
   * Returns event in a format amenable to FullCalendar display or generally
   * sensible json.
   *
   * @return array
   *   The processed event, in JSON ready format.
   */
  public function formatJson();

}

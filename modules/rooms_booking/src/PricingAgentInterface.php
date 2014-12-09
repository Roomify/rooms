<?php
/**
 * Created by PhpStorm.
 * User: plopesc
 * Date: 9/12/14
 * Time: 13:16
 */
namespace Drupal\rooms_booking;

interface PricingAgentInterface {
  public function setDates(\DateTime $start_date, \DateTime $end_date);

  /**
   * Returns the units array in a specific format based on price.
   *
   * @param array $results
   *   Units to sort.
   * @param array $price_modifiers
   *   Price modifiers.
   *
   * @return array
   *   Units in a price based structure.
   */
  public function getUnitsByPriceType($results, $price_modifiers = array());
}
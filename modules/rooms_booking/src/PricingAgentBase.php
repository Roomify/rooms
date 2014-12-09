<?php
/**
 * Created by PhpStorm.
 * User: plopesc
 * Date: 9/12/14
 * Time: 13:17
 */

namespace Drupal\rooms_booking;


abstract class PricingAgentBase implements PricingAgentInterface {
  /**
   * The start date for pricing calculation.
   *
   * @var \DateTime
   */
  public $startDate;

  /**
   * The departure date.
   *
   * @var \DateTime
   */
  public $endDate;

  /**
   * {@inheritdoc}
   */
  public function setDates(\DateTime $start_date, \DateTime $end_date) {
    $this->startDate = $start_date;
    $this->endDate = $end_date;
  }

  /**
   * Ordering units by the optional items that are available.
   *
   * @param array $units
   *   Units to sort.
   *
   * @return array
   *   Sorted units by number of options.
   */
  protected function orderByOptionals($units) {
    foreach ($units as $type => $v) {
      foreach ($v as $price => $value) {
        uasort($value, array(get_class($this), 'compareByOptionals'));
        $units[$type][$price] = $value;
      }
    }

    return $units;
  }

  /**
   * Compares two bookable units based on the number of available options.
   *
   * @param array $unit_a
   *   First unit.
   * @param array $unit_b
   *   Second unit.
   *
   * @return int
   *   Comparison result.
   */
  protected static function compareByOptionals($unit_a, $unit_b) {
    $a_items = rooms_unit_get_unit_options($unit_a['unit']);
    $b_items = rooms_unit_get_unit_options($unit_b['unit']);

    if (count($a_items) == count($b_items)) {
      return $unit_a['unit']->unit_id < $unit_b['unit']->unit_id ? 1 : -1;
    }
    else {
      return count($a_items) < count($b_items) ? 1 : -1;
    }
  }

} 
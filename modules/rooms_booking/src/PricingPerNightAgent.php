<?php
/**
 * @file
 * Contains \Drupal\rooms_booking\PricingPerNightAgent.
 */

namespace Drupal\rooms_booking;

class PricingPerNightAgent extends PricingAgentBase implements PricingAgentInterface, PricingAgentInterface {

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
   * {@inheritdoc}
   */
  public function getUnitsByPriceType($results, $price_modifiers = array()) {
    $units = array();

    if (count($results) > 0) {
      foreach ($results as $unit) {
        // Get the actual entity.
        $unit = rooms_unit_load($unit->unit_id);

        // Get a calendar and check availability.
        $rc = new \UnitCalendar($unit->unit_id);
        // We need to make this based on user-set vars.
        // Rather than using $rc->stateAvailability we will get the states check
        // directly as different states will impact on what products we create.
        $states = $rc->getStates($this->startDate, $this->endDate);

        // Calculate the price as well to add to the array.
        $temp_end_date = clone($this->endDate);
        $temp_end_date->add(new \DateInterval('P1D'));

        $booking_info = array(
          'start_date' => clone($this->startDate),
          'end_date' => $temp_end_date,
          'unit' => $unit,
          'booking_parameters' => $this->booking_parameters,
        );

        // Give other modules a chance to change the price modifiers.
        drupal_alter('rooms_price_modifier', $price_modifiers, $booking_info);

        $price_calendar = new \UnitPricingCalendar($unit->unit_id, $price_modifiers);

        $price = $price_calendar->calculatePrice($this->startDate, $this->endDate);
        $full_price = $price['full_price'];

        $units[$unit->type][$full_price][$unit->unit_id]['unit'] = $unit;
        $units[$unit->type][$full_price][$unit->unit_id]['price'] = $full_price;
        $units[$unit->type][$full_price][$unit->unit_id]['booking_price'] = $price['booking_price'];

        if (in_array(ROOMS_ON_REQUEST, $states)) {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_ON_REQUEST;
        }
        else {
          $units[$unit->type][$full_price][$unit->unit_id]['state'] = ROOMS_AVAILABLE;
        }
      }
    }

    // We order units by optional items to ensure that units with options are
    // the first to be picked by a user.
    $units = $this->orderByOptionals($units);

    return $units;
  }

}
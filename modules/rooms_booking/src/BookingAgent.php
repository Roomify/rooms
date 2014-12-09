<?php
/**
 * @file
 * Contains \Drupal\rooms_booking\BookingAgent.
 */

namespace Drupal\rooms_booking;

class BookingAgent {

  /**
   * The availability agent.
   *
   * @var AvailabilityAgentInterface
   */
  protected $availabilityAgent;

  /**
   * The pricing agent.
   *
   * @var PricingAgentInterface
   */
  protected $pricingAgent;

  /**
   * The start date for availability search.
   *
   * @var \DateTime
   */
  public $startDate;

  /**
   * The departure date
   *
   * @var \DateTime
   */
  public $endDate;

  /**
   * The computed departure date
   *
   * @var \DateTime
   */
  public $computedEndDate;

  /**
   * Creates a new \Drupal\rooms_booking\BookingAgent instance.
   *
   * @param AvailabilityAgentInterface $availability_agent
   * The availability agent.
   * @param PricingAgentInterface $pricing_agent
   * The pricing agent.
   */
  public function __construct(AvailabilityAgentInterface $availability_agent, PricingAgentInterface $pricing_agent) {
    $this->availabilityAgent = $availability_agent;
    $this->pricingAgent = $pricing_agent;
  }

  public function setDates(\DateTime $start_date, \DateTime $end_date) {
    $this->startDate = $start_date;
    $this->endDate = $end_date;
    $computed_end_date = clone($end_date);
    $this->computedEndDate = $computed_end_date->sub(new \DateInterval('P1D'));
  }

  public function checkAvailability($confirmed = FALSE) {
    if (!isset($this->startDate) || !isset($this->endDate) || !isset($this->computedEndDate)) {
      throw new \InvalidArgumentException(t('You must set start and end booking dates'));
    }

    $this->availabilityAgent->setDates($this->startDate, $this->computedEndDate);

    return $this->availabilityAgent->checkAvailability($confirmed);
  }

  public function getUnitPrices($units) {
    if (!isset($this->startDate) || !isset($this->endDate) || !isset($this->computedEndDate)) {
      throw new \InvalidArgumentException(t('You must set start and end booking dates'));
    }

    $this->pricingAgent->setDates($this->startDate, $this->computedEndDate);

    return $this->pricingAgent->getUnitsByPriceType($units);
  }

  public function checkAvailabilityAndCalculatePrice($confirmed = FALSE) {
    if (($units = $this->checkAvailability()) && is_array($units)) {
      return $this->getUnitPrices($units);
    }
  }

}
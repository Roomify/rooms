<?php

use Behat\Behat\Context\ClosuredContextInterface,
    Behat\Behat\Context\TranslatedContextInterface,
    Behat\Behat\Context\BehatContext,
    Behat\Behat\Exception\PendingException;
use Behat\Gherkin\Node\PyStringNode,
    Behat\Gherkin\Node\TableNode;

//
// Require 3rd-party libraries here:
//
//   require_once 'PHPUnit/Autoload.php';
//   require_once 'PHPUnit/Framework/Assert/Functions.php';
//

/**
 * Features context.
 */
class FeatureContext extends Drupal\DrupalExtension\Context\DrupalContext
{

  /**
   * Keep track of bookable units so they can be cleaned up.
   *
   * @var array
   */
  public $units = array();

  /**
   * Keep track of bookable unit types so they can be cleaned up.
   *
   * @var array
   */
  public $unitTypes = array();

  /**
   * Keep track of bookings so they can be cleaned up.
   *
   * @var array
   */
  public $bookings = array();

  /**
   * Keep track of booking types so they can be cleaned up.
   *
   * @var array
   */
  public $bookingTypes = array();

  /**
   * Keep track of customer profiles so they can be cleaned up.
   *
   * @var array
   */
  public $customerProfiles = array();

  /**
   * Initializes context.
   * Every scenario gets its own context object.
   *
   * @param array $parameters context parameters (set them up through behat.yml)
   */
  public function __construct(array $parameters) {
    // Initialize your context here
  }

  public function afterScenario($event) {
    parent::afterScenario($event);

    if (!empty($this->units)) {
      foreach ($this->units as $unit) {
        $unit->delete();
      }
    }

    if (!empty($this->unitTypes)) {
      foreach ($this->unitTypes as $unit_type) {
        $unit_type->delete();
      }
    }

    if (!empty($this->bookingTypes)) {
      foreach ($this->bookingTypes as $booking_type) {
        $booking_type->delete();
      }
    }

    if (!empty($this->bookings)) {
      rooms_booking_delete_multiple($this->bookings);
    }

    if (!empty($this->customerProfiles)) {
      commerce_customer_profile_delete_multiple($this->customerProfiles);
      db_delete('rooms_customers')
        ->condition('commerce_customer_id', $this->customerProfiles)
        ->execute();
    }
  }

  /**
   * @When /^I am on the "([^"]*)" unit$/
   */
  public function iAmOnTheUnit($unit_name) {
    $this->iAmDoingOnTheUnit('view', $unit_name);
  }

  /**
   * @When /^I am editing the "([^"]*)" unit$/
   */
  public function iAmEditingTheUnit($unit_name) {
    $this->iAmDoingOnTheUnit('edit', $unit_name);
  }

  /**
   * @When /^I am deleting the "([^"]*)" unit$/
   */
  public function iAmDeletingTheUnit($unit_name) {
    $this->iAmDoingOnTheUnit('delete', $unit_name);
  }

  /**
   * @When /^I am managing the "([^"]*)" unit availability$/
   */
  public function iAmManagingTheUnitAvailability($unit_name) {
    $this->iAmDoingOnTheUnit('availability', $unit_name);
  }

  /**
   * @When /^I am managing the "([^"]*)" unit pricing$/
   */
  public function iAmManagingTheUnitPricing($unit_name) {
    $this->iAmDoingOnTheUnit('pricing', $unit_name);
  }

  /**
   * Returns a unit_id from its name.
   *
   * @param $unit_name
   * @return int
   * @throws RuntimeException
   */
  protected function findBookableUnitByName($unit_name) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'rooms_unit')
      ->propertyCondition('name', $unit_name);
    $results = $efq->execute();
    if ($results && isset($results['rooms_unit'])) {
      return key($results['rooms_unit']);
    }
    else {
      throw new RuntimeException('Unable to find that bookable unit');
    }
  }

  /**
   * Redirects user to the action page for the given unit.
   *
   * @param $action
   * @param $unit_name
   */
  protected function iAmDoingOnTheUnit($action, $unit_name) {
    $unit_id = $this->findBookableUnitByName($unit_name);
    $url = "admin/rooms/units/unit/$unit_id/$action";
    $this->getSession()->visit($this->locatePath($url));
  }

  /**
   * @Given /^"(?P<type>[^"]*)" units:$/
   */
  public function createUnits($type, TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      $nodeHash['type'] = $type;
      $nodeHash += array(
        'default_state' => 1,
      );
      $unit = rooms_unit_create($nodeHash);
      if (isset($this->user->uid)) {
        $unit->uid = $this->user->uid;
      }
      $unit->save();
      $this->units[] = $unit;
    }
  }

  /**
   * @Given /^unit types:$/
   */
  public function createUnitTypes(TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      $unit_type_definition = array();

      $unit_type_definition['type'] = isset($nodeHash['type']) ? $nodeHash['type'] :drupal_strtolower($this->getDrupal()->random->name(8));
      $unit_type_definition['label'] = isset($nodeHash['label']) ? $nodeHash['label'] : $this->getDrupal()->random->name(8);

      $other_properties = array('base_price', 'min_children', 'max_children', 'min_sleeps', 'max_sleeps');
      foreach ($other_properties as $property) {
        if (isset($nodeHash[$property])) {
          $unit_type_definition['data'][$property] = $nodeHash[$property];
        }
      }

      $unit_type = rooms_unit_type_create($unit_type_definition);
      $unit_type->save();
      $this->unitTypes[] = $unit_type;
    }
  }

  /**
   * @Then /^the state for "([^"]*)" between "([^"]*)" and "([^"]*)" should be "([^"]*)"$/
   */
  public function theStateForBetweenAndShouldBe($unit_name, $start_date, $end_date, $state) {
    $this->checkUnitPropertyRange($unit_name, $start_date, $end_date, $state, 'availability');
  }

  /**
   * @Then /^the price for "([^"]*)" between "([^"]*)" and "([^"]*)" should be "([^"]*)"$/
   */
  public function thePriceForBetweenAndShouldBe($unit_name, $start_date, $end_date, $state) {
    $this->checkUnitPropertyRange($unit_name, $start_date, $end_date, $state, 'pricing');
  }

  /**
   * Checks the state of a unit for a given range of dates.
   *
   * @param $unit_name
   *   The name of the unit actions be performed.
   * @param $start_date
   *   The range start date.
   * @param $end_date
   *   The range end date.
   * @param $expected_value
   *   The id that the event should have.
   * @param $type
   *   The operation to perform type. Can be pricing or availability.
   *
   * @throws RuntimeException
   */
  protected function checkUnitPropertyRange($unit_name, $start_date, $end_date, $expected_value, $type) {
    $unit_id = $this->findBookableUnitByName($unit_name);
    $start = new DateTime($start_date);
    $start_format = $start->format('Y-m-d');
    $end = new DateTime($end_date);
    $end_format = $end->format('Y-m-d');

    foreach ($this->monthsBetweenDates($start, $end) as $month) {
      $path = "rooms/units/unit/$unit_id/$type/json/{$month->format('Y/m')}";
      $this->getSession()->visit($this->locatePath($path));
      $content = $this->getSession()->getPage()->find('xpath', '/body')
        ->getHtml();
      $events = json_decode($content);

      foreach ($events as $event) {
        $event_start = new DateTime($event->start);
        $event_end = new DateTime($event->end);

        // Discard events out of the range to check.
        if (($start_format > $event_end->format('Y-m-d')) || ($end_format < $event_start->format('Y-m-d'))) {
          continue;
        }
        // Throw exception if the event id is not the desired.
        if ($event->id != $expected_value) {
          throw new RuntimeException("The $type for unit $unit_name between $start_date and $end_date is not always $expected_value");
        }
      }
    }
  }

  /**
   * Helper function that returns a period between two dates.
   *
   * @param DateTime $start
   *   The start date.
   * @param DateTime $end
   *   The end date.
   *
   * @return DatePeriod
   *   The period between two dates
   */
  protected function monthsBetweenDates($start, $end) {
    $period_start = clone($start);
    $period_end = clone($end);
    $period_start->modify('first day of this month');
    $period_end->modify('first day of next month');
    $interval = DateInterval::createFromDateString('1 month');
    $period = new DatePeriod($period_start, $interval, $period_end);

    return $period;
  }

  /**
   * @Given /^"(?P<type>[^"]*)" bookings:$/
   */
  public function createBookings($type, TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      $profile_id = $this->customerProfiles[$nodeHash['profile_id']];

      $profile = commerce_customer_profile_load($profile_id);
      $client_name = isset($profile->commerce_customer_address['und'][0]['name_line']) ? $profile->commerce_customer_address['und'][0]['name_line'] : $nodeHash['profile_id'];

      // Save customer in rooms_customers table.
      db_merge('rooms_customers')
        ->key(array('name' => $client_name))
        ->fields(array(
          'name' => $client_name,
          'commerce_customer_id' => $profile_id,
        ))
        ->execute();

      // Get customer id from rooms_customers table.
      $client_id = db_select('rooms_customers')
        ->fields('rooms_customers', array('id'))
        ->condition('name', $client_name, '=')
        ->execute()->fetchField();

      $unit_id = $this->findBookableUnitByName($nodeHash['unit']);
      $unit = rooms_unit_load($unit_id);
      $unit_type = $unit->type;
      $data = array(
        'type' => $type,
        'name' => $client_name,
        'customer_id' => $client_id,
        'unit_id' => $unit_id,
        'unit_type' => $unit_type,
        'start_date' => $nodeHash['start_date'],
        'end_date' => $nodeHash['end_date'],
        'booking_status' => $nodeHash['status'],
        'data' => array(
          'group_size' => $nodeHash['guests'],
          'group_size_children' => $nodeHash['children'],
        ),
      );
      $booking = rooms_booking_create($data);
      $booking->save();
      $this->bookings[] = $booking->booking_id;
    }
  }

  /**
   * @Given /^customer profiles:$/
   */
  public function createCustomerProfiles(TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      $profile = commerce_customer_profile_new('billing', isset($this->user->uid) ? $this->user->uid : 0);
      $wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
      if (isset($nodeHash['country'])) {
        $wrapper->commerce_customer_address->country = $nodeHash['country'];
      }
      if (isset($nodeHash['name'])) {
        $wrapper->commerce_customer_address->name_line = $nodeHash['name'];
      }
      if (isset($nodeHash['address'])) {
        $wrapper->commerce_customer_address->thoroughfare = $nodeHash['address'];
      }
      if (isset($nodeHash['locality'])) {
        $wrapper->commerce_customer_address->locality = $nodeHash['locality'];
      }
      if (isset($nodeHash['postal_code'])) {
        $wrapper->commerce_customer_address->postal_code = $nodeHash['postal_code'];
      }
      $wrapper->save();
      if (isset($nodeHash['profile_id'])) {
        $this->customerProfiles[$nodeHash['profile_id']] = $wrapper->profile_id->value();
      }
      else {
        $this->customerProfiles[] = $wrapper->profile_id->value();
      }
    }
  }

  /**
   * @Given /^booking types:$/
   */
  public function createBookingTypes(TableNode $nodesTable) {
    foreach ($nodesTable->getHash() as $nodeHash) {
      $booking_type_definition = array();

      $booking_type_definition['type'] = isset($nodeHash['type']) ? $nodeHash['type'] :drupal_strtolower($this->getDrupal()->random->name(8));
      $booking_type_definition['label'] = isset($nodeHash['label']) ? $nodeHash['label'] : $this->getDrupal()->random->name(8);

      $booking_type = rooms_boking_type_create($booking_type_definition);
      $booking_type->save();
      $this->bookingTypes[] = $booking_type;
    }
  }

  /**
   * @Then /^the "([^"]*)" unit should be Unconfirmed by the last booking between "([^"]*)" and "([^"]*)"$/
   */
  public function theUnitShouldBeUnconfirmedBetweenAnd($unit_name, $start_date, $end_date) {
    $this->checkUnitLockedByLastBooking($unit_name, $start_date, $end_date, 0);
  }

  /**
   * @Then /^the "([^"]*)" unit should be Confirmed by the last booking between "([^"]*)" and "([^"]*)"$/
   */
  public function theUnitShouldBeConfirmedBetweenAnd($unit_name, $start_date, $end_date) {
    $this->checkUnitLockedByLastBooking($unit_name, $start_date, $end_date, 1);
  }

  /**
   * @Given /^options for "([^"]*)" unit type:$/
   */
  public function optionsForUnitType($unit_type, TableNode $table) {
    $wrapper = entity_metadata_wrapper('rooms_unit_type', $unit_type);
    $this->addOptionsToEntity($table, $wrapper);
  }

  /**
   * @Given /^options for "([^"]*)" unit:$/
   */
  public function optionsForUnit($unit_name, TableNode $table) {
    $unit_id = $this->findBookableUnitByName($unit_name);
    $wrapper = entity_metadata_wrapper('rooms_unit', $unit_id);
    $this->addOptionsToEntity($table, $wrapper);
  }

  /**
   * @Given /^I add a constraint from "(?P<start>[^"]*)" to "(?P<end>[^"]*)" "(?P<constraint_type>[^"]*)" start on "(?P<start_day>[^"]*)" and the minimum is "(?P<minimum>[^"]*)" and the maximum is "(?P<maximum>[^"]*)"$/
   */
  public function iAddAConstraintFromToMustStartOnAndTheMinimumIsAndTheMaximumIs($start, $end, $constraint_type, $start_day, $minimum, $maximum) {
    $this->addAvailabilityConstraint($minimum, $maximum, $constraint_type, $start_day, $start, $end);
  }

  /**
   * @Given /^I add an always constraint where "(?P<constraint_type>[^"]*)" start on "(?P<start_day>[^"]*)" and the minimum is "(?P<minimum>[^"]*)" and the maximum is "(?P<maximum>[^"]*)"$/
   */
  public function iAddAnAlwaysConstraintWhereTheMinimumIsAndTheMaximumIs($minimum, $maximum, $constraint_type, $start_day) {
    $this->addAvailabilityConstraint($minimum, $maximum, $constraint_type, $start_day);
  }

  /**
   * @Then /^I will be able to make a booking for "(?P<unit_name>[^"]*)" unit from "(?P<start>[^"]*)" to "(?P<end>[^"]*)"$/
   */
  public function iWillBeAbleToMakeABookingForUnitFromTo($unit_name, $start, $end) {
    if (!$this->findUnitAvailability($unit_name, $start, $end)) {
      throw new RuntimeException('Unable to book unit ' . $unit_name);
    }
  }

  /**
   * @Then /^I won\'t be able to make a booking for "(?P<unit_name>[^"]*)" unit from "(?P<start>[^"]*)" to "(?P<end>[^"]*)"$/
   */
  public function iWonTBeAbleToMakeABookingForUnitFromTo($unit_name, $start, $end) {
    if ($this->findUnitAvailability($unit_name, $start, $end)) {
      throw new RuntimeException('Able to book unit ' . $unit_name);
    }
  }

  /**
   * Retrieves the last booking ID.
   *
   * @return int
   *   The last booking ID.
   *
   * @throws RuntimeException
   */
  protected function getLastBooking() {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'rooms_booking')
      ->entityOrderBy('entity_id', 'DESC')
      ->range(0, 1);
    $result = $efq->execute();
    if (isset($result['rooms_booking'])) {
      $return = key($result['rooms_booking']);
      return $return;
    }
    else {
      throw new RuntimeException('Unable to find the last booking');
    }
  }

  /**
   * Checks if one unit is being locked by a booking in a date range.
   * @param $unit_name
   * @param $start_date
   * @param $end_date
   * @param $status
   */
  protected function checkUnitLockedByLastBooking($unit_name, $start_date, $end_date, $status) {
    $booking_id = $this->getLastBooking();
    $expected_value = rooms_availability_assign_id($booking_id, $status);
    $this->checkUnitPropertyRange($unit_name, $start_date, $end_date, $expected_value, 'availability');
  }

  /**
   * Adds options field to any room_unit or room_unit_type entity.
   *
   * @param TableNode $table
   *   Table containing options definitions.
   * @param $wrapper
   *   The entity wrapper to attach the options.
   */
  protected function addOptionsToEntity(TableNode $table, $wrapper) {
    $delta = 0;
    if (isset($wrapper->rooms_booking_unit_options)) {
      $delta = count($wrapper->rooms_booking_unit_options);
    }

    foreach ($table->getHash() as $entityHash) {
      $wrapper->rooms_booking_unit_options[$delta] = $entityHash;
      $delta++;
    }
    $wrapper->save();
  }

  /**
   * Fills the constraint range field form.
   *
   * @param $minimum
   * @param $maximum
   * @param $constraint_type
   * @param $start_day
   * @param $start
   * @param $end
   */
  protected function addAvailabilityConstraint($minimum = NULL, $maximum = NULL, $constraint_type = NULL, $start_day = NULL, $start = NULL, $end = NULL) {
    $items = $this->getSession()->getPage()->findAll('css', 'table[id^="rooms-constraints-range-values"] tbody tr');
    $delta = count($items) - 1;

    if (!isset($start) || !isset($end)) {
      $this->checkOption('rooms_constraints_range[und][' . $delta . '][always]');
    }
    else {
      $start_date = new DateTime($start);
      $end_date = new DateTime($end);
      $this->fillField('rooms_constraints_range[und][' . $delta . '][start_date][date]', $start_date->format('d/m/Y'));
      $this->fillField('rooms_constraints_range[und][' . $delta . '][end_date][date]', $end_date->format('d/m/Y'));
    }
    if (isset($constraint_type)){
      $this->selectOption('rooms_constraints_range[und][' . $delta . '][constraint_type]', $constraint_type);
    }
    if (isset($start_day)){
      $this->selectOption('rooms_constraints_range[und][' . $delta . '][start_day]', $start_day);
    }
    if (isset($minimum)){
      $this->fillField('rooms_constraints_range[und][' . $delta . '][minimum_stay]', $minimum);
    }
    if (isset($maximum)){
      $this->fillField('rooms_constraints_range[und][' . $delta . '][maximum_stay]', $maximum);
    }
    $this->pressButton('rooms_constraints_range_add_more');
    $this->iWaitForAjaxToFinish();
  }

  /**
   * @param $unit_name
   * @param $start
   * @param $end
   * @return bool
   */
  protected function findUnitAvailability($unit_name, $start, $end) {
    $unit_id = $this->findBookableUnitByName($unit_name);
    $start_date = new DateTime($start);
    $end_date = new DateTime($end);

    $agent = new AvailabilityAgent($start_date, $end_date);
    $units = $agent->checkAvailability();

    if (is_array($units)) {
      foreach ($units as $units_per_type) {
        foreach ($units_per_type as $units) {
          foreach ($units as $id => $unit) {
            if ($id == $unit_id) {
              return TRUE;
            }
          }
        }
      }
    }
    return FALSE;
  }

}

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
   * Keep track of bookable units so they can be cleaned up.
   *
   * @var array
   */
  public $unitTypes = array();

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
      $unit = rooms_unit_create($nodeHash);
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
   * @param $unit_name
   * @param $start_date
   * @param $end_date
   * @param $state
   * @param $type
   * @throws RuntimeException
   */
  protected function checkUnitPropertyRange($unit_name, $start_date, $end_date, $state, $type) {
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
        if ($event->id != $state) {
          throw new RuntimeException("The $type for unit $unit_name between $start_date and $end_date is not always $state");
        }
      }
    }
  }

  protected function monthsBetweenDates($start, $end) {
    $period_start = clone($start);
    $period_end = clone($end);
    $period_start->modify('first day of this month');
    $period_end->modify('first day of next month');
    $interval = DateInterval::createFromDateString('1 month');
    $period = new DatePeriod($period_start, $interval, $period_end);

    return $period;
  }

}

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

}

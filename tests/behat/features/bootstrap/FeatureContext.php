<?php

use Drupal\DrupalExtension\Context\DrupalContext,
  Drupal\Component\Utility\Random;

use Behat\Behat\Context\ClosuredContextInterface,
    Behat\Behat\Context\Step\Given,
    Behat\Behat\Context\Step\Then,
    Behat\Behat\Context\TranslatedContextInterface,
    Behat\Behat\Context\BehatContext,
    Behat\Behat\Exception\PendingException
  ;
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
class FeatureContext extends DrupalContext
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
   * Keep track of commerce products so they can be cleaned up.
   *
   * @var array
   */
  public $products = array();

  /**
   * Keep track of created content types so they can be cleaned up.
   *
   * @var array
   */
  public $content_types = array();

  /**
   * Keep track of created fields so they can be cleaned up.
   *
   * @var array
   */
  public $fields = array();

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
    foreach ($this->users as $user) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_order')
        ->propertyCondition('uid', $user->uid);
      $result = $query->execute();
      if (isset($result['commerce_order'])) {
        $order_ids = array_keys($result['commerce_order']);
        commerce_order_delete_multiple($order_ids);
      }
      $query2 = new EntityFieldQuery();
      $query2->entityCondition('entity_type', 'rooms_booking')
        ->propertyCondition('uid', $user->uid);
      $result = $query2->execute();
      if (isset($result['rooms_booking'])) {
        $booking_ids = array_keys($result['rooms_booking']);
        rooms_booking_delete_multiple($booking_ids);
      }
    }

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

    if (!empty($this->products)) {
      $product_ids = array();
      foreach ($this->products as $product) {
        $product_ids[] = $product->product_id;
      }
      commerce_product_delete_multiple($product_ids);
    }

    foreach ($this->content_types as $content_type) {
      node_type_delete($content_type);
    }

    foreach ($this->fields as $field) {
      field_delete_field($field);
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
   * Returns a nid from its content_type and title.
   *
   * @param $content_type
   * @param $title
   * @return int
   * @throws RuntimeException
   */
  protected function findNodeByTypeAndTitle($content_type, $title) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', $content_type)
      ->propertyCondition('title', $title);
    $results = $efq->execute();
    if ($results && isset($results['node'])) {
      return key($results['node']);
    }
    else {
      throw new RuntimeException('Unable to find that node');
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
   * @Given /^I select the "(?P<unit_name>[^"]*)" room for package$/
   */
  public function iSelectTheRoomForPackage($unit_name) {
    $unit_id = $this->findBookableUnitByName($unit_name);

    $text = $unit_name . ' (' . $unit_id . ')';
    $items = $this->getSession()->getPage()->findAll('css', 'table[id^="rooms-package-units-values"] tbody tr');
    $delta = count($items) - 1;
    $element_name = 'rooms_package_units[und][' . $delta . '][target_id]';
    $this->fillFieldByJS($element_name, $text);

    $this->pressButton('rooms_package_units_add_more');
    $this->iWaitForAjaxToFinish();
  }

  /**
   * @Then /^I should see the button "(?<button>[^"]*)"$/
   */
  public function iShouldSeeTheButton($button) {
    $element = $this->getSession()->getPage();
    $submit = $element->findButton($button);
    if (empty($submit)) {
      throw new \Exception(sprintf("No submit button at %s", $this->getSession()->getCurrentUrl()));
    }
  }

  /**
   * @Given /^I am viewing the package "(?<package_name>[^"]*)"$/
   */
  public function iAmViewingThePackage($package_name) {
    $nid = $this->findNodeByTypeAndTitle('rooms_package', $package_name);
    $this->getSession()->visit($this->locatePath('node/' . $nid));
  }


  /**
   * Fill the commerce shipping address form fields in a single step.
   *
   * @When /^I fill shipping address with "(?P<address>[^"]*)", "(?P<city>[^"]*)", "(?P<state>[^"]*)", "(?P<zip>[^"]*)", "(?P<country>[^"]*)"$/
   */
  public function fillShippingCommerceAddress($address, $city, $state, $zip, $country) {
    $args = func_get_args();
    return $this->fillCommerceAddress($args, 'customer_profile_shipping');
  }

  /**
   * Fill the commerce billing address form fields in a single step.
   *
   * @When /^I fill billing address with "(?P<address>[^"]*)", "(?P<city>[^"]*)", "(?P<state>[^"]*)", "(?P<zip>[^"]*)", "(?P<country>[^"]*)"$/
   */
  public function fillBillingCommerceAddress($address, $city, $state, $zip, $country) {
    $args = func_get_args();
    return $this->fillCommerceAddress($args, 'customer_profile_billing');
  }

  /**
   * Checks, that form button, field, radio with specified id|name|label|value is disabled.
   *
   * @Then /^the "(?P<select>(?:[^"]|\\")*)" (?P<type>(button)) is disabled$/
   * @Then /^the "(?P<select>(?:[^"]|\\")*)" (?P<type>(field)) is disabled$/
   * @Then /^the "(?P<select>(?:[^"]|\\")*)" (?P<type>(radio)) is disabled$/
   */
  public function assertFieldIsDisabled($select, $type) {
    switch ($type) {
      case 'button' :
        $element = $this->getSession()->getPage()->findButton($select);
        break;
      case 'field' :
        $element = $this->getSession()->getPage()->findField($select);
        break;
      case 'radio' :
        $element = $this->getSession()->getPage()->find('named', array(
          'radio', $this->getSession()->getSelectorsHandler()->xpathLiteral($select)
        ));
        break;
    }

    if (!isset($element) || !$element) {
      throw new \RuntimeException(sprintf("The %s '%s' was not found", $type, $select));
    }

    $disabled = $element->getAttribute('disabled');
    if (!$disabled) {
      throw new \RuntimeException(sprintf('The %s "%s" is enabled, but disabled expected.', $type, $select));
    }
  }

  /**
   * @Then /^I should see values in row table:$/
   */
  public function iShouldSeeValuesInTable(TableNode $nodesTable) {
    $page = $this->getSession()->getPage();
    $rows = $page->findAll('css', 'tr');
    if (!$rows) {
      throw new \Exception(sprintf('No rows found on the page %s', $this->getSession()->getCurrentUrl()));
    }

    foreach ($nodesTable->getHash() as $row_texts) {
      $found = TRUE;
      foreach ($rows as $row) {
        $found = TRUE;
        foreach ($row_texts as $row_text) {
          if (!empty($row_text) && strpos($row->getText(), $row_text) === FALSE) {
            $found = FALSE;
          }
        }
        if ($found) {
          break;
        }
      }
      if (!$found) {
        throw new \Exception(sprintf('Not found a row containing the desired texts'));
      }
    }
  }

  /**
   * Asserts that a given commerce_product type is editable.
   *
   * @Then /^I should be able to edit (?:a|an) "([^"]*)" product$/
   */
  public function assertEditProductOfType($type) {
    $product = commerce_product_new($type);
    $random = new Random();
    $product->title = $random->name();
    $product->sku = $random->name();
    commerce_product_save($product);
    $this->products[] = $product;

    // Set internal browser on the node edit page.
    $this->getSession()->visit($this->locatePath('/admin/commerce/products/' . $product->product_id . '/edit'));
  }

  /**
   * Adds availability reference field to a content type.
   *
   * @When /^I add the "(?<field_name>[^"]*)" availability reference field referencing to "(?<unit_types>[^"]*)" units in "(?<content_type>[^"]*)" content$/
   */
  public function iAddTheAvailabilityReferenceFieldReferencingToUnitsInPageContent($field_name, $unit_types, $content_type) {
    // Create the content type.
    // Make sure a testimonial content type doesn't already exist.
    if (!in_array($content_type, node_type_get_names())) {
      $type = array(
        'type' => $content_type,
        'name' => $content_type,
        'base' => 'node_content',
        'custom' => 1,
        'modified' => 1,
        'locked' => 0,
      );

      $type = node_type_set_defaults($type);
      node_type_save($type);
      node_add_body_field($type);
      $this->content_types[] = $content_type;
    }

    // Create field ('rooms_booking_unit_options') if not exist.
    if (field_read_field($field_name) === FALSE) {
      $field = array(
        'field_name' => $field_name,
        'type' => 'rooms_availability_reference',
        'cardinality' => -1,
        'settings' => array(
          'referenceable_unit_types' => drupal_map_assoc(explode(',', $unit_types)),
        ),
      );
      field_create_field($field);
      $this->fields[] = $field_name;
    }

    if (field_read_instance('node', $field_name, $content_type) === FALSE) {
      // Create the instance on the bundle.
      $instance = array(
        'field_name' => $field_name,
        'entity_type' => 'node',
        'label' => 'Availability reference',
        'bundle' => $content_type,
        'required' => FALSE,
        'widget' => array(
          'type' => 'rooms_availability_reference_autocomplete',
        )
      );
      field_create_instance($instance);
    }

  }

  /**
   * @Given /^reference units "(?<unit_names>[^"]*)" in the "(?<field_name>[^"]*)" field$/
   */
  public function referenceUnitsInTheField($unit_names, $field_name) {
    $table_id = drupal_clean_css_identifier($field_name . '-values');
    $items = $this->getSession()->getPage()->findAll('css', 'table[id^="' . $table_id . '"] tbody tr');
    $delta = count($items) - 1;

    foreach (explode(',', $unit_names) as $unit_name) {
      $unit_id = $this->findBookableUnitByName($unit_name);
      $this->fillFieldByJS('availability_ref[und][' . $delta . '][unit_id]', $unit_name. " [unit_id:$unit_id]");
      $this->pressButton($field_name . '_add_more');
      $this->iWaitForAjaxToFinish();
      $delta++;
    }
  }

  /**
   * @Then /^I navigate in the fullCalendar to "(?<month>[^"]*)"$/
   */
  public function iNavigateInTheFullcalendarTo($month) {
    $today = new DateTime();
    $final = new DateTime($month . '-1');
    $button_selector = ($today > $final) ? '.fc-button-prev' : '.fc-button-next';

    if ($today > $final) {
      $start = $final->add(new DateInterval('P2M'));;
      $end = $today;
    }
    else {
      $start = $today;
      $end = $final->sub(new DateInterval('P1M'));
    }
    foreach ($this->monthsBetweenDates($start, $end) as $month) {
      $element = $this->getSession()->getPage()->find('css', 'span' . $button_selector);
      if ($element === NULL) {
        throw new \InvalidArgumentException(sprintf('Cannot find button: "%s"', $button_selector));
      }
      $element->click();
      $this->iWaitForAjaxToFinish();
    }
  }


  /**
   * Asserts that a given node type is editable.
   */
  public function assertEditNodeOfType($type) {
    $node = (object) array('type' => $type);
    $saved = $this->getDriver()->createNode($node);
    $this->nodes[] = $saved;

    // Set internal browser on the node edit page.
    $this->getSession()->visit($this->locatePath('/node/' . $saved->nid . '/edit'));
  }

    /**
   * Fill commerce address form fields in a single step.
   */
  private function fillCommerceAddress($args, $type) {
    // Replace <random> or member <property> token if is set for any field
    foreach ($args as $delta => $arg) {
      if (preg_match("/^<random>$/", $arg, $matches)) {
        $random = new Random();
        $args[$delta] = $random->name();
      }
    }

    // Need to manually fill country to trigger the AJAX refresh of fields for given country
    $country_field = $this->fixStepArgument("{$type}[commerce_customer_address][und][0][country]");
    $country_value = $this->fixStepArgument($args[4]);
    $this->getSession()->getPage()->fillField($country_field, $country_value);
    $this->iWaitForAjaxToFinish();

    return array(
      new Given("I fill in \"{$type}[commerce_customer_address][und][0][locality]\" with \"$args[1]\""),
      new Given("I fill in \"{$type}[commerce_customer_address][und][0][administrative_area]\" with \"$args[2]\""),
      new Given("I fill in \"{$type}[commerce_customer_address][und][0][postal_code]\" with \"$args[3]\""),
      new Given("I fill in \"{$type}[commerce_customer_address][und][0][thoroughfare]\" with \"$args[0]\""),
    );
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

  /**
   * Fills a field using JS to avoid event firing.
   * @param string $field
   * @param string$value
   *
   */
  protected function fillFieldByJS($field, $value) {
    $field = $this->fixStepArgument($field);
    $value = $this->fixStepArgument($value);
    $xpath = $this->getSession()->getPage()->findField($field)->getXpath();

    $element = $this->getSession()->getDriver()->getWebDriverSession()->element('xpath', $xpath);
    $elementID = $element->getID();
    $subscript = "arguments[0]";
    $script = str_replace('{{ELEMENT}}', $subscript, '{{ELEMENT}}.value = "' . $value . '"');
    return $this->getSession()->getDriver()->getWebDriverSession()->execute(array(
      'script' => $script,
      'args' => array(array('ELEMENT' => $elementID))
    ));
  }

}

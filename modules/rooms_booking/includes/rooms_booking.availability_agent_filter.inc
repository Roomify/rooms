<?php

/**
 * @file
 * Rooms Booking agent filter interfaces and base implementations.
 */

/**
 * An availability agent filter receives a set of units and applies a filter
 * to them returning the remainder.
 */
interface AvailabilityAgentFilterInterface {

  /**
   * Applies the filter operation to the units in the filter.
   *
   * @return array|int
   *   Rooms remaining after the filter, error code otherwise.
   */
  public function applyFilter();

  /**
   * Returns a list of parameters to add to the search array.
   *
   * @return array
   *   List of parameters provided by this filter.
   */
  public static function availabilitySearchParameters();

  /**
   * Adds necessary form elements to Availability search form.
   *
   * @param array $form
   *   The Availability search form array.
   * @param array $form_state
   *   The Availability search form state array.
   */
  public static function availabilitySearchForm(&$form, &$form_state);

  /**
   * Specific validation callback for Availability search form.
   *
   * @param array $form
   *   The Availability search form array.
   * @param array $form_state
   *   The Availability search form state array.
   */
  public static function availabilitySearchFormValidate(&$form, &$form_state);

  /**
   * Adds necessary form elements to Change availability search form.
   *
   * @param array $form
   *   The Change availability search form array.
   * @param array $form_state
   *   The Change availability search form state array.
   */
  public static function availabilityChangeSearchForm(&$form, &$form_state);

  /**
   * Specific validation callback for Change availability search form.
   *
   * @param array $form
   *   The Change availability search form array.
   * @param array $form_state
   *   The Change availability search form state array.
   */
  public static function availabilityChangeSearchFormValidate(&$form, &$form_state);

}

/**
 * Abstract class implementing AvailabilityAgentFilterInterface.
 */
abstract class AvailabilityAgentFilterBase implements AvailabilityAgentFilterInterface {

  /**
   * Set of bookable units to filter through.
   *
   * @var array
   */
  protected $units;

  /**
   * Set of filter parameters.
   *
   * @var array
   */
  protected $parameters;

  /**
   * Builds a new AvailabilityAgentFilter object.
   *
   * @param array $units
   *   Set of bookable units to filter through.
   * @param array $parameters
   *   Set of filter parameters.
   */
  public function __construct(array $units, array $parameters) {
    $this->units = $units;
    $this->parameters = $parameters;
  }

  /**
   * Intersects the units that passes the filter and the unit set provided.
   *
   * @param array $filtered_units
   *   The rooms_units that passes the current filter.
   *
   * @return array
   *   The intersection of filtered units with the provided set.
   */
  protected function intersectUnits($filtered_units) {
    $filtered_keys = array_keys($filtered_units);
    $unit_keys = array_keys($this->units);

    $keys_units = array_intersect($filtered_keys, $unit_keys);

    $results = array();

    foreach ($keys_units as $key) {
      $results[$key] = $this->units[$key];
    }

    return $results;
  }

  /**
   * {@inheritdoc}
   */
  public static function availabilitySearchParameters() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public static function availabilitySearchForm(&$form, &$form_state) { }

  /**
   * {@inheritdoc}
   */
  public static function availabilitySearchFormValidate(&$form, &$form_state) { }

  /**
   * {@inheritdoc}
   */
  public static function availabilityChangeSearchForm(&$form, &$form_state) { }

  /**
   * {@inheritdoc}
   */
  public static function availabilityChangeSearchFormValidate(&$form, &$form_state) { }

}

/**
 * Filter by group_size and group_size_children.
 */
class AvailabilityAgentSizeFilter extends AvailabilityAgentFilterBase {

  /**
   * {@inheritdoc}
   */
  public function applyFilter() {

    // Check parameters.
    $group_size = isset($this->parameters['group_size']) ? $this->parameters['group_size'] : 0;
    $group_size_children = isset($this->parameters['group_size_children']) ? $this->parameters['group_size_children'] : 0;
    $unit_types = isset($this->parameters['unit_types']) ? $this->parameters['unit_types'] : NULL;

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'rooms_unit')
      ->propertyOrderBy('max_sleeps', 'ASC')
      ->propertyCondition('max_sleeps', $group_size, '>=')
      ->propertyCondition('max_children', $group_size_children, '>=')
      ->propertyCondition('bookable', 1);

    // Add a unit type condition if this has been defined.
    if ($unit_types != NULL) {
      $query->propertyCondition('type', $unit_types);
    }

    // Execute the query and collect the results.
    $results = $query->execute();

    if (count($results) == 0) {
      return ROOMS_SIZE_FAILURE;
    }

    if (empty($this->units)) {
      return $results['rooms_unit'];
    }
    else {
      // Computes the intersection of units and results.
      return $this->intersectUnits($results['rooms_unit']);
    }
  }

}

/**
 * Filter by start_date, end_date, valid_states.
 */
class AvailabilityAgentDateFilter extends AvailabilityAgentFilterBase {

  /**
   * {@inheritdoc}
   */
  public function applyFilter() {

    // Check parameters.
    $start_date = isset($this->parameters['start_date']) ? $this->parameters['start_date'] : NULL;
    $end_date = isset($this->parameters['end_date']) ? $this->parameters['end_date'] : NULL;
    $confirmed = isset($this->parameters['confirmed']) ? $this->parameters['confirmed'] : FALSE;

    // Start date and end date parameters must be set.
    if ($start_date == NULL || $end_date == NULL) {
      return $this->units;
    }

    if (isset($this->parameters['valid_states'])) {
      $valid_states = $this->parameters['valid_states'];
    }
    else {
      $valid_states = array_keys(array_filter(variable_get('rooms_valid_availability_states', drupal_map_assoc(array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST)))));
      $valid_states = array_merge($valid_states, array(ROOMS_UNCONFIRMED_BOOKINGS));
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'rooms_unit')
      ->propertyOrderBy('max_sleeps', 'ASC')
      ->propertyCondition('bookable', 1);

    // Execute the query and collect the results.
    $results = $query->execute();

    foreach ($results['rooms_unit'] as $key => $unit) {
      $unit = rooms_unit_load($unit->unit_id);

      // Get a calendar and check availability.
      $rc = new UnitCalendar($unit->unit_id);
      // We need to make this based on user-set vars.
      // Rather than using $rc->stateAvailability we will get the states check
      // directly as different states will impact on what products we create.
      $states = $rc->getStates($start_date, $end_date, $confirmed);
      $state_diff = array_diff($states, $valid_states);

      if (count($state_diff) != 0) {
        unset($results['rooms_unit'][$key]);
      }
    }

    if (empty($this->units)) {
      return $results['rooms_unit'];
    }
    else {
      // Computes the intersection of units and results.
      return $this->intersectUnits($results['rooms_unit']);
    }

  }

}

/**
 * Filter units by unit id.
 */
class AvailabilityAgentSingleUnitFilter extends AvailabilityAgentFilterBase {

  public function applyFilter() {
    if (variable_get('rooms_presentation_style') == ROOMS_INDIVIDUAL && isset($_GET['rooms_id']) && $requested_unit = rooms_unit_load($_GET['rooms_id'])) {

      foreach ($this->units as $unit) {
        if ($unit->unit_id != $requested_unit->unit_id) {
          unset($this->units[$unit->unit_id]);
        }
      }
      if (empty($this->units)) {
        drupal_set_message('Unfortunately ' . $requested_unit->name . ' is not available - try other dates if possible', 'warning');
      }

    }
    return $this->units;
  }
}

/**
 * Filter units if is in the commerce cart.
 */
class AvailabilityAgentCommerceFilter extends AvailabilityAgentFilterBase {

  /**
   * {@inheritdoc}
   */
  public function applyFilter() {

    global $user;

    // Check parameters.
    $start_date = isset($this->parameters['start_date']) ? $this->parameters['start_date'] : date_create()->setTimestamp(0);
    $end_date = isset($this->parameters['end_date']) ? $this->parameters['end_date'] : date_create()->setTimestamp(2147483647);

    if (empty($this->units)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'rooms_unit')
        ->propertyOrderBy('max_sleeps', 'ASC')
        ->propertyCondition('bookable', 1);

      // Execute the query and collect the results.
      $results = $query->execute();
      if (count($results)) {
        $results = $results['rooms_unit'];
      }
    }
    else {
      $results = $this->units;
    }

    // Load all the current carts to
    $orders = array();
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'commerce_order')
      ->propertyCondition('status', array('cart', 'checkout_checkout', 'checkout_review', 'checkout_payment', 'checkout_complete'))
      ->execute();
    if (isset($result['commerce_order'])) {
      $orders = commerce_order_load_multiple(array_keys($result['commerce_order']));
    }

    foreach ($orders as $order) {
      $wrapper = entity_metadata_wrapper('commerce_order', $order);

      // See if there are any product line items.
      if (commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types()) > 0) {
        // Get the unit IDs already in the cart.
        foreach ($wrapper->commerce_line_items as $line_item) {
          // There are some line_item types as: coupon, fee, etc. that don't
          // have a referenced commerce_product.
          if (in_array($line_item->type->value(), commerce_product_line_item_types())) {
            // Need to check if commerce_product_type = 'rooms_product'.
            if ($line_item->commerce_product->value()->type == 'rooms_product') {
              $line_item_values = $line_item->value();

              // Current line_item start_date and end_date values.
              $s_date = new DateTime($line_item_values->rooms_booking_dates[LANGUAGE_NONE][0]['value']);
              $e_date = new DateTime($line_item_values->rooms_booking_dates[LANGUAGE_NONE][0]['value2']);

              // If current line_item date interval overlap parameters interval
              // remove unit from results.
              if ($this->checkInRange($s_date, $e_date, $start_date, $end_date)) {
                if (isset($results[$line_item_values->rooms_booked_unit_id[LANGUAGE_NONE][0]['value']])) {
                  unset($results[$line_item_values->rooms_booked_unit_id[LANGUAGE_NONE][0]['value']]);
                }
              }
            }
          }
        }
      }
    }

    if (empty($this->units)) {
      return $results;
    }
    else {
      // Computes the intersection of units and results.
      return $this->intersectUnits($results);
    }
  }

  /**
   * Checks if search interval overlaps line_item interval.
   *
   * @param DateTime $line_item_start_date
   *   Line item start date.
   * @param DateTime $line_item_end_date
   *   Line item end date.
   * @param DateTime $start_date
   *   Interval start date.
   * @param DateTime $end_date
   *   Interval end date.
   *
   * @return bool
   *   TRUE in case of overlapping, FALSE otherwise.
   */
  private function checkInRange($line_item_start_date, $line_item_end_date, $start_date, $end_date) {
    if (!(($line_item_start_date < $start_date && $line_item_end_date < $start_date) ||
      ($line_item_start_date > $end_date && $line_item_end_date > $end_date))) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

}

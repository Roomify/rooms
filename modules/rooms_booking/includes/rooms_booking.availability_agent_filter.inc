<?php


/**
 * An availability agent filter receives a set of units and applies a filter
 * to them returning the remainder.
 */
interface AvailabilityAgentFilter {

  /**
   * @param array rooms The set of rooms to filter through
   * @param array parameters A set of filter parameters
   */
  public function __construct($units, $parameters);

  /**
   * @return array The rooms remaining following the application of the filter
   */
  public function applyFilter();

}


/**
 * Abstract class implementing AvailabilityAgentFilter that expose common functions.
 */
abstract class AvailabilityAgentCommonFilter implements AvailabilityAgentFilter {

  // Returns an array containing all the values of units_a that are present in units_b
  function array_units_intersect($units_a, $units_b) {
    $keys_units_a = array_keys($units_b['rooms_unit']);
    $keys_units_b = array_keys($units_a['rooms_unit']);

    $keys_units = array_intersect($keys_units_a, $keys_units_b);

    $results = array();

    foreach ($keys_units as $key) {
      $results['rooms_unit'][$key] = $units_a['rooms_unit'][$key];
    }

    return $results;
  }

}


/**
 * Filter by group_size and group_size_children.
 */
class AvailabilityAgentSizeFilter extends AvailabilityAgentCommonFilter {

  protected $units;

  protected $parameters;

  public function __construct($units, $parameters) {
    $this->units = $units;
    $this->parameters = $parameters;
  }

  public function applyFilter() {

    // Check parameters
    $group_size = isset($this->parameters['group_size']) ? $this->parameters['group_size'] : 0;
    $group_size_children = isset($this->parameters['group_size_children']) ? $this->parameters['group_size_children'] : 0;
    $unit_types = isset($this->parameters['unit_types']) ? $this->parameters['unit_types'] : NULL;

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'rooms_unit')
          ->propertyOrderBy('max_sleeps', 'ASC')
          ->propertyCondition('max_sleeps', $group_size, '>=')
          ->propertyCondition('max_children', $group_size_children, '>=')
          ->propertyCondition('bookable', 1);

    // Add a unit type condition if this has been defined
    if ($unit_types != NULL) {
      $query->propertyCondition('type', $unit_types[0]);
    }

    // Execute the query and collect the results
    $results = $query->execute();

    if (count($results) == 0) {
      return ROOMS_SIZE_FAILURE;
    }

    if (empty($this->units)) {
      return $results;
    }
    else {
      // Computes the intersection of units and results
      return $this->array_units_intersect($this->units, $results);
    }

  }

}


/**
 * Filter by start_date, end_date, valid_states.
 */
class AvailabilityAgentDateFilter extends AvailabilityAgentCommonFilter {

  protected $units;

  protected $parameters;

  public function __construct($units, $parameters) {
    $this->units = $units;
    $this->parameters = $parameters;
  }

  public function applyFilter() {

    // Check parameters
    $start_date = isset($this->parameters['start_date']) ? $this->parameters['start_date'] : NULL;
    $end_date = isset($this->parameters['end_date']) ? $this->parameters['end_date'] : NULL;
    $confirmed = isset($this->parameters['confirmed']) ? $this->parameters['confirmed'] : FALSE;

    // Stard date and end date parameters must be set
    if ($start_date == NULL || $end_date == NULL) return $this->units;

    if (isset($this->parameters['valid_states'])) {
      $valid_states = $this->parameters['valid_states'];
    }
    else {
      $valid_states = array_keys(array_filter(variable_get('rooms_valid_availability_states', array(ROOMS_AVAILABLE => ROOMS_AVAILABLE, ROOMS_ON_REQUEST => ROOMS_ON_REQUEST))));
      $valid_states = array_merge($valid_states, array(ROOMS_UNCONFIRMED_BOOKINGS));
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'rooms_unit')
            ->propertyOrderBy('max_sleeps', 'ASC')
            ->propertyCondition('bookable', 1);

    // Execute the query and collect the results
    $results = $query->execute();

    foreach ($results['rooms_unit'] as $key => $unit) {
      $unit = rooms_unit_load($unit->unit_id);

      // Get a calendar and check availability
      $rc = new UnitCalendar($unit->unit_id);
      // We need to make this based on user-set vars

      // Rather than using $rc->stateAvailability we will get the states check directly
      // as different states will impact on what products we create.
      $states = $rc->getStates($start_date, $end_date, $confirmed);
      $state_diff = array_diff($states, $valid_states);

      if (count($state_diff) != 0) {
        unset($results['rooms_unit'][$key]);
      }
    }

    if (empty($this->units)) {
      return $results;
    }
    else {
      // Computes the intersection of units and results
      return $this->array_units_intersect($this->units, $results);
    }

  }

}


/**
 * Filter units if is in the commerce cart.
 */
class AvailabilityAgentCommerceFilter extends AvailabilityAgentCommonFilter {

  protected $units;

  protected $parameters;

  public function __construct($units, $parameters) {
    $this->units = $units;
    $this->parameters = $parameters;
  }

  public function applyFilter() {

    global $user;

    // Check parameters
    $start_date = isset($this->parameters['start_date']) ? $this->parameters['start_date'] : date_create()->setTimestamp(0);
    $end_date = isset($this->parameters['end_date']) ? $this->parameters['end_date'] : date_create()->setTimestamp(2147483647);

    if (empty($this->units)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'rooms_unit')
            ->propertyOrderBy('max_sleeps', 'ASC')
            ->propertyCondition('bookable', 1);

      // Execute the query and collect the results
      $results = $query->execute();
    }
    else {
      $results = $this->units;
    }

    // Load order for current user
    if ($order = commerce_cart_order_load($user->uid)) {
      $wrapper = entity_metadata_wrapper('commerce_order', $order);

      // See if there are any product line items.
      if (commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types()) > 0) {
        // Get the unit IDs already in the cart.
        foreach ($wrapper->commerce_line_items as $line_item) {
          // Need to check if commerce_product_type = 'rooms_product'
          if ($line_item->commerce_product->value()->type == 'rooms_product') {
            $line_item_values = $line_item->value();

            // Current line_item start_date and end_date values
            $s_date = new DateTime($line_item_values->rooms_booking_dates[LANGUAGE_NONE][0]['value']);
            $e_date = new DateTime($line_item_values->rooms_booking_dates[LANGUAGE_NONE][0]['value2']);

            // If current line_item date interval overlap parameters interval remove unit from results
            if ( $this->check_in_range($s_date, $e_date, $start_date, $end_date) ) {
              if (isset($results['rooms_unit'][$line_item_values->rooms_booked_unit_id[LANGUAGE_NONE][0]['value']])) {
                unset($results['rooms_unit'][$line_item_values->rooms_booked_unit_id[LANGUAGE_NONE][0]['value']]);
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
      // Computes the intersection of units and results
      return $this->array_units_intersect($this->units, $results);
    }
  }

  // Check if search interval overlaps line_item interval
  private function check_in_range($line_item_start_date, $line_item_end_date, $start_date, $end_date) {
    if (!(($line_item_start_date < $start_date && $line_item_end_date < $start_date) ||
          ($line_item_start_date > $end_date && $line_item_end_date > $end_date))) {

      return TRUE;

    }
    else {

      return FALSE;

    }
  }

}

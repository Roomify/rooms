<?php

/**
 * @file
 * Rooms editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring
 * instead to use views. That offers more flexibility to change a UI that will,
 * more often than not, be end-user facing.
 */

/**
 * UI controller.
 */
class RoomsBookingUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   *
   * Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Bookings',
      'description' => 'Add edit and update bookings.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      'weight' => 10,
    );

    // Change the add page menu to multiple types of entities.
    $items[$this->path . '/add'] = array(
      'title' => 'Add a Booking',
      'description' => 'Add a new Booking',
      'page callback'  => 'rooms_booking_add_page',
      'access callback'  => '_rooms_booking_add_access',
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    // Add menu items to add each different type of room.
    foreach (rooms_booking_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add @booking_type',
        'title arguments' => array('@booking_type' => $type->label),
        'page callback' => 'rooms_booking_create_form_wrapper',
        'page arguments' => array($type->type),
        'access callback' => 'rooms_booking_access',
        'access arguments' => array('create', rooms_booking_create(array('type' => $type->type, 'uid' => 0))),
        'file' => 'rooms_booking.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module']),
      );
    }

    // Loading and editing Rooms entities.
    $items[$this->path . '/booking/' . $wildcard] = array(
      'page callback' => 'rooms_booking_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_booking_access',
      'access arguments' => array('update', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );
    $items[$this->path . '/booking/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/booking/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'rooms_booking_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_booking_access',
      'access arguments' => array('delete', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    // Menu item for viewing rooms.
    $items['booking/' . $wildcard] = array(
      'title callback' => 'rooms_booking_page_title',
      'title arguments' => array(1),
      'page callback' => 'rooms_booking_page_view',
      'page arguments' => array(1),
      'access callback' => 'rooms_booking_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Booking Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return array(
      '#theme' => 'rooms_booking_add_list',
      '#content' => $content,
    );
  }

}

/**
 * Form callback wrapper: edit a Booking.
 *
 * @param $booking
 *   The  RoomsBooking object being edited by this form.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_form_wrapper($booking) {
  // Add the breadcrumb for the form's location.
  rooms_booking_set_breadcrumb();

  // Set the page title.
  $type = rooms_booking_get_types($booking->type);
  drupal_set_title(t('#@booking_id - @booking_type', array('@booking_type' => $type->label, '@booking_id' => $booking->booking_id)));
  $booking->date = format_date($booking->created, 'custom', 'Y-m-d H:i:s O');
  $account = user_load($booking->uid);
  $booking->owner_name = $account->name;

  return drupal_get_form('rooms_booking_edit_form', $booking);
}

/**
 * Form callback wrapper: create a Booking.
 *
 * @param $booking
 *   The Booking object being edited by this form.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_create_form_wrapper($type) {
  global $user;
  // Add the breadcrumb for the form's location.
  rooms_booking_set_breadcrumb();
  // Create a booking object.
  $booking = rooms_booking_create(array('type' => $type));
  $booking->created = REQUEST_TIME;
  $booking->owner_name = $user->name;
  return drupal_get_form('rooms_booking_edit_form', $booking);
}

/**
 * Form callback wrapper: delete a booking.
 *
 * @param $booking
 *   The booking object being edited by this form.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_delete_form_wrapper($booking) {
  // Add the breadcrumb for the form's location.
  rooms_booking_set_breadcrumb();
  return drupal_get_form('rooms_booking_delete_form', $booking);
}

/**
 * AJAX callback to update "edit profile" link after Customer field changes.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_edit_profile_callback(&$form, $form_state) {
  if (module_exists('commerce_customer')) {
    $client = explode(':', $form_state['values']['client']);

    if (isset($client[1])) {
      $result = db_select('field_data_commerce_customer_address')
        ->fields('field_data_commerce_customer_address', array('entity_id'))
        ->condition('commerce_customer_address_name_line', $client[0])
        ->condition('entity_id', $client[1])
        ->execute()
        ->fetchField();

      if ($result !== FALSE) {
        $form['client']['#field_suffix'] = t('<a href="@edit-profile" class="ctools-use-modal">edit profile</a>', array('@edit-profile' => url('admin/rooms/customer-profiles/' . $client[1] . '/edit')));
      }
      else {
        $form['client']['#field_suffix'] = '';
      }
    }
    else {
      $form['client']['#field_suffix'] = '';
    }
  }

  return render($form['client']);
}

/**
 * Form callback: create or edit a booking.
 *
 * @param $booking
 *   The RoomBooking object to edit or for a create form an empty booking object
 *     with only a booking type defined.
 */
function rooms_booking_edit_form($form, &$form_state, $booking) {
  $form['#attributes']['class'][] = 'rooms-management-form rooms-booking-form';
  $form['#attached'] = array(
    'css' => array(drupal_get_path('module', 'rooms_booking') . '/css/rooms_booking.css'),
    'js' => array(drupal_get_path('module', 'rooms') . '/js/rooms_date_popup.js'),
  );

  $client = '';

  $currency_setting = commerce_currency_load(commerce_default_currency());
  $currency_symbol = $currency_setting['symbol'];

  // Check to see if we are loading an existing booking (existing bookings
  // have customer ids set).
  if (isset($booking->customer_id)) {
    // The Client value needs the id as well to take us back to the Customer
    // Profile in Commerce.
    $client = $booking->name . ':' . $booking->customer_id;
  }

  $form['client'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'admin/rooms/bookings/customers',
    '#default_value' => $client,
    '#weight' => -1,
    '#required' => TRUE,
    '#prefix' => '<div class="rooms-booking-customer-wrapper" id="edit-customer-wrapper">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'callback' => 'rooms_booking_edit_profile_callback',
      'wrapper' => 'edit-customer-wrapper',
    ),
  );

  if (module_exists('commerce_customer')) {
    ctools_include('modal');
    ctools_modal_add_js();

    $form['client']['#description'] = t('Customer profiles are saved in the <a href="@store-profile">store</a>. Search for an existing one by typing the customer name or <a href="@profile-create" class="ctools-use-modal">create a new profile</a>.', array('@store-profile' => url('admin/commerce/customer-profiles'), '@profile-create' => url('admin/rooms/add_customers')));

    if (isset($booking->customer_id)) {
      $commerce_customer_id = db_select('rooms_customers')
        ->fields('rooms_customers', array('commerce_customer_id'))
        ->condition('id', $booking->customer_id, '=')
        ->execute()->fetchField();

      if ($commerce_customer_id != NULL) {
        $form['client']['#field_suffix'] = t('<a href="@edit-profile" class="ctools-use-modal">edit profile</a>', array('@edit-profile' => url('admin/rooms/customer-profiles/' . $commerce_customer_id . '/edit')));
      }
    }
  }

  $form['data']['#tree'] = TRUE;
  $form['data']['group_size'] = array(
    '#type' => 'select',
    '#title' => t('Guests'),
    '#options' => rooms_assoc_range(1, 10),
    '#default_value' => isset($booking->data['group_size']) ? $booking->data['group_size'] : '2',
    '#description' => t('The total number of people staying in this unit (including adults and children).'),
    '#ajax' => array(
      'callback' => 'rooms_booking_group_size_ajax_callback',
      'wrapper' => 'rooms_booking_group_size_children_container',
    ),
  );

  $persons = 2;
  if (isset($form_state['values']['data']['group_size'])) {
    $persons = $form_state['values']['data']['group_size'];
  }

  $form['data']['group_size_children'] = array(
    '#type' => 'select',
    '#title' => t('Children'),
    '#options' => rooms_range(0, $persons),
    '#default_value' => isset($booking->data['group_size_children']) ? $booking->data['group_size_children'] : '0',
    '#description' => t('The number of children staying in this unit.'),
    '#prefix' => '<div class="rooms-booking-group-size-children-container" id="rooms_booking_group_size_children_container">',
    '#suffix' => '</div>',
  );

  // If price calculation is set to 'Price per person per night'.
  if (variable_get('rooms_price_calculation', ROOMS_PER_NIGHT) == ROOMS_PER_PERSON) {
    // Show fields for enter children ages.
    $form['data']['group_size_children']['#ajax'] = array(
      'callback' => 'rooms_booking_children_change_callback',
      'wrapper' => 'rooms_booking_childrensage',
    );

    $form['data']['childrens_age'] = array(
      '#prefix' => '<div class="container-inline rooms-booking-childrensage" id="rooms_booking_childrensage">',
      '#suffix' => '</div>',
    );

    if (isset($form_state['values']['data']['group_size_children'])) {
      if ($persons >= $form_state['values']['data']['group_size_children']) {
        for ($t = 1; $t <= $form_state['values']['data']['group_size_children']; $t++) {
          $form['data']['childrens_age'][$t] = array(
            '#type' => 'textfield',
            '#title' => t('Age of child @num', array('@num' => $t)),
            '#size' => 5,
            '#maxlength' => 5,
            '#ajax' => array(
              'callback' => 'rooms_booking_childrens_age_ajax_callback',
              'event' => 'blur',
            ),
          );
        }
      }
    }
    elseif (!empty($booking->data['childrens_age'])) {
      foreach ($booking->data['childrens_age'] as $key => $age) {
        $form['data']['childrens_age'][$key] = array(
          '#type' => 'textfield',
          '#title' => t('Age of child @num', array('@num' => $key)),
          '#size' => 5,
          '#maxlength' => 5,
          '#default_value' => $age,
          '#ajax' => array(
            'callback' => 'rooms_booking_childrens_age_ajax_callback',
            'event' => 'blur',
          ),
        );
      }
    }
  }

  // A fieldset to hold the date range fields.
  $form['rooms_date_range'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('rooms-booking-date-range-wrapper')),
  );

  $form['rooms_date_range'] += rooms_date_range_fields();

  // Unset the default for max days away for bookings since we are on the admin.
  drupal_add_js(array('rooms' => array('roomsBookingStartDay' => 0)), 'setting');

  // Set the default values for the dates.
  $form['rooms_date_range']['rooms_start_date']['#default_value'] = isset($booking->start_date) ? $booking->start_date : '';
  $form['rooms_date_range']['rooms_end_date']['#default_value'] = isset($booking->end_date) ? $booking->end_date : '';

  // Check startdate and enddate to avoid damage from dirty input.
  $startdate = '';
  if (isset($_GET['startdate'])) {
    $startdate = (is_numeric(check_plain($_GET['startdate']))) ? check_plain($_GET['startdate']) : '';
    if ($startdate != '') $form['rooms_date_range']['rooms_start_date']['#default_value'] = gmdate('Y-m-d', $startdate);
  }
  $enddate = '';
  if (isset($_GET['enddate'])) {
    $enddate = (is_numeric(check_plain($_GET['enddate']))) ? check_plain($_GET['enddate']) : '';
    if ($enddate != '') {
      $form['rooms_date_range']['rooms_end_date']['#default_value'] = gmdate('Y-m-d', $enddate);
    }
  }

  $unitid = '';
  if (isset($_GET['unitid'])) {
    $unitid = (is_numeric(check_plain($_GET['unitid']))) ? check_plain($_GET['unitid']) : '';

    if (rooms_unit_load($unitid) == NULL || $startdate == '' || $enddate == '') {
      $unitid = '';
    }
  }

  // Access to availability information.
  $form['get_availability'] = array(
    '#type' => 'submit',
    '#value' => isset($booking->unit_id) ? t('Re-assign Unit') : t('Check availability'),
    '#submit' => array('rooms_booking_edit_form_availability_submit'),
    '#validate' => array('rooms_form_start_end_dates_validate'),
    '#ajax' => array(
      'callback' => 'rooms_booking_edit_form_availability_callback',
      'wrapper' => 'availability-wrapper',
    ),
    '#prefix' => '<div class="rooms-booking-availability-button-wrapper" id="availability-button">',
    '#suffix' => '</div>',
  );

  $options_list = '';

  // If a unit is assigned get the unit type label.
  if (isset($booking->unit_id)) {
    $unit = rooms_unit_load($booking->unit_id);
    $type_obj = rooms_unit_type_load($unit->type);

    foreach (rooms_unit_get_unit_options($unit) as $option) {
      $option_name = strtolower(str_replace(array(':', ' '), '_', $option['name']));
      if (isset($booking->data[$option_name])) {
        if (!isset($form_state['values'][$option_name])) {
          $form_state['values'][$option_name] = isset($booking->data[$option_name]);
          $form['data'][$option_name] = array(
            '#type' => 'hidden',
            '#value' => isset($booking->data[$option_name]),
          );
        }

        $quantity = 1;
        if (!isset($form_state['values'][$option_name . ':quantity']) && isset($booking->data[$option_name . ':quantity'])) {
          $form_state['values'][$option_name . ':quantity'] = $booking->data[$option_name . ':quantity'];
          $form['data'][$option_name . ':quantity'] = array(
            '#type' => 'hidden',
            '#value' => $booking->data[$option_name . ':quantity'],
          );
        }

        if (isset($form_state['values'][$option_name . ':quantity']) && $option['operation'] != ROOMS_REPLACE) {
          $quantity = $form_state['values'][$option_name . ':quantity'] + 1;
        }
        $options_list .= $option['name'] . ' : ' . $quantity . ', ';
      }
    }

    $form['unit_id'] = array(
      '#type' => 'hidden',
      '#value' => $booking->unit_id,
    );

  }
  $unit_selected = isset($booking->unit_id) ? t('Currently assigned unit: @unit_name / @type', array('@type' => $type_obj->label, '@unit_name' => $unit->name)) : '';

  if ($options_list != '') {
    $unit_selected .= t('(Options: !options)', array('!options' => substr($options_list, 0, strlen($options_list) - 2)));
  }
  if (isset($booking->price)) {
    $unit_selected .= '<br />' . t('Price') . ': ' .
    t('@currency_symbol @amount', array( // TODO: replace with a proper currency formatter
      '@currency_symbol' => $currency_symbol,
      '@amount' => number_format($booking->price / 100, 2, '.', ''),
    ));
  }

  $form['availability_fieldset'] = array(
    '#type' => 'fieldset',
    '#description' => $unit_selected,
    '#prefix' => '<div class="rooms-booking-availability-wrapper" id="availability-wrapper">',
    '#suffix' => '</div>',
  );

  $form['b_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Booking status'),
    '#attributes' => array('class' => array('rooms-booking-status-wrapper')),
  );
  $form['b_status']['booking_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Booking Confirmed'),
    '#options' => array(
      '0' => '0',
      '1' => '1',
    ),
    '#default_value' => isset($booking->booking_status) ? $booking->booking_status : '0',
  );

  if ($booking->booking_id != '') {
    if ($booking->order_id != '') {
      $order = commerce_order_load($booking->order_id);
      $order_states = commerce_order_statuses();

      if ($order !== FALSE) {
        $form['order'] = array(
          '#type' => 'fieldset',
          '#title' => t('Order'),
          '#attributes' => array('class' => array('rooms-booking-order-wrapper')),
        );
        $form['order']['order_link'] = array(
          '#type' => 'markup',
          '#markup' => t('Order status: @order_status', array('@order_status' => $order_states[$order->status]['title'])) . '<br />' . t('Edit <a href="@url">order</a> associated with this booking', array('@url' => url('admin/commerce/orders/' . $booking->order_id . '/edit'))),
        );
      }
    }
  }

  // If submit above pressed and we have dates we can process to assign a unit.
  if (isset($form_state['show_availability_options']) || $unitid != '') {
    $types = rooms_booking_edit_form_get_room_types();

    // Figure out which is the current unit and mark that as selected.
    $selected = '';
    if (isset($form_state['values']['unit_type'])) {
      $selected = $form_state['values']['unit_type'];
    }
    elseif (isset($booking->unit_type)) {
      $selected = $booking->unit_type;
    }
    elseif ($unitid != '') {
      $unit = rooms_unit_load($unitid);
      $selected = $unit->type;

      if ($unit->max_sleeps <= 1) {
        $form['data']['group_size']['#default_value'] = 0;
      }

      $form_state['values']['unit_type'] = $selected;
      $form_state['input']['data']['group_size'] = ($unit->max_sleeps <= 1) ? 1 : 2;
      $form_state['input']['data']['group_size_children'] = 0;
      $form_state['input']['rooms_start_date']['date'] = gmdate('d/m/Y', $startdate);
      $form_state['input']['rooms_end_date']['date'] = gmdate('d/m/Y', $enddate);
      $form_state['values']['rooms_start_date'] = gmdate('Y-m-d', $startdate);
      $form_state['values']['rooms_end_date'] = gmdate('Y-m-d', $enddate);

      $form_state['show_availability_options'] = TRUE;
    }

    // Set the unit types - everytime a different type is chosen the available
    // units for that type pop-up.
    $form['availability_fieldset']['unit_type'] = array(
      '#type' => 'select',
      '#title' => t('Unit Type'),
      '#options' => $types,
      '#empty_option' => t('- Select -'),
      '#default_value' => $selected,
      '#ajax' => array(
        'callback' => 'rooms_booking_edit_form_unit_type_callback',
        'wrapper' => 'unit-wrapper',
      ),
    );
    unset($form['unit_id']);

    // This is where the available untis will pop up.
    $form['availability_fieldset']['unit_fieldset'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div class="rooms-booking-unit-wrapper" id="unit-wrapper">',
      '#suffix' => '</div>',
    );

    $form['availability_fieldset']['unit_fieldset']['instructions'] = array();

    // If the user has selected a unit type or we already have a unit assigned
    // show all the available units of that type (Also the already assign unit).
    if (isset($form_state['values']['unit_type']) || isset($booking->unit_type)) {

      $search_unit_type = '';
      if (isset($form_state['values']['unit_type'])) {
        $search_unit_type = $form_state['values']['unit_type'];
      }
      elseif (isset($booking->unit_type)) {
        $search_unit_type = $booking->unit_type;
      }

      if (!empty($search_unit_type)) {
        $childrens_age = array();
        if (isset($form_state['values']['data']['childrens_age'])) {
          $childrens_age = $form_state['values']['data']['childrens_age'];
        }

        $group_size_children = (isset($form_state['input']['data']['group_size_children'])) ? $form_state['input']['data']['group_size_children'] : 0;
        list($start_date, $end_date) = rooms_form_input_get_start_end_dates($form_state);
        $available_units = rooms_booking_edit_form_get_rooms($search_unit_type, $booking, $start_date, $end_date, $form_state['input']['data']['group_size'], $group_size_children, $childrens_age);
        if ($available_units == ROOMS_NO_ROOMS) {
          $form['availability_fieldset']['unit_fieldset']['instructions'] = array(
            '#markup' => t('No @unit_type units are available for specified party size on the chosen dates.', array('@unit_type' => $search_unit_type)),
          );
        }
        else {
          foreach ($available_units as $type => $units_per_price) {
            // Load the type obj and set a title.
            $type_obj = rooms_unit_type_load($type);
            $form['availability_fieldset']['unit_fieldset']['type'] = array(
              '#type' => 'fieldset',
              '#title' => t('@unit_type available bookable units', array('@unit_type' => $type_obj->label)),
              '#attributes' => array('class' => array('rooms-booking-unit-type-wrapper')),
            );

            $form['availability_fieldset']['unit_fieldset']['instructions'] = array(
              '#markup' => t('The following units are available for the chosen dates; please select one.'),
            );

            $options = array();
            // Create elements based on type/price categorisation.
            foreach ($units_per_price as $price => $units) {
              foreach ($units as $unit) {
                $options[$unit['unit']->unit_id] = $unit['unit']->name . ' - ' . t('Cost') . ': ' .
                  t('@currency_symbol @amount', array( // TODO: replace with a proper currency formatter
                      '@currency_symbol' => $currency_symbol,
                      '@amount' => $units[key($units)]['price'])
                  );
              }
            }

            $form['availability_fieldset']['unit_fieldset']['type']['unit_id'] = array(
              '#type' => 'radios',
              '#options' => $options,
              '#validated' => TRUE,
              '#default_value' => isset($booking->unit_id) ? $booking->unit_id : '',
              '#ajax' => array(
                'callback' => 'rooms_booking_select_unit_callback',
                'wrapper' => 'options-wrapper',
              ),
            );

            if (isset($unitid)) {
              $form['availability_fieldset']['unit_fieldset']['type']['unit_id']['#default_value'] = $unitid;
            }

            $form['availability_fieldset']['unit_fieldset']['options_fieldset'] = array(
              '#type' => 'fieldset',
              '#prefix' => '<div class="rooms-booking-options-wrapper" id="options-wrapper">',
              '#suffix' => '</div>',
            );

            // If a unit is selected.
            if (!empty($form_state['values']['unit_id']) || isset($booking->unit_id) || $unitid != '') {
              if ($unitid != '') {
                $unit_id = $unitid;
              }
              else {
                $unit_id = (isset($form_state['values']['unit_id'])) ? $form_state['values']['unit_id'] : $booking->unit_id;
              }
              $unit = rooms_unit_load($unit_id);

              foreach (rooms_unit_get_unit_options($unit) as $option) {
                if (!isset($form['availability_fieldset']['unit_fieldset']['options_fieldset']['#title'])) {
                  $form['availability_fieldset']['unit_fieldset']['options_fieldset']['#title'] = t('Available options for @unit_name', array('@unit_name' => $unit->name));
                }
                $option_name = strtolower(str_replace(array(':', ' '), '_', $option['name']));

                $form['availability_fieldset']['unit_fieldset']['options_fieldset'][$option_name] = array(
                  '#type' => 'checkbox',
                  '#title' => t($option['name']),
                  '#default_value' => isset($booking->data[$option_name]) ? '1' : '0',
                  '#ajax' => array(
                    'callback' => 'rooms_booking_select_unit_callback',
                    'wrapper' => 'options-wrapper',
                  ),
                );

                // Show quantity field selector if an option quantity is set.
                if (is_numeric($option['quantity']) && isset($form_state['values'][$option_name]) && $form_state['values'][$option_name] == 1 && $option['quantity'] > 1) {
                  $form['availability_fieldset']['unit_fieldset']['options_fieldset'][$option_name . ':quantity'] = array(
                    '#type' => 'select',
                    '#title' => t('Quantity'),
                    '#options' => rooms_range(1, $option['quantity']),
                    '#default_value' => $form_state['values'][$option_name . ':quantity'],
                    '#ajax' => array(
                      'callback' => 'rooms_booking_select_unit_callback',
                      'wrapper' => 'options-wrapper',
                    ),
                  );
                }
              }

              // Create Price Modifiers.
              $price_modifiers = array();
              foreach (rooms_unit_get_unit_options($unit) as $option) {
                $option_name = strtolower(str_replace(array(':', ' '), '_', $option['name']));

                if (isset($form_state['values'][$option_name])) {
                  if ($form_state['values'][$option_name] == 1) {
                    $quantity = 1;
                    if (isset($form_state['values'][$option_name . ':quantity']) && $option['operation'] != ROOMS_REPLACE) {
                      $quantity = $form_state['values'][$option_name . ':quantity'] + 1;
                    }

                    $price_modifiers[$option_name] = array(
                      '#type' => ROOMS_DYNAMIC_MODIFIER,
                      '#op_type' => $option['operation'],
                      '#amount' => $option['value'],
                      '#quantity' => $quantity,
                    );
                  }
                }
              }

              $form['availability_fieldset']['unit_fieldset']['options_fieldset']['price'] = array(
                '#type' => 'textfield',
                '#title' => t('Price'),
                '#default_value' => '',
                '#size' => '10',
                '#required' => TRUE,
              );

              list($start_date, $end_date) = rooms_form_values_get_start_end_dates($form_state);

              $group_size = isset($form_state['values']['data']['group_size']) ? $form_state['values']['data']['group_size'] : $form_state['input']['data']['group_size'];
              $group_size_children = isset($form_state['values']['data']['group_size']) ? $form_state['values']['data']['group_size_children'] : $form_state['input']['data']['group_size_children'];
              $childrens_age = isset($form_state['values']['data']['childrens_age']) ? $form_state['values']['data']['childrens_age'] : array();

              $booking_info = array(
                'start_date' => clone($start_date),
                'end_date' => clone($end_date),
                'unit' => rooms_unit_load($unit_id),
                'booking_parameters' => array(
                  'group_size' => $group_size,
                  'group_size_children' => $group_size_children,
                  'childrens_age' => $childrens_age,
                ),
              );

              // Give other modules a chance to change the price modifiers.
              drupal_alter('rooms_price_modifier', $price_modifiers, $booking_info);

              // Apply price modifiers and replace unit price.
              $price_calendar = new UnitPricingCalendar($unit_id, $price_modifiers);

              $end_date->sub(new DateInterval('P1D'));
              $new_price = $price_calendar->calculatePrice($start_date, $end_date, $group_size, $group_size_children, $childrens_age);
              $new_price = $new_price['full_price'];

              // When user press 'Re-assign Unit' button we don't replace price.
              if (isset($form_state['triggering_element'])) {
                if ($form_state['triggering_element']['#value'] == t('Re-assign Unit') && $form_state['triggering_element']['#name'] == 'op') {
                  $form['availability_fieldset']['unit_fieldset']['options_fieldset']['price']['#value'] = number_format($booking->price / 100, 2, '.', '');
                }
                else {
                  $form['availability_fieldset']['unit_fieldset']['options_fieldset']['price']['#value'] = number_format($new_price, 2, '.', '');
                }
              }
              else {
                $form['availability_fieldset']['unit_fieldset']['options_fieldset']['price']['#value'] = number_format($new_price, 2, '.', '');
              }
            }
          }
        }
      }
      else {
        $form['availability_fieldset']['unit_fieldset']['instructions'] = array(
          '#markup' => t('You must select a Unit type to choose a unit'),
        );
      }
    }
  }

  // Add the field related form elements.
  $form_state['rooms_booking'] = $booking;
  field_attach_form('rooms_booking', $booking, $form, $form_state);

  // Set an #ajax callback for start/end date elements for edited bookings.
  if (is_numeric($booking->booking_id)) {
    $form['rooms_date_range']['rooms_start_date']['#ajax'] = array(
      'callback' => 'rooms_booking_date_ajax_callback',
      'event' => 'blur',
    );
    $form['rooms_date_range']['rooms_end_date']['#ajax'] = array(
      'callback' => 'rooms_booking_date_ajax_callback',
      'event' => 'blur',
    );
  }

  // Management vertical tabs.
  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Add the user account and e-mail fields.
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('administer rooms_booking entities'),
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'rooms_booking') . '/js/rooms_booking.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
    '#weight' => 30,
  );
  $form['user']['owner_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Owned by'),
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => !empty($booking->owner_name) ? $booking->owner_name : '',
    '#weight' => -1,
  );

  // Add a log checkbox and timestamp field to a history tab.
  $form['booking_history'] = array(
    '#type' => 'fieldset',
    '#title' => t('Booking history'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'additional_settings',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'rooms_booking') . '/js/rooms_booking.js'),
    ),
    '#weight' => 40,
  );
  $form['booking_history']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Created on'),
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', array(
      '%time' => !empty($booking->date) ? date_format(date_create($booking->date), 'Y-m-d H:i:s O') : format_date($booking->created, 'custom', 'Y-m-d H:i:s O'),
      '%timezone' => !empty($booking->date) ? date_format(date_create($booking->date), 'O') : format_date($booking->created, 'custom', 'O'),
    )),
    '#maxlength' => 25,
    '#default_value' => !empty($booking->created) ? format_date($booking->created, 'custom', 'Y-m-d H:i:s O') : '',
  );
  $form['booking_history']['created'] = array(
    '#type' => 'hidden',
    '#value' => !empty($booking->created) ? format_date($booking->created, 'short') : '',
    '#attributes' => array('id' => 'edit-created'),
  );
  $form['booking_history']['changed'] = array(
    '#type' => 'hidden',
    '#value' => !empty($booking->changed) ? format_date($booking->changed, 'short') : '',
    '#attributes' => array('id' => 'edit-changed'),
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  // Wrapper to show validation messages where the assigned dates are changed
  // and the price is not re-assigned.
  $form['availability_fieldset']['assigned_dates_msg'] = array(
    '#markup' => '<div id="assigned-dates-msg"></div>',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Booking'),
    '#submit' => $submit + array('rooms_booking_edit_form_submit'),
  );

  if (!empty($booking->name) && rooms_booking_access('delete', $booking)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Booking'),
      '#submit' => $submit + array('rooms_booking_form_submit_delete'),
      '#weight' => 45,

    );
  }
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/rooms/bookings', array('attributes' => array('class' => array('ctools-close-modal')))),
    '#weight' => 50,
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_form_start_end_dates_validate';
  $form['#validate'][] = 'rooms_booking_edit_form_validate';
  return $form;
}

/**
 * AJAX callback for the end date admin form element.
 *
 * Triggers automatically the re-assign submit button and help the user to don't
 * forget re-assign price.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_date_ajax_callback($form, &$form_state) {
  $commands[] = ajax_command_invoke('#edit-get-availability', 'trigger', array('mousedown'));
  print ajax_render($commands);
  exit();
}

/**
 * AJAX callback for the children ages form element.
 *
 * Triggers automatically the re-assign submit button and help the user to don't
 * forget re-assign price.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_childrens_age_ajax_callback($form, &$form_state) {
  if (isset($form_state['values']['unit_id'])) {
    $commands[] = ajax_command_invoke('#edit-get-availability', 'trigger', array('mousedown'));
    print ajax_render($commands);
  }

  exit();
}

/**
 * AJAX callback for the children form selector to show textfields for ages.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_children_change_callback($form, &$form_state) {
  return $form['data']['childrens_age'];
}

/**
 * Submit callback for Assign unit button.
 *
 * When the user presses the assign unit button we rebuild the form and
 * show the available units
 */
function rooms_booking_edit_form_availability_submit($form, &$form_state) {
  $form_state['show_availability_options'] = TRUE;
  $form_state['rebuild'] = TRUE;
}


/**
 * AJAX callback for Assign unit button.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_edit_form_availability_callback(&$form, $form_state) {
  return $form['availability_fieldset'];
}

/**
 * AJAX callback for unit and unit options form elements.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_select_unit_callback(&$form, $form_state) {
  return $form['availability_fieldset']['unit_fieldset']['options_fieldset'];
}

/**
 * AJAX callback for group size form elements.
 *
 * @see rooms_booking_edit_form()
 */
function rooms_booking_group_size_ajax_callback(&$form, $form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#rooms_booking_group_size_children_container', render($form['data']['group_size_children']));
  $commands[] = ajax_command_replace('#rooms_booking_childrensage', render($form['data']['childrens_age']));

  if (isset($form_state['values']['unit_id'])) {
    $commands[] = ajax_command_invoke('#edit-get-availability', 'trigger', array('mousedown'));
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Callback for the actual list of available units.
 */
function rooms_booking_edit_form_unit_type_callback(&$form, $form_state) {
  return $form['availability_fieldset']['unit_fieldset'];
}

/**
 * Returns a list of room types user can edit.
 */
function rooms_booking_edit_form_get_room_types() {
  $efq = new EntityFieldQuery();
  $efq->entityCondition('entity_type', 'rooms_unit');
  $efq->addTag('rooms_availability_access');
  $result = $efq->execute();
  $entities = entity_load('rooms_unit', array_keys($result['rooms_unit']));
  $types = array();
  $all_types = rooms_unit_get_types();
  foreach ($entities as $entity) {
    if (!isset($types[$entity->type])) {
      $current_type = $all_types[$entity->type];
      $types[$entity->type] = $current_type->label;
    }
  }
  return $types;
}

/**
 * Returns a set of available units given a unit type, start and end dates.
 *
 * It makes special provisions for the currently selected unit - to consider
 * that as "available" as well.
 *
 * @param string $unit_type
 *   The unit type.
 * @param RoomsBooking $current_booking
 *   The current booking.
 * @param DateTime $start_date
 *   Start date.
 * @param DateTime $end_date
 *   End date.
 * @param int $group_size
 *   Group size.
 * @param int $group_size_children
 *   Number of children in the group.
 * @param array $children_age
 *   Children age array.
 *
 * @return array|int
 *   Array containing available units or integer to indicate error code.
 */
function rooms_booking_edit_form_get_rooms($unit_type, $current_booking, $start_date, $end_date, $group_size = 1, $group_size_children = 0, $children_age = array()) {

  // Set the unit types,
  $unit_types = array($unit_type);
  $current_booking_state = '';

  $bookable_units = array();

  // The current unit for this booking should also return as available.
  if (isset($current_booking->booking_status) && isset($current_booking->booking_id)) {
    $current_booking_state = rooms_availability_assign_id($current_booking->booking_id, $current_booking->booking_status);
  }

  $valid_states = array_keys(array_filter(variable_get('rooms_valid_availability_states', drupal_map_assoc(array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST)))));

  if (variable_get('rooms_price_calculation', ROOMS_PER_NIGHT) == ROOMS_PER_PERSON) {
    $booking_parameters = array(array('adults' => $group_size, 'children' => $group_size_children, 'childrens_age' => $children_age));
  }
  else {
    $booking_parameters = array(array('adults' => $group_size, 'children' => $group_size_children));
  }

  if (!empty($current_booking->booking_id)) {
    $ac = new AvailabilityAgent($start_date, $end_date, $booking_parameters, 1, array_merge($valid_states, array(rooms_availability_assign_id($current_booking->booking_id, '0'), rooms_availability_assign_id($current_booking->booking_id))), $unit_types);
  }
  else {
    $ac = new AvailabilityAgent($start_date, $end_date, $booking_parameters, 1, array_merge($valid_states, array($current_booking_state)), $unit_types);
  }
  $available_units = $ac->checkAvailability(TRUE);

  if (is_array($available_units)) {
    foreach ($available_units as $type => $units_per_price) {
      foreach ($units_per_price as $price => $units) {
        foreach ($units as $unit_id => $unit) {
          if (rooms_unit_access('update availability', $unit['unit'])) {
            $bookable_units[$type][$price][$unit_id] = $unit;
          }
        }
      }
    }
  }

  return empty($bookable_units) ? ROOMS_NO_ROOMS : $bookable_units;
}


/**
 * Form API validate callback for the booking form.
 */
function rooms_booking_edit_form_validate(&$form, &$form_state) {
  $booking = $form_state['rooms_booking'];
  list($start_date, $end_date) = rooms_form_input_get_start_end_dates($form_state);

  if ($start_date && $end_date) {
    // Convert input format to the same used in the stored dates.
    $input_start_date = $start_date->format('Y-m-d');
    $input_end_date = $end_date->format('Y-m-d');
    // If stored dates differ from input dates check that price was re-assigned.
    if (isset($booking->start_date) && isset($booking->end_date)) {
      if ($input_start_date != $booking->start_date || $input_end_date != $booking->end_date) {
        if (!isset($form_state['values']['unit_id']) || !is_numeric($form_state['values']['unit_id'])) {
          form_set_error('availability_fieldset', t('A unit must be re-assigned for this booking.'));
        }
      }
    }
  }

  // Check if unit assigned.
  if ((!isset($form_state['values']['unit_id'])) || ($form_state['values']['unit_id'] == '')) {
    form_set_error('availability_fieldset', t('A unit has not been assigned for this booking.'));
  }
  elseif ((isset($form_state['values']['unit_id'])) && ($form_state['values']['unit_id'] == '')) {
    form_set_error('availability_fieldset', t('A unit has not been assigned for this booking.'));
  }

  $booking = $form_state['rooms_booking'];
  if (isset($booking->unit_id)) {
    $new_unit_id = $booking->unit_id;
    if (isset($form_state['values']['unit_id'])) {
      $new_unit_id = $form_state['values']['unit_id'];
    }

    if ($new_unit_id == $booking->unit_id) {
      // We are going to be updating the event. So the removing the old event.
      if (($booking->unit_id != 0) && $start_date && $end_date) {
        // Create a calendar.
        $uc = new UnitCalendar($booking->unit_id);

        $adjusted_end_date = clone($end_date);
        $adjusted_end_date->modify('-1 day');

        // Create an event representing the event to remove.
        $event_id = rooms_availability_assign_id($booking->booking_id, $booking->booking_status);
        $be = new BookingEvent($booking->unit_id, $event_id, $start_date, $end_date);

        // Check if a locked event is blocking the update.
        $states_confirmed = $uc->getStates($start_date, $adjusted_end_date);

        $valid_states = array_keys(array_filter(variable_get('rooms_valid_availability_states', drupal_map_assoc(array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST)))));
        $valid_states = array_merge($valid_states, array(ROOMS_UNCONFIRMED_BOOKINGS, $be->id));

        $state_diff_confirmed = array_diff($states_confirmed, $valid_states);

        if (count($state_diff_confirmed) > 0) {
          $date1 = new DateTime($form['rooms_date_range']['rooms_start_date']['#default_value']);
          $date2 = new DateTime($form['rooms_date_range']['rooms_end_date']['#default_value']);

          $form['rooms_date_range']['rooms_start_date']['date']['#value'] = $date1->format($form['rooms_date_range']['rooms_start_date']['#date_format']);
          $form['rooms_date_range']['rooms_end_date']['date']['#value'] = $date2->format($form['rooms_date_range']['rooms_end_date']['#date_format']);

          form_set_error('date_range', t('Could not update calendar because a locked event is blocking the update - you need to unlock any locked events in that period.'));
        }

        // Check if this booking overlap with an unconfirmed booking.
        $states_unconfirmed = $uc->getStates($start_date, $adjusted_end_date, TRUE);

        $valid_states = array_keys(array_filter(variable_get('rooms_valid_availability_states', drupal_map_assoc(array(ROOMS_AVAILABLE, ROOMS_ON_REQUEST)))));
        $valid_states = array_merge($valid_states, array($be->id));

        $state_diff_unconfirmed = array_diff($states_unconfirmed, $valid_states);

        if (count($state_diff_unconfirmed) > 0) {
          $booking_id = rooms_availability_return_id(array_pop($state_diff_unconfirmed));
          form_set_error('date_range', t('Change of dates leads to an overlap with an unconfirmed booking, please resolve issue with booking !booking_link first.', array('!booking_link' => l($booking_id, 'admin/rooms/bookings/booking/' . $booking_id . '/edit'))));
        }
      }
    }
  }

  // For new bookings check if the owner has an outstanding order, don't run on
  // AJAX requests due this could cause message duplication.
  if (!isset($booking->customer_id) && strpos(request_uri(), 'system/ajax') === FALSE) {
    $customer_name = $form_state['input']['client'];
    $commerce_customer_id = rooms_booking_find_customer_by_name($customer_name);
    // The statuses that are considered a complete order we could use
    // commerce_payment_order_balance($order); but that adds the dependency
    // of commerce_payment module and all the orders must have a transaction
    // history to have a reliable balance.
    $complete_statuses = array('complete', 'rooms_unit_confirmed');
    // Determine the commerce order statuses that identify an outstanding order.
    $incomplete_statuses = array_diff(array_keys(commerce_order_statuses()), $complete_statuses);
    $orders = commerce_order_load_multiple(array(), array('uid' => $commerce_customer_id, 'status' => $incomplete_statuses));

    $query = new EntityFieldQuery();
    $orders = $query->entityCondition('entity_type', 'commerce_order')
      ->fieldCondition('commerce_customer_billing', 'profile_id', $commerce_customer_id)
      ->execute();

    // In case of pending orders alert the admin.
    if ($orders) {
      foreach ($orders['commerce_order'] as $order) {
        $order_links[] = l(t('Order #@num', array('@num' => $order->order_id)), "admin/commerce/orders/{$order->order_id}/edit");
      }
      $message_params = array(
        '!customer_name' => check_plain($customer_name),
        '!order_links' => theme('item_list', array('items' => $order_links)),
      );
      drupal_set_message(t('The customer !customer_name has an outstanding order: !order_links', $message_params), 'warning');
    }
  }

  $client = explode(':', $form_state['values']['client']);
  $client_name = $client[0];
  $commerce_customer_id = rooms_booking_find_customer_by_name($client_name);
  if ($commerce_customer_id === FALSE) {
    form_set_error('client', t('Enter a valid customer.'));
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('rooms_booking', $booking, $form, $form_state);
}

/**
 * Form API submit callback for the Booking form.
 *
 * @todo remove hard-coded link
 */
function rooms_booking_edit_form_submit(&$form, &$form_state) {
  // Get the client name and client id.
  $client = explode(':', $form_state['values']['client']);
  $client_name = $client[0];
  $commerce_customer_id = rooms_booking_find_customer_by_name($client_name);

  // Save customer in rooms_customers table.
  db_merge('rooms_customers')
    ->key(array('name' => $client_name))
    ->fields(array(
      'name' => $client_name,
      'commerce_customer_id' => $commerce_customer_id,
    ))
    ->execute();

  // Get customer id from rooms_customers table.
  $client_id = db_select('rooms_customers')
    ->fields('rooms_customers', array('id'))
    ->condition('name', $client_name, '=')
    ->execute()->fetchField();

  // Put them back in $form_state so the entity controller builds the entity.
  $form_state['values']['name'] = $client_name;
  $form_state['values']['customer_id'] = $client_id;

  // We also need appropriate named variables for start and end date.
  // It's simpler to do this than change all the other code for now.
  $form_state['values']['start_date'] = $form_state['values']['rooms_start_date'];
  $form_state['values']['end_date'] = $form_state['values']['rooms_end_date'];

  $unit_id = 0;
  if (isset($form_state['values']['unit_id'])) {
    $unit_id = $form_state['values']['unit_id'];
  }

  // If we are dealing with a new booking.
  if ($form_state['rooms_booking']->booking_id == '') {

    $booking = rooms_booking_create(array('type' => $form_state['rooms_booking']->type));
    $form_state['rooms_booking'] = $booking;

    $booking = entity_ui_controller('rooms_booking')->entityFormSubmitBuildEntity($form, $form_state);

    $booking->is_new = isset($booking->is_new) ? $booking->is_new : 0;
  }
  else {
    $booking = entity_ui_controller('rooms_booking')->entityFormSubmitBuildEntity($form, $form_state);

    // Get the unit id from the booking to be used further down as it is not in
    // the form_values because of how unit ids are assigned.
    $unit_id = $booking->unit_id;
  }

  // Add in created and changed times.
  $booking->created = !empty($booking->date) ? strtotime($booking->date) : REQUEST_TIME;
  $booking->changed = time();

  // Add in the booking owner.
  if ($account = user_load_by_name($booking->owner_name)) {
    $booking->uid = $account->uid;
  }
  else {
    $booking->uid = 0;
  }

  if (isset($form_state['input']['price'])) {
    $booking->price = $form_state['input']['price'] * 100;
  }

  $unit = rooms_unit_load($unit_id);

  // Store selected unit options.
  foreach (rooms_unit_get_unit_options($unit) as $option) {
    $option_name = strtolower(str_replace(array(':', ' '), '_', $option['name']));
    if (isset($form_state['values'][$option_name])) {
      if ($form_state['values'][$option_name] == '1') {
        $booking->data[$option_name] = 1;
        if (isset($form_state['values'][$option_name . ':quantity'])) {
          $booking->data[$option_name . ':quantity'] = $form_state['values'][$option_name . ':quantity'];
        }
      }
    }
  }

  // Store children ages in booking's data field.
  if (isset($form_state['values']['data']['childrens_age'])) {
    $booking->data['childrens_age'] = $form_state['values']['data']['childrens_age'];
  }

  // Save booking.
  $booking->save();

  // Add the booking to $form_state to be altered by other submit handlers.
  $form_state['booking'] = $booking;

  // We have a unit defined so lets block availability there.
  if ($unit_id != 0) {
    if ($booking->rooms_av_update == ROOMS_UPDATED) {
      drupal_set_message(t('Unit availability has been updated.'));
    }
    else {
      drupal_set_message(t('Unit availability could not be updated.'), 'error');
    }
  }

  $form_state['redirect'] = 'admin/rooms/bookings';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_booking_form_submit_delete($form, &$form_state) {
  if (isset($form_state['ajax'])) {
    rooms_booking_delete($form_state['rooms_booking']);
    drupal_set_message(t('The booking has been removed'));
    $form_state['booking_deleted'] = TRUE;
  }
  else {
    $form_state['redirect'] = 'admin/rooms/bookings/booking/' . $form_state['rooms_booking']->booking_id . '/delete';
  }
}

/**
 * Form callback: confirmation form for deleting a booking.
 *
 * @param $booking
 *   The booking to delete.
 *
 * @see confirm_form()
 */
function rooms_booking_delete_form($form, &$form_state, $booking) {
  $form_state['rooms_booking'] = $booking;

  $form['#submit'][] = 'rooms_booking_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete Booking %name?', array('%name' => $booking->name)),
    'admin/rooms/bookings/booking',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for booking_delete_form.
 */
function rooms_booking_delete_form_submit($form, &$form_state) {
  $booking = $form_state['rooms_booking'];

  rooms_booking_delete($booking);

  drupal_set_message(t('The booking %name has been deleted.', array('%name' => $booking->name)));
  watchdog('rooms', 'Deleted booking %name.', array('%name' => $booking->name));

  $form_state['redirect'] = 'admin/rooms/bookings';
}

/**
 * Page to add Rooms.
 *
 * @todo Pass this through a proper theme function
 */
function rooms_booking_add_page() {
  $controller = entity_ui_controller('rooms_booking');
  return $controller->addPage();
}

/**
 * Displays the list of available room types for room creation.
 *
 * @ingroup themeable
 */
function theme_rooms_booking_add_list($variables) {
  $content = $variables['content'];
  if ($content) {
    $output = '<dl class="booking-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer booking types')) {
      $output = '<p>' . t('Bookings cannot be added because you have not created any booking types yet. Go to the <a href="@create-booking-type">booking type creation page</a> to add a new booking type.', array('@create-booking-type' => url('admin/rooms/bookings/booking-types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No booking types have been created yet for you to use.') . '</p>';
    }
  }
  return $output;
}

/**
 * Sets the breadcrumb for administrative rooms pages.
 */
function rooms_booking_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Rooms'), 'admin/rooms'),
    l(t('Bookings'), 'admin/rooms/bookings'),
  );

  drupal_set_breadcrumb($breadcrumb);
}

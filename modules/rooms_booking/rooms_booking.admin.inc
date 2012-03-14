<?php

/**
 * @file
 * Rooms editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class RoomsBookingUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
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
      //'type' => MENU_NORMAL_ITEM,
      'weight' => 10,
    );
        
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a Booking',
      'description' => 'Add a new Booking',
      'page callback'  => 'rooms_booking_add_page',
      'access callback'  => 'rooms_booking_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of room.
    foreach (rooms_booking_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'rooms_booking_create_form_wrapper',
        'page arguments' => array($type->type),
        'access callback' => 'rooms_booking_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'rooms_booking.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing Rooms entities
    $items[$this->path . '/booking/' . $wildcard] = array(
      'page callback' => 'rooms_booking_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_booking_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
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
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'rooms_booking.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing rooms
    $items['booking/' . $wildcard] = array(
      //'title' => 'Title',
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
        
    return theme('rooms_booking_add_list', array('content' => $content));
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
  // Add the breadcrumb for the form's location.
  rooms_booking_set_breadcrumb();
  // Add the js for the improved popup handling
  drupal_add_js(drupal_get_path('module', 'rooms') . '/js/rooms_date_popup.js');
  // Create a booking object
  $booking = rooms_booking_create(array('type' => $type));
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
 * Form callback: create or edit a booking.
 *
 * @param $booking
 *   The RoomBooking object to edit or for a create form an empty booking object
 *     with only a booking type defined.
 */
function rooms_booking_edit_form($form, &$form_state, $booking) {
  $client = '';

  // Check to see if we are loading an existing booking (existing bookings
  // have customer ids set) 
  if (isset($booking->customer_id)) {
    // Get the client name - we need this to set the Client field
    $client_name = $booking->name;
 
    // The Client value needs the id as well (to take us back to the Customer Profile in Commerce)
    $client = $booking->name . ':' . $booking->customer_id;
   }  
  
  $form['client'] = array(
    '#type' => 'textfield', 
    '#title' => t('Customer'), 
    '#maxlength' => 60, 
    '#autocomplete_path' => 'admin/rooms/bookings/customers',
    '#description' => t('A customer profile must already exist under Customer Profiles in the <a href="@store-profile">store</a>.', array('@store-profile' => url('admin/commerce/customer-profiles'))),
    '#default_value' => $client,
    '#weight' => -1,
    '#required' => TRUE,
  );
  $form['data']['#tree'] = TRUE;
  $form['data']['group_size'] = array(
    '#type' => 'select',
    '#title' => 'Group Size',
    '#options' => array(
      '1' => '1',
      '2' => '2',
      '3' => '3',
      '4' => '4',
      '5' => '5',
      '6' => '6',
    ),
    '#default_value' =>  isset($booking->data['group_size']) ? $booking->data['group_size'] : '2',
    '#description' => t('The number of people staying in this room'),
  );  

  // A fieldset to hold the date range fields
  $form['rooms_date_range'] = array (
    '#type' => 'fieldset',
    '#title' => t('Assign Dates'),
  );
  
  $form['rooms_date_range'] = rooms_date_range_fields($form['rooms_date_range']);
  
  // Unset the default for max days away for bookings since we are on the admin side here
  drupal_add_js(array('rooms' => array('roomsBookingStartDay' => 0)), 'setting');
  
  // Set the default values for the dates  
  $form['rooms_date_range']['rooms_start_date']['#default_value'] = isset($booking->start_date) ? $booking->start_date : '';
  $form['rooms_date_range']['rooms_end_date']['#default_value'] = isset($booking->end_date) ? $booking->end_date : '';
    
  // Access to availability information 
  $form['get_availability'] = array(
    '#type' => 'submit',
    '#value' => isset($booking->unit_id) ? t('Re-assign Unit') : t('Assign Room'),
    '#submit' => array('rooms_booking_edit_form_availability_submit'),
    '#validate' => array('rooms_booking_edit_form_availability_validate'),
    '#ajax' => array(
      'callback' => 'rooms_booking_edit_form_availability_callback',
      'wrapper' => 'availability-wrapper',
    ),
  );
    
  // If a unit is assigned get the unit type label
  if (isset($booking->unit_id)) {
    $type_obj = rooms_unit_type_load($booking->unit_type);
    $unit = rooms_unit_load($booking->unit_id);
  }
  $unit_selected = isset($booking->unit_id) ? t('Currently assigned unit: @unit_name / @type', array('@type' => $type_obj->label, '@unit_name' => $unit->name)) : t('No Unit assigned');

  $form['availability_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'Availability',
      '#description' => $unit_selected,
      '#prefix' => '<div id="availability-wrapper">',
      '#suffix' => '</div>',
  );

  $form['b_status'] = array (
    '#type' => 'fieldset',
    '#title' => t('Booking Status'),
  );
  $form['b_status']['booking_status'] = array(
    '#type' => 'checkbox',
    '#title' => 'Booking Confirmed',
    '#options' => array(
      '0' => '0',
      '1' => '1',
    ),
    '#default_value' =>  isset($booking->booking_status) ? $booking->booking_status : '0',
  );

  // If submit above pressed and we have dates we can process to assign a unit
  if (isset($form_state['show_availability_options'])) {
    $room_types = rooms_unit_get_types();
    $types = array('notype' => 'Choose Room Type');
    foreach ($room_types as $type) {
      $types[$type->type] = $type->label;
    }
    
    // Figure out which is the current unit and mark that as selected
    $selected = '';
    if (isset($form_state['values']['unit_type'])) {
      $selected = $form_state['values']['unit_type'];
    }
    elseif (isset($booking->unit_type)) {
      $selected = $booking->unit_type;
    }

    // Set the unit types - everytme a different type is chosen the available
    // units for that type pop-up.
    $form['availability_fieldset']['unit_type'] = array(
      '#type' => 'select', 
      '#title' => t('Unit Type'), 
      '#options' => $types,
      '#default_value' => $selected,
      '#ajax' => array(
        'callback' => 'rooms_booking_edit_form_unit_type_callback',
        'wrapper' => 'unit-wrapper',
      ),  
    );

    // This is where the available untis will pop up
    $form['availability_fieldset']['unit_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Units'),
      '#prefix' => '<div id="unit-wrapper">',
      '#suffix' => '</div>',
    );

    
    $form['availability_fieldset']['unit_fieldset']['instructions'] = array(
      '#markup' => t('Choose a unit from the list of available rooms below - we only show
                     units with availability for the dates selected.'),
    );
    
    // If the user has selected a unit type or we already have a unit assigned
    // show all the available units of that type (including the already assign unit)
    if (isset($form_state['values']['unit_type']) || isset($booking->unit_type)) {
      
      $search_unit_type = '';
      if (isset($form_state['values']['unit_type'])) {
        $search_unit_type =  $form_state['values']['unit_type'];
      }
      elseif(isset($booking->unit_type)) {
        $search_unit_type = $booking->unit_type;
      }

      $available_units = rooms_booking_edit_form_get_rooms($search_unit_type, $form_state['input']['rooms_start_date']['date'], $form_state['input']['rooms_end_date']['date'], $form_state['input']['data']['group_size'], $booking);
      if ($available_units == 2) {
        $form['availability_fieldset']['unit_fieldset']['instructions'] = array(
          '#markup' => t('No @search_unit_type are available', array('@search_unit_type' => $search_unit_type)),
        );
      } else {
        foreach ($available_units as $type => $units_per_price) {
          // Load the type obj and set a title
          $type_obj = rooms_unit_type_load($type);
          $form['availability_fieldset']['unit_fieldset']['type'] = array(
            '#type' => 'fieldset',
            '#title' => t($type_obj->label),
          );
  
          $options = array(); 
          // Create elements based on type/price categorisation
          foreach ($units_per_price as $price => $units) {
            foreach ($units as $unit) {
              $options[$unit['unit']->unit_id] = $unit['unit']->name . ' - ' . t('Cost:') . ' ' . $units[key($units)]['price'] . commerce_default_currency();
            }
          }
  
          $form['availability_fieldset']['unit_fieldset']['type']['unit_id'] = array(
            '#title' => t('Units'),
            '#type' => 'radios',
            '#options' => $options,
            '#default_value' => isset($booking->unit_id) ? $booking->unit_id : '',
          );            
        }
      }
    }
  }

  // Add the field related form elements.
  $form_state['rooms_booking'] = $booking;
  field_attach_form('rooms_booking', $booking, $form, $form_state);
  
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

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Booking'),
    '#submit' => $submit + array('rooms_booking_edit_form_submit'),
  );
  
  if (!empty($booking->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Booking'),
      '#suffix' => l(t('Cancel'), 'admin/rooms/bookings'),
      '#submit' => $submit + array('rooms_booking_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_booking_edit_form_validate';
  return $form;
}


/**
 * Validate function for the assign unit functionality - just a place holder
 */
function rooms_booking_edit_form_availability_validate(&$form, &$form_state) {
}


/**
 * When the user presses the assign unit button we rebuild the form and
 * show the available units
 */
function rooms_booking_edit_form_availability_submit($form, &$form_state) {
  $form_state['show_availability_options'] = TRUE;
  $form_state['rebuild'] = TRUE;
}


/**
 * Simply returns the placeholder for all the relevant unit assignment
 * functionality.
 */
function rooms_booking_edit_form_availability_callback(&$form, $form_state) {  
  return $form['availability_fieldset'];
}


/**
 * Callback for the actual list of available units
 */
function rooms_booking_edit_form_unit_type_callback(&$form, $form_state) {  
  return $form['availability_fieldset']['unit_fieldset'];
}


/**
 * Returns a set of available units given a unit type, start and end dates.
 *
 * It makes special provisions for the currently selected unit - to consider that
 * as "available" as well.
 */
function rooms_booking_edit_form_get_rooms($select, $start_date = 0, $end_date = 0, $group_size, $current_booking) {
  // Set the unit types,
  $unit_types = array($select);

  $available_units = array();

  // The current unit for this booking should also return as available
  $current_booking_state = isset($current_booking->booking_id) ? rooms_availability_assign_id($current_booking->booking_id, $current_booking->booking_status) : '';

  $valid_states = variable_get('rooms_valid_availability_states');

  if (isset($current_booking->booking_id)) {
    $ac = new AvailabilityAgent($start_date, $end_date, $group_size, 1, array($valid_states[ROOMS_AVAILABLE], $valid_states[ROOMS_ON_REQUEST], $valid_states[ROOMS_UNCONFIRMED_BOOKINGS]), $unit_types);
  }
  else {
    $ac = new AvailabilityAgent($start_date, $end_date, $group_size, 1, array($valid_states[ROOMS_AVAILABLE], $valid_states[ROOMS_ON_REQUEST], $valid_states[ROOMS_UNCONFIRMED_BOOKINGS], $current_booking_state), $unit_types);
  }
  $available_units = $ac->checkAvailability();

  return $available_units;   
}


/**
 * Form API validate callback for the room form
 */
function rooms_booking_edit_form_validate(&$form, &$form_state) {
  
  $booking = $form_state['rooms_booking'];

  if ($form_state['values']['rooms_start_date']['date']=='' || $form_state['values']['rooms_end_date']['date']=='' ) {
  } else {
    $date1 = new DateTime($form_state['values']['rooms_start_date']);
    $date2 = new DateTime($form_state['values']['rooms_end_date']);
    $diff = $date1->diff($date2);
    // If date1 > date2
    if ($diff->invert) {
      form_set_error('date_range', t('End date must be after start date'));
    }
  }
  
  if (!isset($form_state['values']['unit_id']) && (!isset($booking->unit_id))) {
      form_set_error('availability_fieldset', t('A unit has not been assigned for this booking'));    
  }


  //Notify field widgets to validate their data.
  field_attach_form_validate('rooms_booking', $booking, $form, $form_state);
  
}


/**
 * Form API submit callback for the Booking form.
 * 
 * @todo remove hard-coded link
 */
function rooms_booking_edit_form_submit(&$form, &$form_state) {
  // Get the client name and client id
  $client = explode(':', $form_state['values']['client']);
  $client_name = $client[0];
  $client_id = $client[1];
  // Put them back in $form_state so the entity controller builds the entity
  $form_state['values']['name'] = $client_name;
  $form_state['values']['customer_id'] = $client_id;
  
  // We also need appropriate named variables for start and end date
  // It's simpler to do this than change all the other code for now
  $form_state['values']['start_date'] = $form_state['values']['rooms_start_date'];
  $form_state['values']['end_date'] = $form_state['values']['rooms_end_date'];

  $start_date = $form_state['values']['rooms_start_date'];
  $end_date = $form_state['values']['rooms_end_date'];
  
  $unit = 0;
  if (isset($form_state['values']['unit_id'])) {
    $unit = $form_state['values']['unit_id'];
  } 
  
  // If we are dealing with a new booking
  if ($form_state['rooms_booking']->booking_id == '') {
  
    $booking = rooms_booking_create(array('type' => $form_state['rooms_booking']->type));
    $form_state['rooms_booking'] = $booking;

    $booking = entity_ui_controller('rooms_booking')->entityFormSubmitBuildEntity($form, $form_state);
    
    // Add in created and changed times.
    if ($booking->is_new = isset($booking->is_new) ? $booking->is_new : 0) {
      $booking->created = time();
    }
    $booking->changed = time();
  }
  else {
    $booking =  $form_state['rooms_booking'];
    
    // We are going to be updating the event - so the first step is to remove the
    // old event.
    if (($booking->unit_id != 0) && ($booking->start_date != '') && ($booking->end_date != '')) {
      // Create a calendar
      $uc = new UnitCalendar($booking->unit_id);
      // Create an event representing the event to remove
      $be = new BookingEvent($booking->unit_id,
                             rooms_availability_assign_id($booking->booking_id, $booking->booking_status),
                             new DateTime($booking->start_date),
                             new DateTime($booking->end_date));
      $uc->removeEvents(array($be));
    }
    
    $booking = entity_ui_controller('rooms_booking')->entityFormSubmitBuildEntity($form, $form_state);
    $booking->changed = time();
    
    // Get the unit id from the booking to be used further down as it is not in the form_values because
    // of how unit ids are assigned
    $unit = $booking->unit_id;
  }
  
  $booking->save();
  
  // We have a unit defined so lets block availability there
  if ($unit != 0) {
    // Set the event_id
    $id = rooms_availability_assign_id($booking->booking_id, $booking->booking_status);
    $sd = new DateTime($start_date);
    $ed = new DateTime($end_date);
    $ed->sub(new DateInterval('P1D'));
    
    // Create an event
    $be = new BookingEvent($unit, $id, $sd, $ed);
    
    // Create UnitCalednar
    $rc = new UnitCalendar($unit);
    $responses = $rc->updateCalendar(array($be));

    if ($responses[$id] == ROOMS_UPDATED) {
      drupal_set_message(t('Room Availability Updated'));
      $be->lock();
    }
    else {
      drupal_set_message(t('Room Availability could not be updated'), 'error');
    }
  }
  
  $form_state['redirect'] = 'admin/rooms/bookings';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function rooms_booking_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/bookings/booking/' . $form_state['rooms_booking']->booking_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a booking.
 *
 * @param $booking
 *   The booking to delete
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
 * Submit callback for booking_delete_form
 */
function rooms_booking_delete_form_submit($form, &$form_state) {
  $booking = $form_state['rooms_booking'];

  //Check if the booking had a unit associated with it and if so update the availability calendar
  if (isset($booking->unit_id) && isset($booking->start_date) && isset($booking->end_date)) {
      $uc = new UnitCalendar($booking->unit_id);
      // Create an event representing the event to remove
      $be = new BookingEvent($booking->unit_id,
                             rooms_availability_assign_id($booking->booking_id),
                             new DateTime($booking->start_date),
                             new DateTime($booking->end_date));
      $uc->removeEvents(array($be));
  }
  
  
  rooms_booking_delete($booking);

  drupal_set_message(t('The booking %name has been deleted.', array('%name' => $booking->name)));
  watchdog('rooms', 'Deleted booking %name.', array('%name' => $booking->name));

  $form_state['redirect'] = 'admin/rooms/bookings';
}



/**
 * Page to add Rooms
 *
 * @todo Pass this through a proper theme function
 */
function rooms_booking_add_page() {
  $controller = entity_ui_controller('rooms_booking');
  return $controller->addPage();
}


/**
 * Displays the list of available room types for orom creation.
 *
 * @ingroup themeable
 */
function theme_rooms_booking_add_list($variables) {
  $content = $variables['content'];
  $output = '';
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




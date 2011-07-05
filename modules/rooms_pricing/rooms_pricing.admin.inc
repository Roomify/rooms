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
class RoomsPricingRuleUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Pricing Rules',
      'description' => 'Add edit and update pricing rules (seasons, discounts, etc).',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      //'type' => MENU_NORMAL_ITEM,
      'weight' => 1,
    );
    
    
    // Allows users to create individual pricing rules as well as see what room type rules apply to
    // a room
    $items['admin/rooms/rooms/room/%room_to_price/pricing_rules'] = array(
      'title' => 'Pricing Rules',
      'description' => 'View and Manage Pricing Rules',
      'page callback'  => 'rooms_room_pricing_rule_overview',
      'page arguments' => array(4),
      'access callback'  => 'rooms_pricing_rule_access',
      'access arguments' => array('administer pricing rules'),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 40,
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),

    );
        
    // Menu item for adding pricing rules on individual rooms
    $items['admin/rooms/rooms/room/%room/pricing_rules/pricing_room'] = array(
      'title' => 'Add Pricing Rule',
      'description' => 'Add a new Pricing Rule',
      'page callback'  => 'rooms_pricing_rule_form_wrapper',
      'page arguments' => array(6, 4),
      'access callback'  => 'rooms_pricing_rule_access',
      'access arguments' => array('administer pricing rules'),
      'type' => MENU_LOCAL_ACTION,
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );
    
    // Menu item for adding pricing rules on room types
    $items[$this->path . '/add/pricing_type'] = array(
      'title' => 'Add Pricing Rule for Room Type',
      'description' => 'Add a new Pricing Rule',
      'page callback'  => 'rooms_pricing_rule_form_wrapper',
      'page arguments' => array(4),
      'access callback'  => 'rooms_pricing_rule_access',
      'access arguments' => array('administer pricing rules'),
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );
    

    //Menu item for adding pricing rules on multiple rooms (not related by type)
    $items[$this->path . '/add/multiple_rooms'] = array(
      'title' => 'Add Pricing Rule for multiple Rooms',
      'description' => 'Add a new Pricing Rule',
      'page callback'  => 'rooms_pricing_rule_form_wrapper',
      'page arguments' => array(4),
      'access callback'  => 'rooms_pricing_rule_access',
      'access arguments' => array('administer pricing rules'),
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );


    // Loading and editing Pricing Rule entities
    $items[$this->path . '/pricing_rule/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/pricing_rule/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'rooms_pricing_rule_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_pricing_rule_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing rooms
    $items['pricing_rule/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'rooms_pricing_rule_page_title',
      'title arguments' => array(1),
      'page callback' => 'rooms_pricing_rule_page_view',
      'page arguments' => array(1),
      'access callback' => 'rooms_pricing_rule_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    
    dpm($items);
    return $items;
  }
}
  
  
  
  
  
function rooms_room_pricing_rule_overview($room){
  dpm($room);
  return 'hello';
}
  
  



function rooms_pricing_rule_form_wrapper($rule_type, $room = null) {
  // Add the type to the starter values for our entity to define the entity
  // bundle instance
  $values = array('rule_type'=> $rule_type);
  $pricing_rule = rooms_pricing_rule_create($values);
  
  // Add the breadcrumb for the form's location.
  rooms_pricing_rule_set_breadcrumb();
  
  return drupal_get_form('rooms_pricing_rule_edit_form', $pricing_rule, $room, $rule_type);
}



/**
 * Form callback wrapper: create a PricingRule.
 *
 * @param $pricing_rule
 *   The RoomsPricingRule object being edited by this form.
 *
 * @see rooms_pricing_rule_edit_form()
 */
function rooms_pricing_rule_create_form_wrapper($type) {
  // Add the breadcrumb for the form's location.
  rooms_pricing_rule_set_breadcrumb();
  $room = rooms_pricing_rule_create(array('type' => $type));
  return drupal_get_form('rooms_pricing_rule_edit_form');
}


/**
 * Form callback wrapper: delete a room.
 *
 * @param $room
 *   The room object being edited by this form.
 *
 * @see rooms_room_edit_form()
 */
function rooms_pricing_rule_delete_form_wrapper($room) {
  // Add the breadcrumb for the form's location.
  //rooms_room_set_breadcrumb();
  return drupal_get_form('rooms_pricing_rule_delete_form', $room);
}


/**
 * Form callback: create or edit a room.
 *
 * @param $room
 *   The Room object to edit or for a create form an empty room object
 *     with only a room type defined.
 */
function rooms_pricing_rule_edit_form($form, &$form_state, $pricing_rule, $room = null, $rule_type) {
  
  
  $form['rule_type'] = array (
    '#type' => 'hidden',
    '#value' => $rule_type,
  );
  
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Pricing Rule Name'),
    '#default_value' => isset($room->name) ? $room->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['room_range'] = array (
    '#title' => t('Room Range'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
    '#description' => 'When should this rule apply.'
  );
  
  // We are dealing with the creation of a rule for multiple rooms 
  if ($rule_type == 'multiple_rooms' && $room == null) {    
    // Load all rooms as checkboxes - yes this is heavy and yes needs to be improved
    $rooms = rooms_room_ids();
    if (count($rooms) == 0 ){
      $form['room_range']['no_room_info'] = array(
        '#markup' => t('No rooms have been created. <a href="/admin/rooms/rooms">Go here to add some rooms.</a>'),
      );
      $form['room_range']['rooms'] = array (
      '#type' => 'hidden',
      '#value' => -1,
      );

    } else {
      $form['room_range']['rooms'] = array (
        '#type' => 'checkboxes',
        '#title' => t('Rooms'),
        '#options' => $rooms,
        '#description' => t('Choose the room types this rule should apply for'),      
      );
    }
  }
  
  if ($rule_type == 'pricing_room' && isset($room)) {
    // Dealing with just one room - set the field to hidden
    $form['room_range']['room'] = array (
      '#type' => 'hidden',
      '#value' => $room->room_id,
    );

    $form['room_range']['room_info'] = array(
      '#markup' => t('This pricing rule will apply to room @room', array('@room' => $room->name)),
    );
  }
  
  if ($rule_type == 'pricing_type' && $room == null) {
    $room_types = rooms_room_types_ids();

    if (count($room_types) == 0 ){
      $form['room_range']['no_types_info'] = array(
        '#markup' => t('No room types have been created. <a href="/admin/rooms/room_types">Go here to add some types.</a>'),
      );
      $form['room_range']['room_type'] = array (
      '#type' => 'hidden',
      '#value' => -1,
      );
    } else {
      $form['room_range']['room_type'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Room Types'),
        '#options' => $room_types,
        '#description' => t('Choose the room types this rule should apply for'),
      );
    }
  }

  
  $form['date_range'] = array (
    '#title' => t('Date Range'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  
  
  $form['date_range']['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start Date'),
    '#date_type' => DATE_DATETIME,
    '#date_format' => 'm/d/Y',
    '#date_increment' => 1,
    '#date_year_range' => '-1:+3',
    '#required' => TRUE,
  );
  
  $form['date_range']['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End Date'),
    '#date_type' => DATE_DATETIME,
    '#date_format' => 'm/d/Y',
    '#date_increment' => 1,
    '#date_year_range' => '-1:+3',
    '#required' => TRUE,
  );
    
  
  $form['action'] = array (
    '#title' => t('Action'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
  );
  
  $form['action']['operation'] = array(
    '#type' => 'select',
    '#title' => 'Operation',
    '#options' => array(
      'add' => 'Add to existing price',
      'substract' => 'Subtract from existing price',
      'replace' => 'Replace existing price',  
      
    ),
    '#default_value' =>  'replace',
  );
  
  $form['action']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => '',
    '#size' => '5',
    '#description' => 'Amount to apply for rule',
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  

  
  // Add the field related form elements.
  $form_state['rooms_pricing_rule'] = $pricing_rule;
  field_attach_form('rooms_pricing_rule', $pricing_rule, $form, $form_state);

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
    '#value' => t('Save Pricing Rule'),
    '#submit' => $submit + array('rooms_pricing_rule_edit_form_submit'),
  );
  
  if (!empty($pricing_rule->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete pricing rule'),
      '#suffix' => l(t('Cancel'), 'admin/rooms/pricing_rules'),
      '#submit' => $submit + array('rooms_pricing_rule_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_pricing_rule_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the room form
 */
function rooms_pricing_rule_edit_form_validate(&$form, &$form_state) {
    
  $pricing_rule = $form_state['rooms_pricing_rule'];

//  dpm($form_state);
  
  // Check date validity
  $date1 = new DateTime($form_state['values']['start_date']);
  $date2 = new DateTime($form_state['values']['end_date']);
  $diff = $date1->diff($date2);
  // If date1 > date2
  if ($diff->invert) {
    form_set_error('date_range', t('End date must be after start date'));
  }
  
  // Check for rooom types
  if (isset($form_state['values']['rooms'])) {
    if ($form_state['values']['rule_type'] == 'multiple_rooms' && $form_state['values']['rooms'] == -1) {
      form_set_error('room_range', t('%name: No rooms selected.', array('%name' => 'Rooms')));
    }
    elseif (array_sum($form_state['values']['rooms']) == 0) {
      form_set_error('room_range', t('%name: No rooms selected.', array('%name' => 'Rooms')));    
    }    
  }
  
  
  // Check for rule types
  if (isset($form_state['values']['room_type'])) {
    if ($form_state['values']['rule_type'] == 'room_type' && $form_state['values']['room_type'] == -1) {
      form_set_error('room_type', t('%name: No rooms selected.', array('%name' => 'Room Type')));
    }
    elseif (array_sum($form_state['values']['room_type']) == 0) {
      form_set_error('room_range', t('%name: No room types selected.', array('%name' => 'Rooms')));    
    }    
  }
  

  // Make sure amount entered is numeric
  if (!empty($form_state['values']['amount']) && !is_numeric($form_state['values']['amount'])) {
    form_set_error('amount', t('%name: you must enter a numeric value for the price.', array('%name' => 'Amount')));
  }
  

  
  // Notify field widgets to validate their data.
  field_attach_form_validate('rooms_pricing_rule', $pricing_rule, $form, $form_state);
  
}


/**
 * Form API submit callback for the Room form.
 * 
 * @todo remove hard-coded link
 */
function rooms_pricing_rule_edit_form_submit(&$form, &$form_state) {
  
  $pricing_rule = entity_ui_controller('rooms_pricing_rule')->entityFormSubmitBuildEntity($form, $form_state);
  
  // Save the Pricing Rule and go back to the list of pricing rules
  
  // Add in created and changed times.
  if ($pricing_rule->is_new = isset($pricing_rule->is_new) ? $pricing_rule->is_new : 0){
    $pricing_rule->created = time();
  }

  $pricing_rule->changed = time();
  
  $pricing_rule->save();
  $form_state['redirect'] = 'admin/rooms/pricing_rules';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function rooms_pricing_rule_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/pricing_rules/rule/' . $form_state['rooms_pricing_rule']->pricing_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a pricing rule.
 *
 * @param $room
 *   The room to delete
 *
 * @see confirm_form()
 */
function rooms_pricing_rule_delete_form($form, &$form_state, $pricing_rule) {
  $form_state['rooms_pricing_rule'] = $pricing_rule;

  $form['#submit'][] = 'rooms_pricing_rule_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete Room %name?', array('%name' => $room->name)),
    'admin/rooms/pricing_rules/rule',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for room_delete_form
 */
function rooms_pricing_rule_delete_form_submit($form, &$form_state) {
  $pricing_rule = $form_state['rooms_pricing_rule'];

  rooms_pricing_rule_delete($pricing_rule);

  drupal_set_message(t('The pricign rule %name has been deleted.', array('%name' => $pricing_rule->name)));
  watchdog('rooms_pricing', 'Deleted room %name.', array('%name' => $pricing_rule->name));

  $form_state['redirect'] = 'admin/rooms/pricing_rules';
}



/**
 * Page to add Pricing Rule
 *
 * @todo Pass this through a proper theme function
 */
function rooms_pricing_rule_add_page() {
  $controller = entity_ui_controller('rooms_pricing_rule');
  return $controller->addPage();
}


/**
 * Displays the list of available room types for orom creation.
 *
 * @ingroup themeable
 */
function theme_rooms_pricing_rule_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="room-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer room types')) {
      $output = '<p>' . t('Rooms cannot be added because you have not created any room types yet. Go to the <a href="@create-room-type">room type creation page</a> to add a new room type.', array('@create-room-type' => url('admin/rooms/room_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No room types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative rooms pages.
 */
function rooms_pricing_rule_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Rooms'), 'admin/rooms'),
    l(t('Rooms'), 'admin/rooms/rooms'),
  );

  drupal_set_breadcrumb($breadcrumb);
}




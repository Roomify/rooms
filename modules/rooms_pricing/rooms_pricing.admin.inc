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
    dpm($this->entityInfo);
    dpm($wildcard);
    dpm($id_count);

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
        
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a Pricing Rule',
      'description' => 'Add a new Pricing Rule',
      'page callback'  => 'rooms_pricing_rule_form_wrapper',
      'access callback'  => 'rooms_pricing_rule_access',
      'access arguments' => array('administer pricing rules'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'rooms_pricing.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

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
  
  
  /**
   * Create the markup for the add Room Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('rooms_pricing_rule_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: edit a Pricing Rule.
 *
 * @param $pricing_rule
 * *   The RoomsPricingRule object being edited by this form.
 *
 * @see rooms_pricing_rule_edit_form()
 */
function rooms_pricing_rule_form_wrapper() {
  
  $pricing_rule = rooms_pricing_rule_create();
  // Add the breadcrumb for the form's location.
  rooms_pricing_rule_set_breadcrumb();
  return drupal_get_form('rooms_pricing_rule_edit_form', $pricing_rule);
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
  return drupal_get_form('rooms_pricing_rule_edit_form', $room);
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
function rooms_pricing_rule_edit_form($form, &$form_state, $pricing_rule) {

  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Pricing Rule Name'),
    '#default_value' => isset($room->name) ? $room->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
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
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('rooms_pricing_rule', $room, $form, $form_state);
  
}


/**
 * Form API submit callback for the Room form.
 * 
 * @todo remove hard-coded link
 */
function rooms_pricing_rule_edit_form_submit(&$form, &$form_state) {
  
  $pricing_rule = entity_ui_controller('rooms_pricing_rule')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the Room and go back to the list of rooms
  
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




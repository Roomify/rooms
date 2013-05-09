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
class RoomsUnitUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Bookable Units',
      'description' => 'Add edit and update bookable units (e.g. rooms at a hotel or a villa).',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
      //'type' => MENU_NORMAL_ITEM,
      'weight' => 10,
    );

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a Bookable Unit',
      'description' => 'Add a new Unit',
      'page callback'  => 'rooms_unit_add_page',
      'access callback'  => 'rooms_unit_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'rooms_unit.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );


    // Add menu items to add each different type of room.
    foreach (rooms_unit_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'rooms_unit_create_form_wrapper',
        'page arguments' => array($type->type),
        'access callback' => 'rooms_unit_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'rooms_unit.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing Rooms entities
    $items[$this->path . '/unit/' . $wildcard] = array(
      'page callback' => 'rooms_unit_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_unit_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'rooms_unit.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/unit/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/unit/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'rooms_unit_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'rooms_unit_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'rooms_unit.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing rooms
    $items['unit/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'rooms_unit_page_title',
      'title arguments' => array(1),
      'page callback' => 'rooms_unit_page_view',
      'page arguments' => array(1),
      'access callback' => 'rooms_unit_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Bookable Unit Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }
    return theme('rooms_unit_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: edit a Bookable Unit.
 *
 * @param $unit
 *   The  RoomsUnit object being edited by this form.
 *
 * @see rooms_unit_edit_form()
 */
function rooms_unit_form_wrapper($unit) {
  // Add the breadcrumb for the form's location.
  rooms_unit_set_breadcrumb();
  return drupal_get_form('rooms_unit_edit_form', $unit);
}

/**
 * Form callback wrapper: create a Unit.
 *
 * @param $unit
 *   The Unit object being edited by this form.
 *
 * @see rooms_unit_edit_form()
 */
function rooms_unit_create_form_wrapper($type) {
  // Add the breadcrumb for the form's location.
  rooms_unit_set_breadcrumb();
  $unit = rooms_unit_create(array('type' => $type));
  return drupal_get_form('rooms_unit_edit_form', $unit);
}


/**
 * Form callback wrapper: delete a unit.
 *
 * @param $unit
 *   The unit object being edited by this form.
 *
 * @see rooms_unit_edit_form()
 */
function rooms_unit_delete_form_wrapper($unit) {
  // Add the breadcrumb for the form's location.
  rooms_unit_set_breadcrumb();
  return drupal_get_form('rooms_unit_delete_form', $unit);
}


/**
 * Form callback: create or edit a unit.
 *
 * @param $unit
 *   The RoomUnit object to edit or for a create form an empty unit object
 *     with only a unit type defined.
 */
function rooms_unit_edit_form($form, &$form_state, $unit) {
  
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Bookable Unit Name'),
    '#default_value' => isset($unit->name) ? $unit->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['base_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Unit Base Price'),
    '#default_value' => isset($unit->base_price) ? $unit->base_price : '',
    '#size' => '5',
    '#description' => 'Base price is per unit per night',
    '#maxlength' => 10,
  );

  $form['bookable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bookable'),
    '#description' => t('To remove unit from bookable units uncheck - this will not alter availability states but remove the unit from any searches.'),
    '#default_value' => isset($unit->bookable) ? $unit->bookable : 1,
  );

  $form['min_sleeps'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->min_sleeps) ? $unit->min_sleeps : 1,
    '#field_prefix' => 'Min: ',
    '#title' => t('Person sleeps'),
  );

  $form['max_sleeps'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->max_sleeps) ? $unit->max_sleeps : 1,
    '#field_prefix' => 'Max: ',
  );

  $form['min_children'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->min_children) ? $unit->min_children : 0,
    '#field_prefix' => 'Min: ',
    '#title' => t('Children sleeps'),
  );

  $form['max_children'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->max_children) ? $unit->max_children : 0,
    '#field_prefix' => 'Max: ',
  );

  $form['default_state'] = array(
    '#type' => 'select',
    '#title' => t('Bookable Unit Default State'),
    '#options' => array(
      ROOMS_NOT_AVAILABLE => 'Unavailable',
      ROOMS_AVAILABLE => 'Available',
      ROOMS_ON_REQUEST => 'Available on Request',
      ROOMS_ANON_BOOKED => 'Anonymous Booking'
    ),
    '#default_value' => isset($unit->default_state) ? $unit->default_state : 1,
    '#description' => t('Choose what state to put the unit in for the dates chosen above'),
  );

  $form['data']['#tree'] = TRUE;

  $form['data']['bed_arrangement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bed Arrangement'),
  );

  $form['data']['bed_arrangement']['singles'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->data['bed_arrangement']['singles']) ? $unit->data['bed_arrangement']['singles'] : '',
    '#title' => t('Single Beds'),
  );

  $form['data']['bed_arrangement']['doubles'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($unit->data['bed_arrangement']['doubles']) ? $unit->data['bed_arrangement']['doubles'] : '',
    '#title' => t('Double Beds'),
  );

  // If we are creating a new booking unit
  if ($unit->unit_id == '') {
    $form['multi'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create multiple units'),
      '#description' => t('Create multiple booking units allows you to quickly create the number of
                          required units that all have the same configutation - you can still personalise
                          each later on.'),
    );

    $form['multi']['multiple'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => 1,
      '#title' => t('How many booking units like this should we create'),
    );
  }

  // Add the field related form elements.
  $form_state['rooms_unit'] = $unit;
  field_attach_form('rooms_unit', $unit, $form, $form_state);

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
    '#value' => t('Save Unit'),
    '#submit' => $submit + array('rooms_unit_edit_form_submit'),
  );

  if (!empty($unit->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Unit'),
      '#suffix' => l(t('Cancel'), 'admin/rooms/units'),
      '#submit' => $submit + array('rooms_unit_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_unit_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the booking unit form
 */
function rooms_unit_edit_form_validate(&$form, &$form_state) {
  $unit = $form_state['rooms_unit'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('rooms_unit', $unit, $form, $form_state);

  // Validate price field
  if (!empty($form_state['values']['base_price']) && !is_numeric($form_state['values']['base_price'])) {
    form_set_error('Base price error', t('%name: you must enter a numeric value for the price.', array('%name' => 'Base Price')));
  }

  if (!empty($form_state['values']['multiple']) && !is_numeric($form_state['values']['multiple'])) {
    form_set_error('Multiple Rooms Error', t('%name: you must enter at least 1 as the number of multiple rooms.', array('%name' => 'Multiple Rooms')));
  }
}


/**
 * Form API submit callback for the Unit form.
 *
 * @todo remove hard-coded link
 */
function rooms_unit_edit_form_submit(&$form, &$form_state) {

  $multiple = 1;
  $units = array();

  if ($form_state['rooms_unit']->unit_id == '') {
    $multiple = $form_state['values']['multiple'];

    $name = $form_state['values']['name'];
    for ($i = 1; $i <= $multiple ; $i++ ) {
      $units[$i] = rooms_unit_create(array('type' => $form_state['rooms_unit']->type));
      $form_state['rooms_unit'] = $units[$i];
      if ($i>1) {
        $form_state['values']['name'] = $name . ' - ' . $i;
      }

      $units[$i] = entity_ui_controller('rooms_unit')->entityFormSubmitBuildEntity($form, $form_state);

      // Add in created and changed times.
      if ($units[$i]->is_new = isset($units[$i]->is_new) ? $units[$i]->is_new : 0) {
        $units[$i]->created = time();
      }

      $units[$i]->changed = time();
    }
  }
  else {
    $units[0] =  $form_state['rooms_unit'];
    $units[0] = entity_ui_controller('rooms_unit')->entityFormSubmitBuildEntity($form, $form_state);
    $units[0]->changed = time();
  }

  foreach ($units as $unit) {
    $unit->save();
  }
  $form_state['redirect'] = 'admin/rooms/units';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function rooms_unit_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/units/unit/' . $form_state['rooms_unit']->unit_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a unit.
 *
 * @param $unit
 *   The unit to delete
 *
 * @see confirm_form()
 */
function rooms_unit_delete_form($form, &$form_state, $unit) {
  $form_state['rooms_unit'] = $unit;

  $form['#submit'][] = 'rooms_unit_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete Unit %name?', array('%name' => $unit->name)),
    'admin/rooms/units/unit',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for unit_delete_form
 */
function rooms_unit_delete_form_submit($form, &$form_state) {
  $unit = $form_state['rooms_unit'];

  rooms_unit_delete($unit);

  drupal_set_message(t('The unit %name has been deleted.', array('%name' => $unit->name)));
  watchdog('rooms', 'Deleted unit %name.', array('%name' => $unit->name));

  $form_state['redirect'] = 'admin/rooms/units';
}



/**
 * Page to add Rooms
 *
 * @todo Pass this through a proper theme function
 */
function rooms_unit_add_page() {
  $controller = entity_ui_controller('rooms_unit');
  return $controller->addPage();
}


/**
 * Displays the list of available room types for orom creation.
 *
 * @ingroup themeable
 */
function theme_rooms_unit_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="unit-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer unit types')) {
      $output = '<p>' . t('Bookable Units cannot be added because you have not created any unit types yet. Go to the <a href="@create-unit-type">unit type creation page</a> to add a new unit type.', array('@create-unit-type' => url('admin/rooms/units/unit-types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No bookable unit types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}


/**
 * Sets the breadcrumb for administrative rooms pages.
 */
function rooms_unit_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Rooms'), 'admin/rooms'),
    l(t('Rooms'), 'admin/rooms/units'),
  );

  drupal_set_breadcrumb($breadcrumb);
}

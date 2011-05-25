<?php

/**
 * @file
 * Room editing UI.
 */

/**
 * UI controller.
 */
class RoomsRoomUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    
    $items[$this->path]['description'] = 'Manage rooms.';
    
    // Change the overview menu type for the list of rooms_rooms.
    $items[$this->basePath()]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to handle more types of entities
    $items[$this->basePath() . '/add']['title'] = 'Add a Room';
    $items[$this->basePath() . '/add']['description'] = 'Add a new Room';
    $items[$this->basePath() . '/add']['page callback']  = 'rooms_room_add_page';
    $items[$this->basePath() . '/add']['access callback']  = 'rooms_room_access';
    $items[$this->basePath() . '/add']['access arguments'] = array('edit');
    $items[$this->basePath() . '/add']['type'] = MENU_NORMAL_ITEM;
    $items[$this->basePath() . '/add']['weight'] = 20;
    unset($items[$this->basePath() . '/add']['title callback']);
    unset($items[$this->basePath() . '/add']['title arguments']);
    
    // Add menu items for each type of entity.
    foreach (rooms_room_get_types() as $type) {
      $items[$this->basePath() . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->type,
        'page callback' => 'rooms_room_form_wrapper',
        'page arguments' => array(rooms_room_create(array('type' => $type->type))),
        'access callback' => 'rooms_room_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'rooms_room.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }
    
    //Add menu items for editing
    unset($items['admin/rooms/rooms/manage/%rooms_room']);
    $items['admin/rooms/rooms/room/%rooms_room'] = array(
      //'title callback' => 'rooms_room_title',
      //'title arguments' => array(3),
      'page callback' => 'rooms_room_form_wrapper',
      'page arguments' => array(4),
      'access callback' => 'rooms_room_access',
      'access arguments' => array('edit', 4),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'rooms_room.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items['admin/rooms/rooms/room/%rooms_room/edit'] = array(
    'title' => 'Edit',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
  );
    //dpm($items, "Rooms Menu");
    return $items;
  }
  
  /**
   * Gives us the base path on which to "hang" the entity functionality
   */ 
  protected function basePath() {
    return 'admin/rooms/rooms';
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
        
    return theme('rooms_room_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a rooms_room.
 *
 * @param $rooms_room
 *   The rooms_room object being edited by this form.
 *
 * @see rooms_room_edit_form()
 */
function rooms_room_form_wrapper($rooms_room) {
  // Add the breadcrumb for the form's location.
  rooms_room_set_breadcrumb();

  return drupal_get_form('rooms_room_edit_form', $rooms_room);
}


/**
 * Form callback: create or edit a rooms_room.
 *
 * @param $rooms_room
 *   The rooms_room object to edit or for a create form an empty rooms_room object
 *     with only a rooms_room type defined.
 */
function rooms_room_edit_form($form, &$form_state, $room) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Room Name'),
    //'#default_value' => $rooms_room->title,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
    '#default_value' => isset($room->name) ? $room->name : '',
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bookable'),
    '#description' => t('To remove room from bookable rooms uncheck'),
    '#default_value' => isset($room->data['status']) ? $room->data['status'] : 1,
  );

  $form['data']['sleeps'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($room->data['sleeps']) ? $room->data['sleeps'] : 1,
    '#title' => t('Sleeps'),
  );


  $form['data']['bed_arrangement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bed Arrangement'),
    //'#description' => t('Describe bed arrangements'),
  );

  $form['data']['bed_arrangement']['singles'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($room->data['bed_arrangement']['singles']) ? $room->data['bed_arrangement']['singles'] : '',
    '#title' => t('Single Beds'),
  );

  $form['data']['bed_arrangement']['doubles'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($room->data['bed_arrangement']['doubles']) ? $room->data['bed_arrangement']['doubles'] : '',
    '#title' => t('Double Beds'),
  );
  
  $form['data']['bed_arrangement']['cot_available'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cot available'),
    '#default_value' => isset($room->data['bed_arrangement']['cot_available']) ? $room->data['bed_arrangement']['cot_available'] : 0,
  );
  
   $form['data']['pricing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pricing'),
    //'#description' => t('Describe bed arrangements'),
  );

  $form['data']['pricing']['baseprice'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($room->data['pricing']['baseprice']) ? $room->data['pricing']['baseprice'] : '',
    '#title' => t('Base Price'),
    '#description' =>  t('The default price to use when nothing else is set'),
  );

  $form['data']['pricing']['singlediscount'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => isset($room->data['pricing']['singlediscount']) ? $room->data['pricing']['singlediscount'] : '',
    '#title' => t('Discount for single occupancy'),
    '#postfix' => '%',
  );




  // Add the field related form elements.
  $form_state['rooms_room'] = $room;
  field_attach_form('rooms_room', $room, $form, $form_state);

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
    '#value' => t('Save room'),
    '#submit' => $submit + array('rooms_room_edit_form_submit'),
  );

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'rooms_room_edit_form_validate';
  return $form;
}

/**
 * Form API validate callback for the rooms_room form
 */
function rooms_room_edit_form_validate(&$form, &$form_state) {
  $room = $form_state['rooms_room'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('rooms_room', $room, $form, $form_state);
}

/**
 * Form API submit callback for the rooms_room form.
 */
function rooms_room_edit_form_submit(&$form, &$form_state) {
  
  $room = entity_ui_controller('rooms_room')->entityFormSubmitBuildEntity($form, $form_state);
  // Save and go back.
  $room->save();
  $form_state['redirect'] = 'admin/rooms/rooms';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_room_edit_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/rooms/manage/' . $form_state['rooms_room']->type . '/delete';
}


/**
 * Page to add Room Entities.
 *
 * @todo Pass this through a proper theme function
 */
function rooms_room_add_page() {
  $controller = entity_ui_controller('rooms_room');
  return $controller->addPage();
}


/**
 * Displays the list of available rooms_room types for rooms_room creation.
 *
 * @ingroup themeable
 */
function theme_rooms_room_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="rooms_room-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer rooms_room types')) {
      $output = '<p>' . t('You have not created any room types yet. Go to the <a href="@create-rooms_room-type">rooms_room type creation page</a> to add a new rooms_room type.', array('@create-rooms_room-type' => url('admin/rooms/room_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No rooms_room types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative rooms_room pages.
 */
function rooms_room_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Rooms'), 'admin/rooms'),
    l(t('Rooms'), 'admin/rooms/rooms'),
  );

  drupal_set_breadcrumb($breadcrumb);
}




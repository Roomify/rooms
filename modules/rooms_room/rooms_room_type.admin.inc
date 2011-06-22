<?php

/**
 * @file
 * RoomsRoom type editing UI.
 */

/**
 * UI controller.
 */
class RoomsRoomTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage room entity types, including adding
		and removing fields and the display of fields.';
		$items[$this->path]['weight'] = '3';

    return $items;
  }
}

/**
 * Generates the room type editing form.
 *
 * We are NOT following the rooms_room convention for naming functions to fit with the
 * EntityUIControlloer expectation of finding form names of the type entity_type_form.
 */
function rooms_room_type_form($form, &$form_state, $room_type, $op = 'edit') {

  if ($op == 'clone') {
    $room_type->label .= ' (cloned)';
    $room_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $room_type->label,
    '#description' => t('The human-readable name of this room type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($room_type->type) ? $room_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $room_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'rooms_room_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this room type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['extra_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting room switch'),
    '#default_value' => !empty($room_type->data['extra_data']),
  );
	
	$form['data']['base_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Room Base Price'),
    '#default_value' => isset($room_type->data['base_price']) ? $room_type->data['base_price'] : '',
    '#size' => '5',
    '#description' => t('Base price is per room per night - the price you set here will be the default price for all rooms of this type. You can then change the default base price on a per room basis'),
    '#maxlength' => 10,
  );
	
	$form['data']['sleeps'] = array(
    '#type' => 'textfield',
    '#title' => t('Sleeps'),
    '#default_value' => isset($room_type->data['sleeps']) ? $room_type->data['sleeps'] : '',
    '#size' => '5',
    '#description' => t('The default number of people that can sleeps in rooms of this type'),
    '#maxlength' => 2,
  );
	

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save room type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$room_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete room type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('rooms_room_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function rooms_room_type_form_submit(&$form, &$form_state) {
  //$room_type = entity_ui_form_submit_build_entity($form, $form_state);
  $room_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $room_type->save();
  $form_state['redirect'] = 'admin/rooms/room_types';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_room_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/room_types/manage/' . $form_state['rooms_room_type']->type . '/delete';
}

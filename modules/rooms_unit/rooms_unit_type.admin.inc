<?php

/**
 * @file
 * RoomsUnit type editing UI.
 */

/**
 * UI controller.
 */
class RoomsUnitTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage unit types, including adding
		and removing fields and the display of fields.';
    $items[$this->path]['weight'] = '3';
    return $items;
  }
}

/**
 * Generates the unit type editing form.
 *
 */
function rooms_unit_type_form($form, &$form_state, $unit_type, $op = 'edit') {

  if ($op == 'clone') {
    $unit_type->label .= ' (cloned)';
    $unit_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $unit_type->label,
    '#description' => t('The human-readable name of this unit type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($unit_type->type) ? $unit_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $unit_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'rooms_unit_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this unit type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
//  $form['data']['extra_data'] = array(
//    '#type' => 'checkbox',
//    '#title' => t('An interesting room switch'),
//    '#default_value' => !empty($room_type->data['extra_data']),
//  );

  $form['data']['base_price'] = array(
    '#type' => 'textfield',
    '#title' => t('Unit Base Price'),
    '#default_value' => isset($unit_type->data['base_price']) ? $unit_type->data['base_price'] : '',
    '#size' => '5',
    '#description' => t('Base price is per unit per night - the price you set here will be the default price for all units of this type. You can then change the default base price on a per unit basis'),
    '#maxlength' => 10,
  );
  
  $form['data']['sleeps'] = array(
    '#type' => 'textfield',
    '#title' => t('Sleeps'),
    '#default_value' => isset($unit_type->data['sleeps']) ? $unit_type->data['sleeps'] : '',
    '#size' => '5',
    '#description' => t('The default number of people that can sleeps in units of this type'),
    '#maxlength' => 2,
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save unit type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$unit_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete unit type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('rooms_unit_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function rooms_unit_type_form_submit(&$form, &$form_state) {
  $unit_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $unit_type->save();
  $form_state['redirect'] = 'admin/rooms/unit-types';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_unit_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/unit_types/manage/' . $form_state['rooms_unit_type']->type . '/delete';
}

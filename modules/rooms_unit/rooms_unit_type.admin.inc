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
    foreach ($items as &$item) {
      unset($item['access callback']);
      $item['access arguments'] = array('administer rooms_unit_type entities');
    }
    $items[$this->path]['description'] = 'Manage unit types, including adding and removing fields and the display of fields.';
    $items[$this->path]['weight'] = '3';
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }
}

/**
 * Generates the unit type editing form.
 */
function rooms_unit_type_form($form, &$form_state, $unit_type, $op = 'edit') {

  $form['#attributes']['class'][] = 'rooms-management-form rooms-unit-type-edit-form';

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'rooms_unit') . '/css/rooms_unit_type.css',
  );

  if ($op == 'clone') {
    $unit_type->label .= ' (cloned)';
    $unit_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Unit type name'),
    '#type' => 'textfield',
    '#default_value' => $unit_type->label,
    '#description' => t('The human-readable name of this unit type.'),
    '#required' => TRUE,
    '#size' => 30,
    '#weight' => -100,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($unit_type->type) ? $unit_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'rooms_unit_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this unit type. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => -99,
  );

  // Add the field related form elements.
  $form_state['rooms_unit_type'] = $unit_type;
  field_attach_form('rooms_unit_type', $unit_type, $form, $form_state);

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  if (variable_get('rooms_booking_manager_type_selector', ROOMS_DISPLAY_TYPE_SELECTOR_NO)) {
    $unit_types = variable_get('rooms_unit_type_selector', array());
    $form['unit_type_selector'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unit type selector'),
      '#description' => t('Unchecking this option the unit type will not appear in the unit-type selector form.'),
      '#options' => array(
        0 => 'Remove from unit type selector',
        1 => 'Add to unit type selector',
      ),
      '#default_value' => !empty($unit_types[$unit_type->type]),
      '#weight' => 100,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save unit type'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validate callback for rooms_unit_type_form form.
 *
 * @see rooms_unit_type_form()
 */
function rooms_unit_type_form_validate(&$form, &$form_state) {
  $unit_type = $form_state['rooms_unit_type'];
  field_attach_form_validate('rooms_unit_type', $unit_type, $form, $form_state);
}

/**
 * Form API submit callback for the type form.
 */
function rooms_unit_type_form_submit($form, &$form_state) {
  $unit_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $unit_type->save();

  // Get all unit types to show in unit type selector.
  $unit_types = variable_get_value('rooms_unit_type_selector');

  // If checked add unit type to list of unit types to show.
  if (!empty($form_state['values']['unit_type_selector'])) {
    $unit_types[$form_state['values']['type']] = $form_state['values']['label'];
    variable_set('rooms_unit_type_selector', $unit_types);
  }

  // If we are editing the unit type.
  else {
    if (isset($unit_types[$form_state['values']['type']])) {
      unset($unit_types[$form_state['values']['type']]);
      variable_set('rooms_unit_type_selector', $unit_types);
    }
  }

  $form_state['unit_type'] = $unit_type;

  $form_state['redirect'] = 'admin/rooms/units/unit-types';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_unit_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/units/unit_types/manage/' . $form_state['rooms_unit_type']->type . '/delete';
}

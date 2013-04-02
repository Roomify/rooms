<?php

/**
 * @file
 * RoomsBooking type editing UI.
 */

/**
 * UI controller.
 */
class RoomsBookingTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage booking types, including adding and removing fields and the display of fields.';
    $items[$this->path]['weight'] = '3';
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }
}

/**
 * Generates the Booking type editing form.
 *
 */
function rooms_booking_type_form($form, &$form_state, $booking_type, $op = 'edit') {

  if ($op == 'clone') {
    $booking_type->label .= ' (cloned)';
    $booking_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $booking_type->label,
    '#description' => t('The human-readable name of this booking type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($booking_type->type) ? $booking_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'rooms_booking_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this booking type. It must only contain lowercase letters, numbers, and underscores.'),
  );



  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Booking type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function rooms_booking_type_form_submit(&$form, &$form_state) {
  $booking_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $booking_type->save();
  $form_state['redirect'] = 'admin/rooms/bookings/booking-types';
}

/**
 * Form API submit callback for the delete button.
 */
function rooms_booking_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/rooms/bookings/booking_types/manage/' . $form_state['rooms_booking_type']->type . '/delete';
}

<?php

/**
 * @file
 * Rules integration for Rooms Booking Manager.
 */

/**
 * Implements hook_rules_action_info().
 */
function rooms_booking_manager_rules_action_info() {
  $actions = array();

  $actions['rooms_booking_cancel_order'] = array(
    'label' => t('Cancel the booking related to an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order from which to extract info for booking cancellation'),
      ),
    ),
    'group' => t('Rooms'),
    'callbacks' => array(
      'execute' => 'rooms_booking_cancel_order_booking',
    ),
  );

  $actions['rooms_booking_manager_price_apply'] = array(
    'label' => t('Apply price to a booking line item'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Rooms'),
    'callbacks' => array(
      'execute' => 'rooms_booking_manager_price_apply',
    ),
  );

  return $actions;
}

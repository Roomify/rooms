<?php

/**
 * @file
 * Providing extra functionality for the Room UI via views.
 */


/**
 * Implements hook_views_data().
 */
function rooms_booking_views_data_alter(&$data) {
  $data['rooms_bookings']['link_booking'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the booking.'),
      'handler' => 'rooms_booking_handler_link_field',
    ),
  );
  $data['rooms_bookings']['edit_booking'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the booking.'),
      'handler' => 'rooms_booking_handler_edit_link_field',
    ),
  );
  $data['rooms_bookings']['delete_booking'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the booking.'),
      'handler' => 'rooms_booking_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows rooms/booking/%booking_id/op
  $data['rooms_bookings']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this booking.'),
      'handler' => 'rooms_booking_handler_booking_operations_field',
    ),
  );
  $data['rooms_bookings']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The starting date for a booking'),
    'field' => array(
      'float' => TRUE,
      'handler' => 'rooms_booking_handler_date_field',
      'click sortable' => TRUE,
    ),
  );
  $data['rooms_bookings']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The end date for a booking'),
    'field' => array(
      'float' => TRUE,
      'handler' => 'rooms_booking_handler_date_field',
      'click sortable' => TRUE,
    ),
  );
  $data['rooms_bookings']['unit_id'] = array(
    'title' => t('Unit'),
    'help' => t('The unit this booking is in'),
    'field' => array(
      'handler' => 'rooms_booking_handler_unit_field',
    ),
  );
  $data['rooms_bookings']['type']['field'] = array(
    'title' => t('Booking  Type'),
    'help' => t('Booking  Type Label.'),
    'handler' => 'rooms_booking_handler_booking_type_field',
  );

}


/**
 * Implements hook_views_default_views().
 */
function rooms_booking_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'bookings';
  $view->description = 'A list of all bookings';
  $view->tag = 'rooms';
  $view->base_table = 'rooms_bookings';
  $view->human_name = 'Bookings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bookings';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer bookings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'booking_id' => 'booking_id',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'booking_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty Message';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No bookings have been created yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Bookings: Bookings ID */
  $handler->display->display_options['fields']['booking_id']['id'] = 'booking_id';
  $handler->display->display_options['fields']['booking_id']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['booking_id']['field'] = 'booking_id';
  /* Field: Bookings: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  /* Field: Bookings: Start Date */
  $handler->display->display_options['fields']['start_date']['id'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['start_date']['field'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['start_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['start_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['start_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['start_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['start_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['start_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['start_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['start_date']['date_format'] = 'custom';
  $handler->display->display_options['fields']['start_date']['custom_date_format'] = variable_get('rooms_date_format', 'd-m-Y');
  /* Field: Bookings: End Date */
  $handler->display->display_options['fields']['end_date']['id'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['end_date']['field'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['end_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['end_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['end_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['end_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['end_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['end_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['end_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['end_date']['date_format'] = 'custom';
  $handler->display->display_options['fields']['end_date']['custom_date_format'] = variable_get('rooms_date_format', 'd-m-Y');
  /* Field: Bookings: Unit */
  $handler->display->display_options['fields']['unit_id']['id'] = 'unit_id';
  $handler->display->display_options['fields']['unit_id']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['unit_id']['field'] = 'unit_id';
  $handler->display->display_options['fields']['unit_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['unit_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['unit_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['unit_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['unit_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['unit_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['unit_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['unit_id']['hide_alter_empty'] = 0;
  /* Field: Bookings: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Booking Type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 0;
  /* Field: Bookings: Order_id */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'Order ID';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['order_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['order_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['order_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['order_id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['order_id']['format_plural'] = 0;
  /* Field: Bookings: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'rooms_bookings';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  $handler->display->display_options['fields']['operations']['hide_alter_empty'] = 0;
  /* Sort criterion: Bookings: Bookings ID */
  $handler->display->display_options['sorts']['booking_id']['id'] = 'booking_id';
  $handler->display->display_options['sorts']['booking_id']['table'] = 'rooms_bookings';
  $handler->display->display_options['sorts']['booking_id']['field'] = 'booking_id';
  $handler->display->display_options['sorts']['booking_id']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/rooms/bookings/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Bookings List';
  $handler->display->display_options['menu']['description'] = 'List of bookings';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bookings';
  $handler->display->display_options['tab_options']['description'] = 'A list of all bookings. Bookings can be created, edited or deleted manually or can be created following a purchase from a client using Drupal Commerce.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views['bookings'] = $view;
  return $views;
}

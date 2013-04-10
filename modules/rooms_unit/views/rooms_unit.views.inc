<?php

/**
 * @file
 * Providing extra functionality for the Room UI via views.
 */


/**
 * Implements hook_views_data().
 */
function rooms_unit_views_data_alter(&$data) {

  $data['rooms_units']['type']['field'] = array(
    'title' => t('Booking Unit Type'),
    'help' => t('Booking Unit Type Label.'),
    'handler' => 'rooms_unit_handler_unit_type_field',
  );

  $data['rooms_units']['link_unit'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the unit.'),
      'handler' => 'rooms_unit_handler_link_field',
    ),
  );
  $data['rooms_units']['edit_unit'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the unit.'),
      'handler' => 'rooms_unit_handler_edit_link_field',
    ),
  );
  $data['rooms_units']['delete_unit'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the unit.'),
      'handler' => 'rooms_unit_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows rooms/unit/%unit_id/op
  $data['rooms_units']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this unit.'),
      'handler' => 'rooms_unit_handler_unit_operations_field',
    ),
  );

}


/**
 * Implements hook_views_default_views().
 */
function rooms_unit_views_default_views() {
  $views = array();

  $view = new view;

  $view->name = 'bookable_units';
  $view->description = 'A list of bookable units';
  $view->tag = 'rooms';
  $view->base_table = 'rooms_units';
  $view->human_name = 'Bookable Units';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bookable Units';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer bookable units';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'unit_id' => 'unit_id',
    'base_price' => 'base_price',
    'bookable' => 'bookable',
    'unit_id_1' => 'unit_id_1',
    'edit_unit' => 'edit_unit',
    'delete_unit' => 'delete_unit',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'unit_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'base_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'bookable' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'unit_id_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_unit' => array(
      'align' => '',
      'separator' => '',
    ),
    'delete_unit' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No bookable units have been created yet.';
  /* Field: Bookable Units: Bookable units ID */
  $handler->display->display_options['fields']['unit_id']['id'] = 'unit_id';
  $handler->display->display_options['fields']['unit_id']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['unit_id']['field'] = 'unit_id';
  $handler->display->display_options['fields']['unit_id']['label'] = 'ID';
  $handler->display->display_options['fields']['unit_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['unit_id']['alter']['replace_spaces'] = 0;
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
  $handler->display->display_options['fields']['unit_id']['format_plural'] = 0;
  /* Field: Bookable Units: Booking Unit Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['type']['field'] = 'type';
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
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 1;
  /* Field: Bookable Units: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
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
  /* Field: Bookable Units: Base_price */
  $handler->display->display_options['fields']['base_price']['id'] = 'base_price';
  $handler->display->display_options['fields']['base_price']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['base_price']['field'] = 'base_price';
  $handler->display->display_options['fields']['base_price']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['external'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['base_price']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['base_price']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['base_price']['alter']['html'] = 0;
  $handler->display->display_options['fields']['base_price']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['base_price']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['base_price']['hide_empty'] = 0;
  $handler->display->display_options['fields']['base_price']['empty_zero'] = 0;
  $handler->display->display_options['fields']['base_price']['format_plural'] = 0;
  /* Field: Bookable Units: Bookable */
  $handler->display->display_options['fields']['bookable']['id'] = 'bookable';
  $handler->display->display_options['fields']['bookable']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['bookable']['field'] = 'bookable';
  $handler->display->display_options['fields']['bookable']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['external'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bookable']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bookable']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bookable']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bookable']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['bookable']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['bookable']['hide_empty'] = 0;
  $handler->display->display_options['fields']['bookable']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bookable']['format_plural'] = 0;
  /* Field: Bookable Units: Link */
  $handler->display->display_options['fields']['link_unit']['id'] = 'link_unit';
  $handler->display->display_options['fields']['link_unit']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['link_unit']['field'] = 'link_unit';
  $handler->display->display_options['fields']['link_unit']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_unit']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_unit']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_unit']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_unit']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_unit']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_unit']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_unit']['empty_zero'] = 0;
  /* Field: Bookable Units: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
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
  /* Filter criterion: Bookable Units: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'rooms_units';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['type']['expose']['reduce'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/rooms/units/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Bookable Units List';
  $handler->display->display_options['menu']['description'] = 'A list of the units (rooms, etc) that can be booked for your accommodation.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Bookable Units';
  $handler->display->display_options['tab_options']['description'] = 'Create and administer units that can be booked, such as rooms for a hotel. Bookable units have availability and pricing information attached to them by the Rooms module and can have extra fields added via the standard Drupal Field API functionality.';
  $handler->display->display_options['tab_options']['weight'] = '-10';
  $handler->display->display_options['tab_options']['name'] = 'management';


  $views['bookable_units'] = $view;
  return $views;
}

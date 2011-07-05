<?php

/**
 * @file
 * Providing extra functionality for the Room UI via views.
 */


/**
 * Implements hook_views_data()
 */
/*function rooms_room_views_data_alter(&$data) { 
  $data['room']['link_room'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the room.'),
      'handler' => 'rooms_room_handler_link_field',
    ),
  );
  $data['room']['edit_room'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the room.'),
      'handler' => 'rooms_room_handler_edit_link_field',
    ),
  );
  $data['room']['delete_room'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the room.'),
      'handler' => 'rooms_room_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows rooms/room/%room_id/op
  $data['room']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this room.'),
      'handler' => 'rooms_room_handler_room_operations_field',
    ),
  );
}*/


/**
 * Implements hook_views_default_views().
 */
function rooms_pricing_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'rooms_pricing_rules';
  $view->description = 'A list of pricing rules';
  $view->tag = 'rooms_pricing';
  $view->base_table = 'rooms_pricing_rule';
  $view->human_name = 'Rooms Pricing Rules';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer pricing rules';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'pricing_id' => 'pricing_id',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'pricing_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Rooms Pricing Rule: Rooms pricing rule ID */
  $handler->display->display_options['fields']['pricing_id']['id'] = 'pricing_id';
  $handler->display->display_options['fields']['pricing_id']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['pricing_id']['field'] = 'pricing_id';
  $handler->display->display_options['fields']['pricing_id']['label'] = 'ID';
  $handler->display->display_options['fields']['pricing_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pricing_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pricing_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pricing_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pricing_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['pricing_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pricing_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pricing_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['pricing_id']['format_plural'] = 0;
  /* Field: Rooms Pricing Rule: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Rooms Pricing Rule: Start_date */
  $handler->display->display_options['fields']['start_date']['id'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['start_date']['field'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['label'] = 'Start Date';
  $handler->display->display_options['fields']['start_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['start_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['start_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['start_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['start_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['start_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['start_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['start_date']['empty_zero'] = 0;
  /* Field: Rooms Pricing Rule: End_date */
  $handler->display->display_options['fields']['end_date']['id'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['end_date']['field'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['label'] = 'End Date';
  $handler->display->display_options['fields']['end_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['end_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['end_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['end_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['end_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['end_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['end_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['end_date']['empty_zero'] = 0;
  /* Field: Rooms Pricing Rule: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['external'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['amount']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['amount']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['amount']['alter']['html'] = 0;
  $handler->display->display_options['fields']['amount']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['amount']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['amount']['hide_empty'] = 0;
  $handler->display->display_options['fields']['amount']['empty_zero'] = 0;
  $handler->display->display_options['fields']['amount']['format_plural'] = 0;
  /* Field: Rooms Pricing Rule: Operation */
  $handler->display->display_options['fields']['operation']['id'] = 'operation';
  $handler->display->display_options['fields']['operation']['table'] = 'rooms_pricing_rule';
  $handler->display->display_options['fields']['operation']['field'] = 'operation';
  $handler->display->display_options['fields']['operation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operation']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operation']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operation']['empty_zero'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/rooms/pricing_rules/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Pricing Rules';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'List';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['rooms_pricing_rules'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('ID'),
    t('.'),
    t(','),
    t('Name'),
    t('Start Date'),
    t('End Date'),
    t('Amount'),
    t('Operation'),
    t('Page'),
  );

  $views[] = $view;
  return $views;
}

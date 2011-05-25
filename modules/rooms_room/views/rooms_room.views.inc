<?php

/**
 * Views for the default rooms UI.
 */


/**
 * Implements hook_views_default_views().
 */
function rooms_room_views_default_views() {
  $views = array();
  
  $view = new view;
  
  $view->name = 'rooms';
  $view->description = 'A list of all rooms';
  $view->tag = 'rooms';
  $view->base_table = 'rooms_room';
  $view->human_name = 'Rooms';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Rooms';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer rooms';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'room_id' => 'room_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'room_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No rooms have been created yet';
  /* Field: Room: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'rooms_room';
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
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Room: Room ID */
  $handler->display->display_options['fields']['rooms_room_id']['id'] = 'rooms_room_id';
  $handler->display->display_options['fields']['rooms_room_id']['table'] = 'rooms_room';
  $handler->display->display_options['fields']['rooms_room_id']['field'] = 'rooms_room_id';
  $handler->display->display_options['fields']['rooms_room_id']['label'] = 'Edit';
  $handler->display->display_options['fields']['rooms_room_id']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['text'] = '<a href="/admin/rooms/rooms/room/[rooms_room_id]/edit">edit</a>';
  $handler->display->display_options['fields']['rooms_room_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rooms_room_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rooms_room_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rooms_room_id']['format_plural'] = 0;
  /* Field: Room: Room ID */
  $handler->display->display_options['fields']['rooms_room_id_1']['id'] = 'rooms_room_id_1';
  $handler->display->display_options['fields']['rooms_room_id_1']['table'] = 'rooms_room';
  $handler->display->display_options['fields']['rooms_room_id_1']['field'] = 'rooms_room_id';
  $handler->display->display_options['fields']['rooms_room_id_1']['label'] = 'Availability';
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['text'] = '<a href="/rooms/rooms/[rooms_room_id_1]/availability">manage availability</a>';
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rooms_room_id_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rooms_room_id_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rooms_room_id_1']['format_plural'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'rooms_admin_page');
  $handler->display->display_options['path'] = 'admin/rooms/rooms';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Rooms';
  $handler->display->display_options['menu']['description'] = 'Manage rooms and add new rooms';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Rooms';
  $handler->display->display_options['tab_options']['description'] = 'Manage rooms';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['rooms'] = array(
    t('Master'),
    t('Rooms'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No rooms have been created yet'),
    t('Name'),
    t('Edit'),
    t('<a href="/admin/rooms/rooms/room/[rooms_room_id]/edit">edit</a>'),
    t('.'),
    t(','),
    t('Availability'),
    t('<a href="/rooms/rooms/[rooms_room_id_1]/availability">manage availability</a>'),
    t('Page'),
  );

  $views[$view->name] = $view;

  return $views;

}

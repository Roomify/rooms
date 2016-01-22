<?php


/**
 * Implements hook_views_default_views().
 */
function rooms_views_default_views() {
  $views = array();
  $view = new view();

  $view->name = 'search_rooms_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_search_types';
  $view->human_name = 'Search Rooms Types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Search Rooms Types';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Indexed Type: Image */
  $handler->display->display_options['fields']['field_rooms_image']['id'] = 'field_rooms_image';
  $handler->display->display_options['fields']['field_rooms_image']['table'] = 'search_api_index_search_types';
  $handler->display->display_options['fields']['field_rooms_image']['field'] = 'field_rooms_image';
  $handler->display->display_options['fields']['field_rooms_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_rooms_image']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Indexed Type: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'search_api_index_search_types';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['link_to_entity'] = 0;
  /* Field: Indexed Type: Amenities */
  $handler->display->display_options['fields']['field_rooms_amenities']['id'] = 'field_rooms_amenities';
  $handler->display->display_options['fields']['field_rooms_amenities']['table'] = 'search_api_index_search_types';
  $handler->display->display_options['fields']['field_rooms_amenities']['field'] = 'field_rooms_amenities';
  $handler->display->display_options['fields']['field_rooms_amenities']['link_to_entity'] = 0;
  $handler->display->display_options['fields']['field_rooms_amenities']['view_mode'] = 'full';
  $handler->display->display_options['fields']['field_rooms_amenities']['bypass_access'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'search-rooms-types';

  $views['search_rooms_types'] = $view;

  $view->name = 'rooms_types';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'bat_types';
  $view->human_name = 'Rooms Types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Rooms Types';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'update any bat_type entity of bundle rooms';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Type: Type ID */
  $handler->display->display_options['fields']['type_id']['id'] = 'type_id';
  $handler->display->display_options['fields']['type_id']['table'] = 'bat_types';
  $handler->display->display_options['fields']['type_id']['field'] = 'type_id';
  /* Field: Type: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'bat_types';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Type: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'bat_types';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Published';
  $handler->display->display_options['fields']['status']['separator'] = '';
  /* Field: Type: Calendar links */
  $handler->display->display_options['fields']['calendars']['id'] = 'calendars';
  $handler->display->display_options['fields']['calendars']['table'] = 'bat_types';
  $handler->display->display_options['fields']['calendars']['field'] = 'calendars';
  $handler->display->display_options['fields']['calendars']['label'] = 'Calendars';
  /* Field: Type: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'bat_types';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/rooms/rooms-types';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Rooms Types';
  $handler->display->display_options['menu']['description'] = 'Manage Rooms Types and their Units';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  return $views;
}

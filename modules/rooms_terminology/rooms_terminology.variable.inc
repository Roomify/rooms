<?php
/**
 * @file
 * Variable API module. Definition for some xample variables
 */

/**
 * Implements hook_variable_info().
 */
function rooms_terminology_variable_info($options) {
  // Simple text
  $variables['variable_1'] = array(
    'type' => 'text',
    'title' => t('Simple text'),
    'default' => 'Example text.',
    'description' => t('Example of text variable.'),
    'group' => 'rooms_terminology',
  );
  // Simple number, demonstrates validate callback.
  $variables['variable_2'] = array(
    'type' => 'number',
    'title' => t('Number'),
    'default' => 0,
    'description' => t('Example of numeric variable.'),
    'group' => 'rooms_terminology',
  );
  
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
/*function rooms_terminology_variable_group_info() {
  $groups['rooms_terminology'] = array(
    'title' => t('Rooms Terminology'),
    'description' => t('Variable examples of different types.'),
    'access' => 'administer site configuration',
    'path' => array('admin/rooms/config/terminology'),
  );
  return $groups;
}*/


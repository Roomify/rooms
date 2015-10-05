<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows modules to deny or provide access for a user to perform a non-view
 * operation on an entity before any other access check occurs.
 *
 * Modules implementing this hook can return FALSE to provide a blanket
 * prevention for the user to perform the requested operation on the specified
 * entity. If no modules implementing this hook return FALSE but at least one
 * returns TRUE, then the operation will be allowed, even for a user without
 * role based permission to perform the operation.
 *
 * If no modules return FALSE but none return TRUE either, normal permission
 * based checking will apply.
 *
 * @param $op
 *   The request operation: update, create, or delete.
 * @param $entity
 *   The entity to perform the operation on.
 * @param $account
 *   The user account whose access should be determined.
 * @param $entity_type
 *   The machine-name of the entity type of the given $entity.
 *
 * @return
 *   TRUE or FALSE indicating an explicit denial of permission or a grant in the
 *   presence of no other denials; NULL to not affect the access check at all.
 */
function hook_rooms_entity_access($op, $entity, $account, $entity_type) {
  // No example.
}

/**
 * Allows modules to alter the conditions used on the query to grant view access
 * to a Rooms entity of the specified ENTITY TYPE.
 *
 * The Rooms module defines a generic implementation of hook_query_alter() to
 * determine view access for its entities, rooms_entity_access_query_alter().
 * This function is called by modules defining Rooms entities from their
 * view access altering functions to apply a standard set of permission based
 * conditions for determining a user's access to view the given entity.
 *
 * @param $conditions
 *   The OR conditions group used for the view access query.
 * @param $context
 *   An array of contextual information including:
 *   - account: the account whose access to view the entity is being checked
 *   - entity_type: the type of entity in the query
 *   - base_table: the name of the table for the entity type
 *
 * @see rooms_entity_access_query_alter()
 */
function hook_rooms_entity_access_condition_ENTITY_TYPE_alter() {
  // No example.
}

/**
 * Allows modules to alter the conditions used on the query to grant view access
 * to a Rooms entity.
 *
 * This hook uses the same parameters as the entity type specific hook but is
 * invoked after it.
 *
 * @see hook_rooms_entity_access_condition_ENTITY_TYPE_alter()
 */
function hook_rooms_entity_access_condition_alter() {
  // No example.
}

/**
 * Allows modules to use contextual information about bookings
 * to change what is shown to the user.
 *
 * @param &$string_suggestions - String suggestions, the suggestion with the
 *   highest index in the array will be used. The value at index 0 is the
 *   default value.
 * @param $context - Contextual information about the string.
 *
 * @return mixed
 */
function hook_rooms_string_alter(&$string_suggestions, $context) {
  if ($context['#purpose'] == 'rooms_create_line_item') {

    // Alter the line item label to add additional information about the unit.
    $string_suggestions[] = $string_suggestions[0] . ' maximum guests: ' .
                            $context['#data']['unit']['unit']->max_sleeps;
  }
}

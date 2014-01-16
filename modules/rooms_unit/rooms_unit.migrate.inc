<?php

/**
 * @file
 * Class MigrateDestinationRoomsUnit.
 */

/*
 * Migrate destination class to import bookable units.
 */
class MigrateDestinationRoomsUnit extends MigrateDestinationEntity {
  function __construct($bundle, array $options = array()) {
    parent::__construct('rooms_unit', $bundle, $options);
  }

  static public function getKeySchema() {
    return array(
      'unit_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination bookable unit',
      ),
    );
  }

  /**
   * Returns a list of fields available to be mapped for the rooms unit (bundle)
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array();

    // First the core (room_unit table) properties
    $fields['unit_id'] = t('Existing Unit ID');
    $fields['type'] = t('Unit Type');
    $fields['name'] = t('Unit Name');
    $fields['base_price'] = t('Base price');
    $fields['default_state'] = t('Default state');
    $fields['bookable'] = t('Default state');
    $fields['min_sleeps'] = t('Min sleeps');
    $fields['max_sleeps'] = t('Max sleeps');
    $fields['min_children'] = t('Min children');
    $fields['max_children'] = t('Max children');
    $fields['singles'] = t('Single beds');
    $fields['doubles'] = t('Double beds');
    $fields['data'] = t('Room unit data');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('RoomsUnit', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
  }

  /**
   * Put into the data field all stuff that must go there.
   *
   * @param $rooms_unit
   * @param \stdClass $source_row
   *  Raw source data object - passed through to prepare handlers.
   * @internal param $entity
   *  RoomsUnit object to build. Prefilled with any fields mapped in the Migration.*  RoomsUnit object to build. Prefilled with any fields mapped in the Migration.
   */
  public function prepare($rooms_unit, stdClass $source_row) {
    parent::prepare($rooms_unit, $source_row);

    // Store bed arrangement into data.
    $data = array(
      'bed_arrangement' => array(
        'singles' => $rooms_unit->singles,
        'doubles' => $rooms_unit->doubles,
      ),
    );

    $rooms_unit->data = $data;
  }

  /**
   * Imports a single rooms unit.
   *
   * @param stdClass $unit
   *  Rooms unit object to build. Prefilled with any fields mapped in the Migration.
   * @param stdClass $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array|bool
   *  Array of key fields (unit_id only in this case) of the node that was saved if
   *  successful. FALSE on failure.
   * @throws MigrateException
   */
  public function import(stdClass $unit, stdClass $row) {
    // Convert stdClass to RoomsUnit.
    $rooms_unit = new RoomsUnit((array)$unit);

    // Updating previously-migrated unit?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      if (isset($rooms_unit->unit_id)) {
        if ($rooms_unit->unit_id != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming unit_id !unit_id and map destination unit_id !destid1 don't match",
            array('!unit_id' => $rooms_unit->unit_id, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $rooms_unit->unit_id = $row->migrate_map_destid1;
      }
    }

    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($rooms_unit->unit_id)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination unit_id provided'));
      }
      $old_rooms_unit = rooms_unit_load($rooms_unit->unit_id);
      if (empty($old_rooms_unit)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but rooms_unit !unit_id does not exist',
          array('!unit_id' => $rooms_unit->unit_id)));
      }
    }

    if (!isset($rooms_unit->type)) {
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $rooms_unit->type = $this->bundle;
    }

    // Set some required properties.

    if ($migration->getSystemOfRecord() == Migration::SOURCE) {
      if (empty($rooms_unit->language)) {
        $rooms_unit->language = $this->language;
      }

      if (isset($rooms_unit->created)) {
        $created = MigrationBase::timestamp($rooms_unit->created);
      }
      else {
        $rooms_unit->created = REQUEST_TIME;
      }

      if (isset($created)) {
        $rooms_unit->created = $created;
      }

      if (!isset($rooms_unit->changed)) {
        $rooms_unit->changed = REQUEST_TIME;
      }
    }

    // Invoke migration prepare handlers
    $this->prepare($rooms_unit, $row);

    // Trying to update an existing rooms_unit
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      // Incoming data overrides existing data, so only copy non-existent fields
      foreach ($old_rooms_unit as $field => $value) {
        // An explicit NULL in the source data means to wipe to old value (i.e.,
        // don't copy it over from $old_rooms_unit)
        if (property_exists($rooms_unit, $field) && $rooms_unit->$field === NULL) {
          // Ignore this field
        }
        elseif (!isset($rooms_unit->$field)) {
          $rooms_unit->$field = $old_rooms_unit->$field;
        }
      }
    }

    if (isset($rooms_unit->unit_id)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('rooms_unit_save');
    rooms_unit_save($rooms_unit);
    migrate_instrument_stop('rooms_unit_save');

    if (isset($rooms_unit->unit_id)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }

      $return = array($rooms_unit->unit_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($rooms_unit, $row);

    return $return;
  }

  /**
   * Delete a batch of room units at once.
   *
   * @param $units_ids
   *  Array of rooms_unit IDs to be deleted.
   */
  public function bulkRollback(array $units_ids) {
    migrate_instrument_start('rooms_unit_delete_multiple');
    $this->prepareRollback($units_ids);
    rooms_unit_delete_multiple($units_ids);
    $this->completeRollback($units_ids);
    migrate_instrument_stop('rooms_unit_delete_multiple');
  }
}

/**
 * Base Migration class that imports bookable units from a csv file.
 *
 * Arguments:
 *   'type': The bundle of the bookable unit.
 *   'file': The filepath of the csv file.
 *
 * @see vacation_rental_example_content_migrate_api() as an example of use.
 */
class RoomsUnitMigration extends Migration {
  public function __construct($args) {
    parent::__construct($args);
    $this->description = t('Import Rooms units of type "@type" from the file @file',
      array('@type' => $args['type'], '@file' => $args['file']));

    // Create a map object for tracking the relationships between source rows
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'row_id' => array(
          'type' => 'int',
          'length' => 10,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationRoomsUnit::getKeySchema()
    );

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($args['file'], $this->unit_csvcolumns(), array('header_rows' => 1));

    // Create a MigrateDestination object.
    $this->destination = new MigrateDestinationRoomsUnit($args['type']);

    // Field mappings
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('base_price', 'base_price');
    $this->addFieldMapping('default_state', 'default_state');
    $this->addFieldMapping('bookable', 'bookable');
    $this->addFieldMapping('min_sleeps', 'min_sleeps');
    $this->addFieldMapping('max_sleeps', 'max_sleeps');
    $this->addFieldMapping('min_children', 'min_children');
    $this->addFieldMapping('max_children', 'max_children');

    // Bed arrangement, will be automatically inserted into ->data['bed_arrangement'].
    $this->addFieldMapping('singles', 'singles');
    $this->addFieldMapping('doubles', 'doubles');

    $this->addUnmigratedDestinations(
      array(
        'data',
        'path',
        'type',
      )
    );
  }

  static public function unit_csvcolumns() {
    //'Row ID','Name','Base price','Default state','Bookable','Min sleeps','Max sleeps','Min children','Max children','Single beds','Double beds'
    $columns[0] = array('row_id', 'Row ID');
    $columns[1] = array('name', 'Name');
    $columns[2] = array('base_price', 'Base price');
    $columns[3] = array('default_state', 'Default state');
    $columns[4] = array('bookable', 'Default state');
    $columns[5] = array('min_sleeps', 'Min sleeps');
    $columns[6] = array('max_sleeps', 'Max sleeps');
    $columns[7] = array('min_children', 'Min children');
    $columns[8] = array('max_children', 'Max children');
    $columns[9] = array('singles', 'Single beds');
    $columns[10] = array('doubles', 'Double beds');
    return $columns;
  }
}

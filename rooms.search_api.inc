<?php
/**
 * @file
 * rooms.search_api.inc
 */

/**
 * Implements hook_default_search_api_index().
 */
function rooms_default_search_api_index() {
  $items = array();
  $items['search_types'] = entity_import('search_api_index', '{
    "name" : "Search Types",
    "machine_name" : "search_types",
    "description" : null,
    "server" : "search_rooms",
    "item_type" : "bat_type",
    "options" : {
      "datasource" : { "bundles" : [ "rooms" ] },
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "field_rooms_amenities" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_rooms_sleeps" : { "type" : "integer" },
        "name" : { "type" : "text" },
        "search_api_language" : { "type" : "string" },
        "type" : { "type" : "string" },
        "type_id" : { "type" : "integer" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_bundle_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "bundles" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : false } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : { "name" : true } }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : { "name" : true },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : { "name" : true },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : { "name" : true },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        },
        "search_api_highlighting" : {
          "status" : 0,
          "weight" : "35",
          "settings" : {
            "prefix" : "\\u003Cstrong\\u003E",
            "suffix" : "\\u003C\\/strong\\u003E",
            "excerpt" : 1,
            "excerpt_length" : "256",
            "exclude_fields" : [],
            "highlight" : "always"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function rooms_default_search_api_server() {
  $items = array();
  $items['search_rooms'] = entity_import('search_api_server', '{
    "name" : "Search Rooms",
    "machine_name" : "search_rooms",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "1",
      "partial_matches" : 0,
      "indexes" : { "search_types" : {
          "type_id" : {
            "table" : "search_api_db_search_types",
            "column" : "type_id",
            "type" : "integer",
            "boost" : "1.0"
          },
          "type" : {
            "table" : "search_api_db_search_types",
            "column" : "type",
            "type" : "string",
            "boost" : "1.0"
          },
          "name" : {
            "table" : "search_api_db_search_types_text",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_rooms_sleeps" : {
            "table" : "search_api_db_search_types",
            "column" : "field_rooms_sleeps",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_rooms_amenities" : {
            "table" : "search_api_db_search_types_field_rooms_amenities",
            "column" : "value",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_search_types",
            "column" : "search_api_language",
            "type" : "string",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');

  return $items;
}

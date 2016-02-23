<?php

/**
 * @file rooms.addons.inc
 *
 * Display and manage available add-ons.
 */

define ('ROOMIFY_STORE_URI', 'https://store.roomify.us');

/**
 * Callback for /admin/rooms/addons
 */
function rooms_addons() {
  drupal_add_css(drupal_get_path('module', 'rooms') . '/css/rooms_addons.css');
  $output = array(
    'title' => array(
      '#type' => 'markup',
      '#markup' => '<h1>Roomify Add-Ons</h1>',
    ),
    'intro' => array(
      '#type' => 'markup',
      '#markup' => '<p>Roomify (the company behind the Rooms module) provides the following add-on functionality for your Rooms installation.
                    You are welcome to <a href="https://roomify.us/get-started">ask any questions about these modules and services or get
                    in touch for customization services.</a>',
    ),
  );

  // Retrieve the available add-ons.
  $request = drupal_http_request(ROOMIFY_STORE_URI . '/roomify/products/addons?module=rooms&version=1');
  $products = drupal_json_decode($request->data);
  $license_keys = variable_get('rooms_addons_licenses', array());

  foreach ($products as $id => $product) {
    $module = $product['module'];

    // Output installed version if present.
    $product['installed_version'] = FALSE;
    if (module_exists($module)) {
      $info = system_get_info('module', $module);
      $product['installed_version'] = $info['version'];
    }

    // Output license key if present.
    $product['license_key'] = 'UNLICENSED';
    if (isset($license_keys[$module])) {
      $product['license_key'] = $license_keys[$module];
    }

    // Build updates link.
    $product['update_available'] = FALSE;
    if (($product['installed_version'] != $product['latest_version']) && isset($license_keys[$module])) {
      $product['update_available'] = TRUE;
      $product['updates_link'] = '<a target="_blank" class="rooms-addons-updates-link" href="' . ROOMIFY_STORE_URI . '/user' . '">Download updates</a>';
    }

    // Build purchase link.
    $product['purchase_link'] = '<a target="_blank" class="rooms-addons-purchase-link" href="' . $product['link'] . '">Purchase now</a>';

    // Theme output.
    $output[$id] = array(
      '#type' => 'markup',
      '#markup' => theme('rooms_addon', $product)
    );
  }

  return $output;
}

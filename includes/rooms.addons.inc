<?php

/**
 * @file rooms.addons.inc
 *
 * Display and manage available add-ons.
 */


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

  // Bail out if we couldn't retrieve product info.
  if (empty($products)) {
    drupal_set_message('Oh no! We were temporarily unable to retrieve available add-ons! Please try again in a few minutes.', 'warning');
    return $output;
  }

  $license_keys = variable_get('rooms_addons_licenses', array());

  foreach ($products as $id => $product) {
    $module = $product['module'];

    // Output installed version if present.
    $product['installed_version'] = FALSE;
    if (module_exists($module)) {
      $info = system_get_info('module', $module);
      if (isset($info['version'])) {
        $product['installed_version'] = $info['version'];
      } else {
        $product['installed_version'] = $info['core'];
      }
    }

    // Output license key if present.
    $form = drupal_get_form('roomify_addon_license_' . $module, $module);
    $product['license_key'] = render($form);

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

/**
 * License form callback.
 */
function roomify_addon_license($form, &$form_state, $module) {
  $license_keys = variable_get('rooms_addons_licenses', array());
  $form = array(
    'container' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('container-inline'),
      ),
      'module' => array(
        '#type' => 'value',
        '#value' => $module,
      ),
      'license_key' => array(
        '#type' => 'textfield',
        '#size' => 25,
        '#default_value' => isset($license_keys[$module]) ? $license_keys[$module] : '',
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => empty($license_keys[$module]) ? t('Add license key') : t('Update license key'),
      ),
    ),
    '#submit' => array(
      'roomify_addon_license_submit',
    ),
  );

  return $form;
}

/**
 * License form submit handler.
 */
function roomify_addon_license_submit($form, $form_state) {
  if (isset($form_state['values']['module']) && isset($form_state['values']['license_key'])) {
    $license_keys = variable_get('rooms_addons_licenses', array());
    $license_keys[$form_state['values']['module']] = $form_state['values']['license_key'];
    variable_set('rooms_addons_licenses', $license_keys);
  }
}

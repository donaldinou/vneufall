<?php
// $Id: thumb.admin.inc,v 1.1 2008/11/24 09:28:45 davyvandenbremt Exp $

/**
 * Menu callback - Get an overview of all presets.
 */
function thumb_presets_list() {
  $header = array(t('Preset'), t('Operations'));

  $rows = array();

  $presets = thumb_presets();

  foreach ($presets as $id => $preset) {
    $links = array();
    $links[] = l(t('Edit'), 'admin/build/thumb/'. $preset->pid .'/edit');
    $links[] = l(t('Delete'), 'admin/build/thumb/'. $preset->pid .'/delete');
    $links[] = l(t('Flush'), 'admin/build/thumb/'. $preset->pid .'/flush');

    $rows[] = array(
      $id,
      implode('&nbsp;&nbsp;&nbsp;', $links)
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No presets available.'), 'colspan' => '2', 'class' => 'message'));
  }

  return theme('table', $header, $rows);
}

/**
 * Menu callback - Show preset form (add and delete).
 *
 * @param $preset
 *   The preset to edit (if in edit mode).
 */
function thumb_presets_form(&$formstate, $preset = NULL) {
  $form = array();

  if ($preset) {
    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $preset->pid
    );
  }

  /**
   * General preset info.
   */
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Info'),
    '#collapsible' => TRUE,
  );
  $form['info']['identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#description' => t('The title you enter here appears on the page.'),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#element_validate' => array('thumb_element_identifier_validate'),
    '#default_value' => isset($preset) ? $preset->identifier : '',
    '#disabled' => $preset ? TRUE : FALSE,
  );
  
  $form['info']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('The text you enter here appears on the page.'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => isset($preset) ? $preset->description : '',
  );

  /**
   * Preset settings.
   */
  $form['params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parameters'),
    '#collapsible' => TRUE,
  );
  $form['params']['w'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum width'),
    '#description' => t('Maximum width of output thumbnail in pixels.'),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('thumb_element_numeric_validate'),
    '#default_value' => isset($preset) && isset($preset->data['w']) ? $preset->data['w'] : '',
  );
  $form['params']['h'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum height'),
    '#description' => t('Maximum width of output thumbnail in pixels.'),
    '#size' => 4,
    '#maxlength' => 4,
    '#element_validate' => array('thumb_element_numeric_validate'),
    '#default_value' => isset($preset) && isset($preset->data['h']) ? $preset->data['h'] : '',
  );
  $form['params']['f'] = array(
    '#type' => 'select',
    '#title' => t('Format'),
    '#description' => t('Output image format.'),
    '#options' => array(-1 => t('Default'), 'jpeg' => 'jpeg', 'png' => 'png', 'gif' => 'gif'),
    '#default_value' => isset($preset) && isset($preset->data['f']) ? $preset->data['f'] : -1,
  );
  $form['params']['q'] = array(
    '#type' => 'textfield',
    '#title' => t('Quality'),
    '#description' => t('JPEG compression (1=worst, 95=best, 75=default)'),
    '#size' => 2,
    '#maxlength' => 2,
    '#element_validate' => array('thumb_element_numeric_validate'),
    '#default_value' => isset($preset) && isset($preset->data['q']) ? $preset->data['q'] : '',
  );

  /**
   * Buttons.
   */
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for the preset form.
 */
function thumb_presets_form_submit($form, &$form_state) {
  // Gathering all settings in $params arary.
  $params = array();
  foreach ($form['params'] as $key => $info) {
    if (!empty($form_state['values'][$key]) && $form_state['values'][$key] != -1) {
      $params[$key] = $form_state['values'][$key];
    }
  }
  
  // Serializing the array for storage.
  $params = serialize($params);

  // No preset id set; we're inserting a new one.
  if (!isset($form_state['values']['pid'])) {
    $sql = "INSERT INTO {thumb_preset} (identifier, description, data) VALUES ('%s', '%s', '%s')";
    db_query($sql, $form_state['values']['identifier'], $form_state['values']['description'], $params);
    drupal_set_message(t('Preset %identifier was created.', array('%identifier' => $form_state['values']['identifier'])));
  }
  // Preset id set; we're editing an existing one.
  else {
    $sql = "UPDATE {thumb_preset} SET identifier = '%s', description = '%s', data = '%s' WHERE pid = %d";
    db_query($sql, $form_state['values']['identifier'], $form_state['values']['description'], $params, $form_state['values']['pid']);
    drupal_set_message(t('Preset %identifier was updated.', array('%identifier' => $form_state['values']['identifier'])));
  }

  // Redirecting to preset overview.
  $form_state['redirect'] = 'admin/build/thumb';
}

/**
 * Element validation handler - Validate if an element's value is a valid identifier (alphanumeric, _ and -).
 */
function thumb_element_identifier_validate($element, &$form_state) {
  // Check for duplicates.
  $presets = thumb_presets();

  if (in_array($element['#value'], array_keys($presets))) {
    form_set_error($element['#name'], t('The identifier you have chosen is already in use.'));
  }

  // Check for illegal characters.
  if (preg_match('/[^0-9a-zA-Z_\-]/', $element['#value'])) {
    form_set_error($element['#name'], t('Please only use alphanumic characters, underscores (_), and hyphens (-) for identifiers.'));
  }
}

/**
 * Element validation handler - Validate if an element's value is numeric.
 */
function thumb_element_numeric_validate($element, &$form_state) {
  if (!empty($element['#value']) && !is_numeric($element['#value'])) {
    form_set_error($element['#name'], t('Please specify a numeric value for !field', array('!field' => $element['#title'])));
  }
}

/**
 * Menu callback - Show preset delete confirmation form.
 *
 * @param $preset
 *   The preset to delete.
 */
function thumb_delete_confirm_form(&$form_state, $preset) {
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $preset->pid,
  );
  $form['identifier'] = array(
    '#type' => 'value',
    '#value' => $preset->identifier,
  );

  return confirm_form($form,
    t('Are you sure you want to delete %identifier?', array('%identifier' => $preset->identifier)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/build/thumb',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for the preset delete form.
 */
function thumb_delete_confirm_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    thumb_preset_flush($form_state['values']['identifier']);
    db_query("DELETE FROM {thumb_preset} WHERE pid = %d", $form_state['values']['pid']);
  }

  $form_state['redirect'] = 'admin/build/thumb';
}

/**
 * Menu callback - Show preset flush confirmation form.
 *
 * @param $preset
 *   The preset to flush thumbnails for.
 */
function thumb_flush_confirm_form(&$form_state, $preset) {
  $form['identifier'] = array(
    '#type' => 'value',
    '#value' => $preset->identifier,
  );

  return confirm_form($form,
    t('Are you sure you want to flush %identifier?', array('%identifier' => $preset->identifier)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/build/thumb',
    t('This action cannot be undone.'),
    t('Flush'),
    t('Cancel')
  );
}

/**
 * Submit handler for the flush confirmation form.
 */
function thumb_flush_confirm_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    thumb_preset_flush($form_state['values']['identifier']);
  }

  $form_state['redirect'] = 'admin/build/thumb';
}

/**
 * Menu callback - Flush all thumbnails generated with a certain preset.
 *
 * @param $presetname
 *   The preset to flush thumbnails for.
 */
function thumb_preset_flush($presetname) {
  $presetdir = realpath(file_directory_path() .'/thumb/'. $presetname);
  if (is_dir($presetdir)) {
    _thumb_recursive_delete($presetdir);
  }
}

/**
 * Recursively delete images under a certain path.
 *
 * @param $path
 *   The path to delete images from.
 */
function _thumb_recursive_delete($path) {
  if (is_file($path) || is_link($path)) {
    unlink($path);
  }
  elseif (is_dir($path)) {
    $d = dir($path);
    while (($entry = $d->read()) !== FALSE) {
      if ($entry == '.' || $entry == '..') continue;
      $entry_path = $path .'/'. $entry;
      _thumb_recursive_delete($entry_path);
    }
    $d->close();
    rmdir($path);
  }
  else {
    watchdog('thumb', 'Unknown file type(%path) stat: %stat ', array('%path' => $path, '%stat' => print_r(stat($path), 1)), WATCHDOG_ERROR);
  }
}
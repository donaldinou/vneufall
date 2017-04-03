<?php

/**
 * Menu callback for handling thumb requests.
 */
function thumb() {
  $args = func_get_args();
  $preset = check_plain(array_shift($args));
  $path = implode('/', $args);
  _thumb($preset, $path);
}

function _thumb($presetname, $path) {
  $parts = explode('.', $path);
  $bare_path = $parts[0];
  
  $pathinfo = pathinfo($path);

  $presets = thumb_presets();
  if (!array_key_exists($presetname, $presets)) {
    // Send a 404 if we don't know of a preset.
    header("HTTP/1.0 404 Not Found");
    exit;
  }

  $preset = $presets[$presetname];

  require_once(drupal_get_path('module', 'thumb') .'/phpThumb/phpthumb.class.php');

  $thumb = new phpThumb();
  $thumb->src = thumb_decompress_string($bare_path);

  foreach ($preset->data as $setting => $value) {
    $thumb->{$setting} = $value;
  }

  $thumb->config_output_format = $pathinfo['extension'];
  $thumb->config_error_die_on_error = TRUE;
  $thumb->config_document_root = '';
  $thumb->config_temp_directory = file_directory_temp();
  $thumb->config_cache_directory = file_directory_temp() .'/cache/';
  $thumb->config_cache_disable_warning = TRUE;

  // Build the destination folder tree if it doesn't already exists.
  $root = realpath((getenv('DOCUMENT_ROOT') && ereg('^'. preg_quote(realpath(getenv('DOCUMENT_ROOT'))), realpath(__FILE__))) ? getenv('DOCUMENT_ROOT') : str_replace(dirname(@$_SERVER['PHP_SELF']), '', str_replace(DIRECTORY_SEPARATOR, '/', dirname(__FILE__))));
  
  $dir = $root .'/'. file_directory_path() .'/thumb/'. $presetname .'/'. $pathinfo['dirname'];
  if (!file_check_directory($dir, FILE_CREATE_DIRECTORY) && !mkdir($dir, 0775, TRUE)) {
    watchdog('imagecache', 'Failed to create imagecache directory: %dir', array('%dir' => $dir), WATCHDOG_ERROR);
    return FALSE;
  }

  $thumb->cache_filename = $dir .'/'. $pathinfo['basename'] .'.'. $pathinfo['extension'];

  if ($thumb->GenerateThumbnail()) {
    $thumb->RenderToFile($thumb->cache_filename);
  }
  else {
    die('Failed: '. $thumb->error);
  }

  if (is_file($thumb->cache_filename)) {
    thumb_transfer($thumb->cache_filename);
  }
}
<?php
// $Id: views_handler_field_upload_fid.inc,v 1.6 2009/04/07 22:48:03 merlinofchaos Exp $
/**
 * Field handler to provide a list of roles.
 */
class views_handler_field_immobilier_picture extends views_handler_field {
	private $files;

	function construct() {
		parent::construct();
	}

	function allow_advanced_render() {
		return FALSE;
	}

	function option_definition() {
		$options = parent::option_definition();
		return $options;
	}

	function options_form(&$form, &$form_state) {
		parent::options_form($form, $form_state);
		$formatters = array(
			'image_plain' 		=> t('Image'),
			'image_nodelink' 	=> t('Image linked to node'),
			'image_imagelink' 	=> t('Image linked to file'),
			'path_plain' 		=> t('Path to file'),
			'url_plain' 		=> t('URL to file'),
		);
		
		if(module_exists("imagecache")) {
			$formatters['imagecache'] = array();
			foreach (imagecache_presets() as $preset) {
				$formatters['imagecache'] = array_merge($formatters['imagecache'], array(
					'imagecache_'.$preset['presetname'] .'_default' 	=> t('@preset image', array('@preset' => $preset['presetname'])),
					'imagecache_'.$preset['presetname'] .'_linked' 		=> t('@preset image linked to node', array('@preset' => $preset['presetname'])),
					'imagecache_'.$preset['presetname'] .'_imagelink' 	=> t('@preset image linked to image', array('@preset' => $preset['presetname'])),
					'imagecache_'.$preset['presetname'] .'_path' 		=> t('@preset file path', array('@preset' => $preset['presetname'])),
					'imagecache_'.$preset['presetname'] .'_url' 		=> t('@preset URL', array('@preset' => $preset['presetname'])),
				));
			}
		}
			
		$form['format'] = array(
		  '#title' => t('Format'),
		  '#type' => 'select',
		  '#options' => $formatters,
		  '#required' => TRUE,
		  '#default_value' => $this->options['format'],
		  '#weight' => 4,
		);
		
	}

	function pre_render($values) {
		$vids = array();
		$this->items = array();

		foreach ($values as $result) 
			$vids[] = $result->{$this->field_alias};
		$vids = array_filter($vids);
		
		if ($vids) { 
			/*$sql = "SELECT i.vid, i.nid, f.*
			FROM {files} f 
			JOIN {immo_files} i ON i.fid = f.fid
			WHERE f.fid IN (
				SELECT SUBSTRING(MIN(CONCAT(LPAD(i.weight,10,'0'), i.fid)),11) as fid 
				FROM {immo_files} i 
				WHERE i.vid IN (" . implode(', ', $vids) . ")
				GROUP BY i.vid
			)";*/
			
			$sql = "SELECT 
				i.nid, 
				i.vid, 
				SUBSTRING(MIN(CONCAT(LPAD(i.weight,10,'0'), f.filepath)),11) as filepath 
			FROM {immo_files} i
			JOIN {files} f ON i.fid = f.fid
			GROUP BY i.vid";
			
			$result = db_query($sql);
			while ($file = db_fetch_object($result)) {
				$this->files[$file->vid] = $file;
			}
		}
	}

  /**
   * Render whatever the data is as a link to the file.
   *
   * Data should be made XSS safe prior to calling this function.
   */
	function render($values) {
		if(!isset($this->files[$values->{$this->field_alias}]))
			return false;
		
		$item = $this->files[$values->{$this->field_alias}];
		$format = $this->options['format'];
		
		//image cache
		if(preg_match("/^imagecache_/si", $format)) {
			$formatter_name = preg_replace("/^imagecache_/si", "", $format);
			$theme = 'imagecache_formatter_'. $formatter_name;
			$item->fid = 9999;
			$element = array(
				'#theme' 		=> $theme,
				'#formatter' 	=> $formatter_name,
				'#item'			=> (array)$item
			);
			return theme($theme, $element);
		}
		
		switch($format) {
			case "image_plain":
				return theme("image", $item->filepath);
			break;
			case "image_nodelink":
				return l(theme("image", $item->filepath), 'node/'.$item->nid, array('html' => true));
			break;
			case "image_imagelink":
				return l(theme("image", $item->filepath), $item->filepath, array('html' => true));
			break;
			case "path_plain":
				return $item->filepath;
			break;
			case "url_plain":
				return url($item->filepath, array("absolute" => true));
			break;
		}
		
		return check_plain($item->filepath);
	}
}

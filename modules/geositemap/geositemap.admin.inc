<?php
// $Id: immobilier.module,v 1.0 2009/09/08 11:30:00$

/*******************************************************************************
 * HOOKS DRUPAL
 ******************************************************************************/

/*******************************************************************************
 * PREPROCESS
 ******************************************************************************/
 
/*******************************************************************************
 * FORMULAIRES
 ******************************************************************************/
 
/**
* Formulaire de configuration du module
 */
function geositemap_admin_settings_form() {
	$form = null;
	
	$form["general"] = array(
		'#type' 		=> 'fieldset',
		'#title' 		=> 'Configuration générale',
		'#description' 	=> "Configuration générale du Geositemap",
	);
	
	$form["general"]['geositemap_title'] = array(
		'#type' 		=> 'textfield',
		'#title' 		=> 'Titre de page',
		'#description' 	=> "Titre principale de la page",
		'#default_value' => geositemap_variable_get("page_title")
	);
	
	$form["general"]['geositemap_head_title'] = array(
		'#type' 		=> 'textfield',
		'#title' 		=> 'Titre de head',
		'#description' 	=> "Contenu de la balise <em>&lt;title&gt;</em>",
		'#default_value' => geositemap_variable_get("head_title")
	);
	
		
	foreach(array("departement","ville") as $key) {
		
		$form["$key"] = array(
			'#type' 		=> 'fieldset',
			'#title' 		=> ucfirst($key),
			'#description' 	=> "Configuration de la partie $key",
		);
		
		$form["$key"]['geositemap_'.$key.'_title'] = array(
			'#type' 		=> 'textfield',
			'#title' 		=> 'Titre de page',
			'#description' 	=> "Titre principale de la page",
			'#default_value' => geositemap_variable_get($key."_page_title")
		);
		
		$form["$key"]['geositemap_'.$key.'_head_title'] = array(
			'#type' 		=> 'textfield',
			'#title' 		=> 'Titre de head',
			'#description' 	=> "Contenu de la balise <em>&lt;title&gt;</em> de la page",
			'#default_value' => geositemap_variable_get($key."_head_title")
		);
		
		$form["$key"]['geositemap_'.$key.'_meta_description'] = array(
			'#type' 		=> 'textarea',
			'#title' 		=> 'Description',
			'#description' 	=> "Balise méta description de la page",
			'#default_value' => geositemap_variable_get($key."_meta_description")
		);
	}
	return system_settings_form($form);
} 

/*******************************************************************************
 * PAGES / CALLBACK
 ******************************************************************************/

/*******************************************************************************
 * THEMES
 ******************************************************************************/

/*******************************************************************************
 * DIVERS
 ******************************************************************************/

<?php
// $Id: immobilier_subscription.module,v 1.0 2009/09/08 11:30:00$

/*******************************************************************************
 * HOOKS DRUPAL
 ******************************************************************************/

/*******************************************************************************
 * PREPROCESS
 ******************************************************************************/
 
/*******************************************************************************
 * FORMULAIRES
 ******************************************************************************/

/*
* formulaire d'inscription
*/
function immobilier_subscription_register_form($form_state, $type = false, $params = false) {
	global $user;
	
	$form = array(
		"#validate" =>  array('immobilier_subscription_register_form_validate', 'immobilier_search_form_validate'),
	);
	
	$form["mail"] = array(
		"#title" => "Adresse mail",
		"#type" => "textfield",
		"#default_value" => @$_GET["mail"],
		"#required" => true
	);
	
	// votre recherche
	$form["search"] = array(
		"#type" => "fieldset",
		"#title" => "Votre recherche",
		"#tree" => true
	);
		
	$values = $params ? immobilier_search_params_load($params) : null;
	
	// récupération du formulaire
	$form_id = "immobilier_search_form".($type ? "_$type" : "");
	$search_form = drupal_retrieve_form($form_id, $form_state, $values);
	drupal_prepare_form($form_id, $search_form, $form_state);

	foreach(element_children($search_form) as $key) {
		if(in_array($key, array("reference", "submit", "form_token", "form_id"))) continue;
		$form["search"][$key] = $search_form[$key];
	}
	
	$form["submit"] = array(
		"#value" => "Inscription",
		"#type" => "submit"
	);
	
	return $form;
}

/*
* validate
* @see immobilier_subscription_register_form
*/
function immobilier_subscription_register_form_validate($form, $form_state) {
	if (!valid_email_address($form_state["values"]['mail'])) {
		form_set_error('mail', 'Vous devez spécifier une adresse mail valide !');
	}
} 

/*
* submit
* @see immobilier_subscription_register_form
*/
function immobilier_subscription_register_form_submit($form, $form_state) {
	$values = $form_state["values"];
	
	// data
	$data = array();
	foreach($values as $key=>$value) {
		if(!in_array($key,array("mail", "type", "search", "form_build_id", "form_token", "form_id", "op", "submit"))) {
			$data[$key] = $value;
			unset($values[$key]);
		}
	}
	
	// search
	$search = $values["search"];
	$type = $search["type"];
	unset($search["type"]);
	
	// subscription
	$subscription = new stdClass();
	$subscription->mail = $values["mail"];
	$subscription->type = $type;
	$subscription->search = $search ? serialize($search) : false;
	$subscription->data = $data ? serialize($data) : false;
	$subscription->created = time();
	$subscription->changed = time();
	$subscription->active = 1;
		
	drupal_write_record("immo_subscription", $subscription);
	drupal_set_message(t("Votre inscription a bien été prise en compte."));
} 

/*
* formulaire de desinscription
*/
function immobilier_subscription_unregister_form($form_state, $type = false) {
	global $user;
	$form = array();
	
	$form["mail"] = array(
		"#title" => "Adresse mail",
		"#type" => "textfield",
		"#default_value" => @$_GET["mail"],
		"#required" => true
	);
		
	$form["submit"] = array(
		"#value" => "Désinscription",
		"#type" => "submit"
	);
 
	return $form;
}

/*
* submit
* @see immobilier_subscription_unregister_form
*/
function immobilier_subscription_unregister_form_submit($form, $form_state) {
	$mail = $form_state["values"]["mail"];
	$rs = db_query("UPDATE immo_subscription SET active = 0, changed = UNIX_TIMESTAMP() WHERE mail = '%s' AND active = 1", array($mail));
	drupal_set_message(t("Votre désinscription a bien été prise en compte."));
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

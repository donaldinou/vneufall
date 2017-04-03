<?php
// $Id: system.admin.inc,v 1.63.2.7 2009/02/25 11:38:41 goba Exp $

/*******************************************************************************
 * PAGES / CALLBACKS
 ******************************************************************************/
 
/*
* Liste des abonnements
*/
function immobilier_subscription_admin_list() {

	$headers = array(
		array('data'=>'Adresse mail', 	'field'=>'mail'), 
		array('data'=>'Type', 			'field'=>'type'),
		array('data'=>'Paramètres'),
		array('data'=>'Créé le', 		'field'=>'created'),
		array('data'=>'Envois', 		'field'=>'messages'),
		array('data'=>'Annonces', 		'field'=>'nodes'),
		array('data'=>'Statut', 		'field'=>'active'),
		array()
	);
	
	// filtres
	$where = array();
	
	// filtre : actif
	if(!isset($_REQUEST['actif'])) $_REQUEST['actif'] = "1";
	if(strlen($_REQUEST['actif'])) 
		$where[] = "active = ".($_REQUEST['actif'] ? "1" : "0"); 
		
	// filtre : type
	if(strlen($_REQUEST['type'])) 
		$where[] = "type = '".db_escape_string($_REQUEST['type'])."'";
		
	// filtre : keyword
	if($keyword = $_REQUEST['keyword']) {
		$_where = array();
		foreach(array('mail', 'search') as $col)
			$_where[] = "$col LIKE '%%".db_escape_string($keyword)."%%'";	
		$where[] = "(".join(" OR ", $_where).")";
	}

	$where = $where ? "WHERE (".join(" AND ", $where).")" : "";
	
	// récupération de tous les utilisateurs depuis la bdd kekoli		
	$sql = "SELECT subscription.*, COUNT(DISTINCT stats.timestamp) as messages, COUNT(stats.sid) as nodes FROM immo_subscription subscription LEFT JOIN immo_subscription_stats stats ON stats.sid = subscription.sid $where GROUP BY subscription.sid";
	$sql_count = preg_replace('/^SELECT(?:.(?!FROM))*(.*)GROUP BY(?:.(?!GROUP))*$/si', "SELECT COUNT(DISTINCT subscription.sid) $1", $sql);

	$sql .= tablesort_sql($headers); 
	$result = pager_query($sql, 50, 0, $sql_count);
	
	while ($subscription = db_fetch_object($result)){
		$row = array(
			$subscription->mail,
			ucfirst($subscription->type),
			//nl2br(immobilier_subscription_search_string($subscription->type, $subscription->search)),
			nl2br(immobilier_subscription_to_string($subscription)),
			format_date($subscription->created, "custom", "d/m/Y"),
			array('data' => $subscription->messages/*, 'style' => 'text-align:right'*/),
			array('data' => $subscription->nodes/*, 'style' => 'text-align:right'*/),
			$subscription->active ? '<strong style="color:green">Actif</strong>' : '<strong style="color:red">Inactif</strong>',
			l($subscription->active ? 'Désactiver' : 'Activer', "admin/immobilier/subscription/".$subscription->sid."/".($subscription->active ? "de" : "")."activate", array("query" => drupal_get_destination()))
		);
		$rows[] = $row;
	}
	
	$output = drupal_get_form('immobilier_subscription_admin_list_filter_form');
	$output .= theme('table', $headers, $rows);
	$output .= theme('pager');
	
	return $output;
}

/*
* immobilier_subscription_set_active
*/
function immobilier_subscription_set_active($subscription, $active = true) {
	db_query("UPDATE immo_subscription SET active = %d WHERE sid = %d", array($active ? 1 : 0, $subscription->sid));
	drupal_goto("admin/immobilier/subscription");
}

/*******************************************************************************
 * FORMULAIRES
 ******************************************************************************/
 
/*
* Formulaire de paramètre
*/
function immobilier_subscription_admin_settings_form() {
	$form = null;
	
	// tokens
    $headers = array(t('Token'), t('Replacement value'));
	$tokens = token_get_list("notification");
	foreach ($tokens as $type) {
		foreach($type as $token => $token_description) {
			$row = array();
			$row[] = '[' . $token . ']';
			$row[] = $token_description;
			$rows[] = $row;
		}
	}	

	$form["immobilier_subscription_notification_method"] = array(
		'#type' => 'select',
		'#title' => "Méthode d'envoi par défaut",
		'#options' => messaging_method_list(),
		'#default_value' => immobilier_subscription_variable_get('notification_method'),
		'#required' => true
	);
	
	$form["immobilier_subscription_notification_subject"] = array(
		'#type' => 'textfield',
		'#title' => 'Sujet',
		'#size' => 100,
		'#description' => 'Sujet du message envoyé. Voir ci-dessous les tokens disponibles',
		'#default_value' => immobilier_subscription_variable_get('notification_subject')
	);
	
	$form["tokens"] = array(
		'#title' => t('Available tokens'),
		'#type' => 'fieldset',
		'#description' => t('These special strings will be replaced by their real value at run time.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
		'list' => array( '#value' => theme('table', $headers, $rows, array('class' => 'description')))
	);
	
	$form = system_settings_form($form);
	$form["buttons"]["#weight"] = 99;
	return $form;
}


/**
 * Filtrer la liste des abonnements
 * @see kekoli_user_admin_user_list
 */
function immobilier_subscription_admin_list_filter_form(&$form_state) {
	$form = array(
		'#id' => 'produit-admin-filter-form',
		'#method' => 'GET',
		'#attributes' => array('class'=>'container-inline')
	);
	
	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Filtres'),
		//'#collapsible' => true,
		'#collapsed' => empty($_REQUEST['filter'])
	);
	
	$form['filters']['type'] = array(
		'#type' => 'select',
		'#title' => 'Type',
		'#options' => array(''	=> 'Tous')+immobilier_get_types("names"),
		'#default_value' => $_REQUEST['type'],
		'#attributes' => array('onchange'=>'this.form.submit()')
	);

	$form['filters']['actif'] = array(
		'#type' => 'select',
		'#title' => 'Statut',
		'#options' => array(
			''	=> 'Tous', 
			1 	=> 'Actifs', 
			0 	=> 'Inactifs'
		),
		'#default_value' => $_REQUEST['actif'],
		'#attributes' => array('onchange'=>'this.form.submit()')
	);
	
	$form['filters']['keyword'] = array(
		'#type' => 'textfield',
		'#title' => 'Mot clé',
		'#size' => 30,
		'#default_value' => $_REQUEST['keyword']
	);
	
	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Search')
	);


	$form['filters']['reset'] = array(
		'#type' => 'button',
		'#value' => t('Reset'),
		'#attributes' => array(
			'onclick' => 'document.location.href = "'.url('admin/immobilier/subscription').'"; return false;'
		)
	);
	
	// comptabilité avec le sort/order actuel de la liste
	$form['filters']['sort'] = array('#type' => 'hidden', '#value' => $_REQUEST["sort"] );
	$form['filters']['order'] = array('#type' => 'hidden', '#value' => $_REQUEST["order"] );
	
	return $form;	 
}

/**
 * Test un envoi de notifications
 */
function immobilier_subscription_admin_test_form() {
	global $user;
	
	$form['to'] = array(
		'#type' => 'textfield',
		'#title' => 'Destination',
		'#size' => 30,
		'#default_value' => isset($_GET["mail"]) ? $_GET["mail"] : $user->mail,
		'#required' => true,
		'#description' => "Entrez une adresse mail.",
	);
	
	$form['type'] = array(
		'#type' => 'select',
		'#title' => 'Type',
		'#required' => true,
		'#options' => immobilier_get_types("names"),
		'#default_value' => 'vente',
		'#description' => "Entrez une adresse mail.",
	);
	
	$form['count'] = array(
		'#type' => 'select',
		'#title' => "Nombre d'annonce",
		'#default_value' => 5,
		'#options' => array(1 => 1,	2 => 2,	3 => 3,	4 => 4,	5 => 5,	6 => 6,	7 => 7,	8 => 8,	9 => 9,	10 => 10),
		'#description' => "Nombre d'annonce aléatoires qui seront envoyées.",
	);
	
	$form['submit'] = array('#type' => 'submit', '#value' => 'Envoyer');

	//$form['#redirect'] = false;
	return $form;
}

/**
 * validate
 * @see immobilier_subscription_admin_test_form
 */
function immobilier_subscription_admin_test_form_validate($form, $form_state) {
	global $user;
	if(!valid_email_address($form_state['values']['to']))
		form_error($form['to'], "Vous devez fournir une adresse mail valide !");
}

/**
 * submit
 * @see immobilier_subscription_admin_test_form
 */
function immobilier_subscription_admin_test_form_submit($form, $form_state) {
	global $user;
	
	global $custom_theme;
    $custom_theme = false;
	
	$mail = $form_state['values']['to'];
	$type = $form_state['values']['type'];
	$count = $form_state['values']['count'];

	// query
	$query = immobilier_search_sql($type, null, $options);
	$query = preg_replace("/SELECT .* FROM/", "SELECT n.nid FROM", $query);
	$query = preg_replace("/ORDER BY .*$/", "ORDER BY RAND() LIMIT $count", $query); 
	$rs = db_query($query);
	
	$nodes = array();
	while($node = db_fetch_object($rs)) 
		$nodes[$node->nid] = node_load($node->nid);
	
	$subscription =  new stdClass;
	$subscription->mail = $mail;
	$subscription->sid = 0;
	$subscription->unsubscribe = url($type."/unsubscribe",array("absolute" => true, "query" => array("mail" => $mail)));
	
	if(immobilier_subscription_send($subscription, $nodes))
		drupal_set_message("Le message a correctement été envoyé !", "status");
	else	
		drupal_set_message("Il y a eu un problème lors de l'envoi du message !", "error");
		
	drupal_goto('admin/immobilier/subscription/test', "mail=$mail");
}
<?php
// $Id: immobilier.module,v 1.0 2009/09/08 11:30:00$

/*******************************************************************************
 * HOOKS DRUPAL
 ******************************************************************************/

/*******************************************************************************
 * PREPROCESS
 ******************************************************************************/
 
/*******************************************************************************
 * PAGES / CALLBACK
 ******************************************************************************/
 
/**
* Liste des annonces immobilieres
 */
function immobilier_admin_node_list($type = null) {

	$header = array(
		array('data' => 'Référence', 'field' => 'annonce.reference', 'sort' => 'asc'),
		array('data' => 'Titre', 'field' => 'annonce.title'),
		array('data' => 'Pièces', 'field' => 'annonce.pieces'),
		//array('data' => 'Chambres', 'field' => 'annonce.chambres'),
		array('data' => 'Surface', 'field' => 'annonce.surface'),
		array('data' => 'Prix', 'field' => 'annonce.prix'),
		array('data' => 'CP', 'field' => 'annonce.code_postal'),
		array('data' => 'Ville', 'field' => 'annonce.ville'),
		array('data' => 'Secteur', 'field' => 't_secteur.name'),
		null
	);

	// filtre eventuel
	$where = array();
	if($filter = $_REQUEST['filter']) {
		foreach(array('annonce.reference', 'annonce.title', 'annonce.prix', 'annonce.code_postal', 'annonce.ville', 't_secteur.name', 'annonce.body') as $col)
			$where[] = "$col LIKE '%".db_escape_string($filter)."%'";	
	}
	$where = $where ? "(".join(" OR ", $where).")" : "";
	
	// where  original
	if(!empty($type))
		$where = "annonce.type = '".db_escape_string($type)."'".($where ? "AND $where" : "");
	
	$sql = immobilier_annonce_sql($where);
	$query_count = "SELECT COUNT(annonce.nid) FROM {immo_annonce} annonce JOIN term_data t_secteur ON annonce.secteur_tid = t_secteur.tid".($where ? " WHERE $where" : ""); 
	$sql .= tablesort_sql($header);
	$result = pager_query($sql, 50, 0, $query_count);
	
	if(db_affected_rows($result) == 0) {
		$rows[] = array(array('data' => '<em>Aucune copropriété ne correspond à ces critères</em>', 'colspan' => '9'));
	}	
	
	while($annonce = db_fetch_object($result)) {
		$rows[] = array(
			l($annonce->reference, 'node/'.$annonce->nid),
			$annonce->title,
			$annonce->pieces,
			//$annonce->chambres,
			array('data' => $annonce->surface ? $annonce->surface." m²" : null),
			array('data' => $annonce->prix ? $annonce->prix." &euro;" : null),
			$annonce->code_postal,
			$annonce->ville,
			$annonce->secteur,
			l('éditer', 'node/'.$annonce->nid.'/edit')
		 );
	}
	
	$output = drupal_get_form('immobilier_admin_node_list_filter_form', $type);
	$output .= theme('table', $header, $rows);
	$output .= theme('pager');
	return $output;
}


/**
* Autocompletion des villes
 * @see hook_menu (admin/immobilier/ajax/ville)
 */
function immobilier_admin_ville_autocomplete() {
	$string = arg(3);
	$matches = array();
	if ($string) {
		$result = db_query("SELECT DISTINCT ville FROM immo_annonce WHERE LOWER(ville) LIKE LOWER('%%%s%%') ORDER BY 1 LIMIT 15", $string);
		while ($ville = db_fetch_object($result)) {
			$matches[$ville->ville] = check_plain($ville->ville);
		}
	}
	drupal_json($matches);
}

/*******************************************************************************
 * FORMULAIRES
 ******************************************************************************/

/**
 * Filtrer la liste des annonces
 * @see immobilier_admin_node_list
 */
function immobilier_admin_node_list_filter_form(&$form_state, $type) {
	$form = array(
		'#id' => 'produit-admin-filter-form',
		'#method' => 'GET',
		'#attributes' => array('class'=>'container-inline')
	);
	
	$form['filters'] = array(
		'#type' => 'fieldset',
		'#title' => t('Search'),
		//'#collapsible' => true,
		'#collapsed' => empty($_REQUEST['filter'])
	);

	$form['filters']['filter'] = array(
		'#type' => 'textfield',
		'#title' => 'Mot clé',
		'#default_value' => $_REQUEST['filter']
	);

	$form['filters']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Search')
	);


	$form['filters']['reset'] = array(
		'#type' => 'button',
		'#value' => t('Reset'),
		'#attributes' => array(
			'onclick' => 'document.location.href = "'.url('admin/immobilier/node/'.$type).'"; return false;'
		)
	);

	// comptabilité avec le sort/order actuel de la liste
	$form['filters']['sort'] = array('#type' => 'hidden', '#value' => $_REQUEST["sort"] );
	$form['filters']['order'] = array('#type' => 'hidden', '#value' => $_REQUEST["order"] );
	
	return $form;	 
}

/**
* Formulaire de configuration du module immobilier
 * @see hook_menu (admin/immobilier/ajax/ville)
 */
function immobilier_admin_settings_form() {
	$form = null;
	
	$vocabularies = taxonomy_get_vocabularies();
	$options = array(0 => '');
	foreach($vocabularies as $voc)
		$options[$voc->vid] = $voc->name;
	
	$form["taxonomy"] = array(
		"#type" => "fieldset",
		"#title" => "Taxonomie",
		// ---
		"immobilier_categories_vid" => array(
			'#type' => 'select',
			'#title' => 'Catégories',
			'#description' => 'Vocabulaire des catégories',
			'#options' => $options,
			'#default_value' => immobilier_variable_get('categories_vid', 0)
		),
		// ---
		"immobilier_secteurs_vid" => array(
			'#type' => 'select',
			'#title' => 'Secteurs',
			'#description' => 'Vocabulaire des secteurs',
			'#options' => $options,
			'#default_value' => immobilier_variable_get('secteurs_vid', 0)
		),
		// ---
	);
	
	$form["results"] = array(
		"#type" => "fieldset",
		"#title" => "Page de résultat",
		// ---
		"immobilier_search_pager_limit" => array(
			'#type' => 'textfield',
			'#title' => 'Nodes par page de résultat',
			'#description' => 'Nombre de node par page de résultat',
			'#default_value' => immobilier_variable_get('search_pager_limit', 10)
		),
		// ---
	);
	
	$form["geocoding"] = array(
		"#type" => "fieldset",
		"#title" => "Géolocalisation",
		// ---
		"immobilier_geocode_bounds" => array(
			'#type' => 'textfield',
			'#title' => 'Bounds',
			'#description' => 'Limite géographique des localisations (NO|SE) séparés par |',
			'#default_value' => immobilier_variable_get('geocode_bounds', '')
		),
		// ---
		"immobilier_geocode_departements" => array(
			'#type' => 'textfield',
			'#title' => 'Départements',
			'#description' => 'Liste des départements pour la géolocalisation séparés par des virgules',
			'#default_value' => immobilier_variable_get('geocode_departements', '')
		),
		// ---
	);
	
	
	// champs cck
	include_once(drupal_get_path('module', 'content'). '/includes/content.admin.inc');
	$existing_field_options = content_existing_field_options('');
	array_unshift($existing_field_options, '');
	
	
	$form["divers"] = array(
		"#type" => "fieldset",
		"#title" => "Divers",
		// ---
		"immobilier_images_cck_field" => array(
			'#type' => 'select',
			'#title' => 'Champ CCK des visuels',
			'#options' => $existing_field_options,
			'#default_value' => immobilier_variable_get('images_cck_field', 0)
		)
	);

	$form = system_settings_form($form);
	$form["buttons"]["#weight"] = 99;
	return $form;
}

/*******************************************************************************
 * THEMES
 ******************************************************************************/

/*******************************************************************************
 * DIVERS
 ******************************************************************************/

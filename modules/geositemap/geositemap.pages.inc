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
 
/*******************************************************************************
 * PAGES / CALLBACK
 ******************************************************************************/
 
/**
 * Page d'accueil
 */
function geositemap_front() {
	
	// Title
	drupal_set_title(geositemap_variable_get("title"));
	
	// Balise title
	geositemap_set_global("head_title", geositemap_variable_get("head_title"));
	
	// Meta Description
	geositemap_set_meta("description", geositemap_variable_get("meta_description"));
	
	// Breadcrumb
	drupal_set_breadcrumb(array());
	
	# top villes
	$villes = array();
	$sql = geositemap_sql_ville(false, false, false, 30, "population DESC", "niveauGeo = 'COM'");
	$rs = db_query($sql);
	while($ville = db_fetch_object($rs)) {
		geositemap_prepare_object(&$ville);
		$villes[] = $ville;
	}
	
	# departements
	$departements = array();
	$sql = geositemap_sql_departement(false, false, "nomDepartement ASC", "codeDepartement <> 75");
	$rs = db_query($sql);
	while($departement = db_fetch_object($rs)) {
		geositemap_prepare_object(&$departement);
		$departements[] = $departement;
	}
	
	return theme("geositemap", $villes, $departements);
}
 
/**
 * Page de département
 */
function geositemap_departement($departement, $lettre = false) {
	
	# pour Paris, il s'agit d'une ville
	if($departement->codeDepartement == 75) {
		return geositemap_ville(geositemap_ville_load($departement->chefLieuDepartement));
	}
	
	// Title
	drupal_set_title(geositemap_variable_get("departement_title"));
	
	// Balise title
	geositemap_set_global("head_title", geositemap_variable_get("departement_head_title"));
	
	// Meta Description
	geositemap_set_meta("description", geositemap_variable_get("departement_meta_description"));
	
	// Breadcrumb
	$breadcrumb = array();
	$breadcrumb[] = l(geositemap_variable_get("title"), 'geositemap');
	$breadcrumb[] = $departement->link_text;
	drupal_set_breadcrumb($breadcrumb);
	
	# top villes
	$top_villes = array();
	$sql = geositemap_sql_ville(false, $departement->codeDepartement, false, 20, "population DESC", "niveauGeo = 'COM'");
	$rs = db_query($sql);
	while($ville = db_fetch_object($rs)) {
		geositemap_prepare_object(&$ville);
		$top_villes[] = $ville;
	}
	
	#lettres
	$lettres = array();
	$rs = db_query("SELECT DISTINCT SUBSTRING( search_nom, 1, 1 ) as lettre FROM geositemap_commune WHERE codeDepartement = %d AND search_nom > ''", $departement->codeDepartement);
	while($l = db_fetch_object($rs)) 
		$lettres[] = $l->lettre;
	if(!$lettre) $lettre = @$lettres[0];
	
	
	#VILLES
	$villes = array();
	$where = sprintf("SUBSTRING(search_nom,1,1) = '%s' AND niveauGeo = 'COM'", db_escape_string($lettre));
	$sql = geositemap_sql_ville(false, $departement->codeDepartement, false, false, "nom ASC", $where);
	$rs = db_query($sql);
	while($ville = db_fetch_object($rs)) {
		geositemap_prepare_object(&$ville);
		$villes[] = $ville;
	}
	
	geositemap_set_global("token", $departement);
	return theme("geositemap_departement", $departement, $top_villes, $villes, $lettres);
}

/**
 * Page de ville
 */
function geositemap_ville($ville) {

	// Title
	drupal_set_title(geositemap_variable_get("ville_title"));
	
	// Balise title
	geositemap_set_global("head_title", geositemap_variable_get("ville_head_title"));
	
	// Meta Description
	geositemap_set_meta("description", geositemap_variable_get("ville_meta_description"));
	//geositemap_set_meta("ICBM", $ville->lat.",".$ville->lon);
	
	// Breadcrumb
	$breadcrumb = array();
	$breadcrumb[] = l(geositemap_variable_get("title"), 'geositemap');
	if($ville->codeDepartement <> 75) // si la ville n'est pas en région parisienne, on ajoute le département
		$breadcrumb[] = geositemap_l($ville, "departement");
	if($ville->niveauGeo == 'ARM') // si c'est un arrondissement, on ajoute la ville référence
		$breadcrumb[] = geositemap_l(geositemap_ville_load($ville->bassinVie));
	$breadcrumb[] = $ville->link_text;
	drupal_set_breadcrumb($breadcrumb);
	
	// arrondissements
	$arrondissements = array();
	if(in_array($ville->codeInsee, array(75056,69123,13055))) {
		$sql = geositemap_sql_ville(false,$ville->codeDepartement, false, false, $order = "codeInsee ASC", sprintf("niveauGeo = 'ARM' AND bassinVie = %d", $ville->codeInsee));
		$rs = db_query($sql);
		while($arrondissement = db_fetch_object($rs)) {
			geositemap_prepare_object(&$arrondissement);
			$arrondissements[] = $arrondissement;
		}
	}
	
	geositemap_set_global("token", $ville);
	return theme("geositemap_ville", $ville, $arrondissements);
}

/*******************************************************************************
 * THEMES
 ******************************************************************************/

/*******************************************************************************
 * DIVERS
 ******************************************************************************/

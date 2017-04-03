<?php
/****
 * @file acreat.zend.inc
* fonctions d'aide du Zend Framework
* fonctionne avec le module Zend
*/

/*
* zend_db
* Initilalise un objet Zend_Db configuré avec les paramètres de connexion du site Drupal
*/
function zend_db($config='default', $abstract_class=false) {
	static $zend_db;
	global $db_url;
	
	if($zend_db) return $zend_db;
	
	zend_initialize('Zend_Db');
	
	if(is_array($db_url))
		$db_url = $db_url[$config];
	
	$db_infos = parse_url($db_url);
	
	/*if(!$abstract_class) {
		switch($db_infos["scheme"]) {
			case "mysql": $abstract_class = "php_mysql"; break;
			case "mysqli": $abstract_class = "mysqli"; break;
		}
	}*/
		
	if(!$abstract_class)
		$abstract_class = $db_infos["scheme"];
	
	$zend_db = Zend_Db::factory($abstract_class, array(
		"dbname"	=> $db_infos["dbname"],
		"password"	=> $db_infos["pass"],
		"host"		=> $db_infos["host"],
		"username"	=> $db_infos["user"]
	));
	
	return $zend_db;
}

/*
* zend_db_select_searchWhere
* Ajoute une requete WHERE pour effectuer une recherche de texte sur un ensemble de colonnes
*/
function zend_db_select_searchWhere(&$sql, $string, $cols) {
	$where = array();
	foreach($cols as $col) 
		$where[] = $sql->getAdapter()->quoteInto("$col LIKE ?", '%'.$string.'%');
	return $sql->where(join(' OR ', $where));
}


		
		
		
		
		
		
		
		
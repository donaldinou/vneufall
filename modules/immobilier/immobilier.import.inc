<?php
// $Id: immobilier.module,v 1.0 2009/09/08 11:30:00$
// fonctions diverses pour l'import automatique des annonces dans Drupal


/**
 * charge un node à partir de la référence et de l'agence
 * vérification par la référence / agence
 * @param $node : objet $node ou strings $reference + $agence
 */
function immobilier_node_load($reference, $aid = NULL) {
	if(is_object($reference)) {
		if($reference->nid) 
			return node_load($reference->nid);
		$reference 	= $reference->reference;
		$aid 		= $reference->aid;
	}
	if(empty($reference))
		return false;
	$sql = "SELECT a.nid FROM  {immo_annonce} a WHERE a.reference = '%s'" . ($aid !== NULL ? " AND a.aid = ".((int)$aid) : "");
	$nid = db_result(db_query($sql, $reference));
	if(!$nid) return false;
	return node_load($nid);
}

/**
 * sauvegarde d'un node immobilier
 * vérification par la référence / agence
 */
function immobilier_node_save(&$node, $options = array()) {

	if(empty($node->reference))
		return false;
		
	// chargement initial
	if(!$node->nid) {
		$_node = immobilier_node_load($node);
		if($_node) {
			$node->nid = $_node->nid;
			$node->vid = $_node->vid;
			$node->created = $_node->created;
		}
	}
	
	// sauvegarde de la categorie
	if(isset($node->categorie)) {
		$vid 	= immobilier_variable_get('categories_vid');
		$term 	= immobilier_get_term($node->categorie, $vid);
		$node->taxonomy[$vid] = $term ? $term->tid : 0;
		unset($node->categorie);
	}
		
	// sauvegarde du secteur
	if(isset($node->secteur)) {
		$vid 	= immobilier_variable_get('secteurs_vid');
		$term 	= immobilier_get_term($node->secteur, $vid); // , true
		$node->taxonomy[$vid] = $term ? $term->tid : 0;
		unset($node->secteur);
	}
	
	// appel du hook immobilier_node_save_alter 
	module_invoke_all("immobilier_node_save_alter", $node);
	
	// teaser
	if (!isset($node->teaser) && isset($node->body)) 
		$node->teaser = node_teaser($node->body, NULL);
	$insert = empty($node->nid);
	node_save($node);
	return true;
}


/**
 * Implémentation du hook immobilier_node_save_alter
 * Modifications éventuelles du node en vue de son enregistrement
 * @see immobilier_node_save_alter
 */
function immobilier_immobilier_node_save_alter($node) {
	// MODULES TIERS
	if(module_exists("print")) {
		$node->print_display = variable_get('print_display_'. $node->type, PRINT_TYPE_SHOW_LINK_DEFAULT);
		$node->print_display_comment = variable_get('print_display_comment_'. $form['#node_type']->type, PRINT_TYPE_COMMENT_LINK_DEFAULT);
		$node->print_display_urllist = variable_get('print_display_urllist_'. $form['#node_type']->type, PRINT_TYPE_URLLIST_DEFAULT);
	}
}

/**
 * suppression d'un node
 */
function immobilier_node_delete($node) {
	if(!is_object($node)) {
		$node = node_load($node);
		if(!$node) return false;
	}

	db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
	db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);

	// Call the node-specific callback (if any):
	node_invoke($node, 'delete');
	node_invoke_nodeapi($node, 'delete');

	// Clear the cache so an anonymous poster can see the node being deleted.
	cache_clear_all();

	// Remove this node from the search index if needed.
	if (function_exists('search_wipe')) {
		search_wipe($node->nid, 'node');
	}
}
	
/**
 * sauvegarde d'un fichier
 * pour une annonce
 */
function immobilier_file_save($filepath, $nid, $type = false, $index = 0, $replace = FILE_EXISTS_REPLACE) {
	if(isset($nid->nid)) 
		$nid = $nid->nid;
		
	$file = new stdClass();
	$file->filename = basename($filepath);
	$file->filepath = $filepath;
	$file->filemime = file_get_mimetype($filepath);
	$file->filesize = filesize($filepath);
	
	if(!$type)
		$type =  substr($file->filemime, 0, strpos($file->filemime, "/"));
	
	$dir = file_create_path()."/immobilier/".$type; 
	 // Build the destination folder tree if it doesn't already exists.
	if (!file_check_directory($dir, FILE_CREATE_DIRECTORY) && !mkdir($dir, 0775, true)) {
		watchdog('immobilier', 'Failed to create immobilier directory: %dir', array('%dir' => $dir), WATCHDOG_ERROR);
		return false;
	}
	
	if(file_copy($file, $dir, $replace)) {
		$file->uid = 0;
		$file->status = FILE_STATUS_PERMANENT;
		$file->timestamp = time();
		drupal_write_record('files', $file);
		
		$node = node_load($nid);
		$immo_file = new stdClass();
		$immo_file->fid = $file->fid;
		$immo_file->vid = $node->vid;
		$immo_file->nid = $node->nid;
		$immo_file->type = $type;
		$immo_file->weight = $index;
		drupal_write_record('immo_files', $immo_file);
		
		return 1;
	}
	
	return 0;
}

/**
 * sauvegarde l'ensemble des fichiers
 * pour une annonce
 */
function immobilier_files_save($filepathes, $nid, $type = "image", $replace = false) {
	//$filepathes = array();
	$node = node_load($nid);
	
	// construction du tableau comparateur
	$files = (array)$node->medias->$type;
	$filenames = array();
	foreach($files as $key=>$value) 
		$filenames[$key] = $value->filename;

	// pour chaque fichier passé en paramètre
	foreach($filepathes as $index => $from) {
		$save = true;
		
		// recherche du fichier en mémoire
		$filename = basename($from);
		$key = array_search($filename, $filenames);
		if($key !== FALSE) {
			$file = $files[$key];
			$save = false;
			// si la taille differt ou que la photo n'existe pas (bug), on supprime pour resauvegarder
			if($file->filesize <> filesize($from) || !file_exists($file->filepath)) {
				immobilier_file_delete($file);
				$save = true;
			// si l'index change on update uniquement l'index
			} elseif($file->weight <> $index) {
				db_query("UPDATE immo_files SET weight = %d WHERE fid = %d", $index, $file->fid);
			}
			unset($files[$key]);			
		}

		// sauvegarde du fichier
		if($save)
			immobilier_file_save($from, $node, $type, $index);
	}

	if($replace) {
		// les fichiers restants sont supprimés
		foreach($files as $file) {
			immobilier_file_delete($file);	
		}
	}
	
	return true;
}

/**
 * remplace l'ensemble des fichiers
 * pour une annonce
 */
function immobilier_files_replace($filepathes, $nid, $type = "image") {
	return immobilier_files_save($filepathes, $nid, $type, true);
}

/**
 * retrouve le term correspondant à un string ou un int
 * si $insert est à true on peut insérer le term s'in n'existe pas
 */
function immobilier_get_term($search, $vid, $insert = false) {
	static $terms;
	
	if(empty($search))
		return false;
	
	if(!is_numeric($vid)) {
		$vid = immobilier_variable_get($vid.'_vid');
		if(!$vid) return false;
	}
	
	$vocabulary = taxonomy_vocabulary_load($vid);
	if(!$vocabulary) return false;
	
	if(!isset($terms[$vocabulary->vid][$search])) {
		$term = false;
		
		if(is_numeric($search)) 
			$term = db_fetch_object(db_query(db_rewrite_sql('SELECT * FROM {term_data} t WHERE t.tid = %d AND t.vid = %d', 't', 'tid'), (int)$search, $vocabulary->vid));	
		
		if(is_string($search)) {
			$term = db_fetch_object(db_query("
				SELECT 
					t.* 
				FROM term_data t 
				LEFT JOIN term_synonym s ON s.tid = t.tid
				WHERE t.vid = %d
				AND (
					LOWER(t.name) = LOWER('%s') 
					OR LOWER(s.name) = LOWER('%s')
				)
				GROUP BY t.tid", $vocabulary->vid, trim($search), trim($search), trim($search)
			));
			
			if(!$term) 
				watchdog("immobilier","Impossible d'associer <em>$search</em> à un terme du vocabulaire <em>".$vocabulary->name."</em>", null, WATCHDOG_ALERT);
			
			if(!$term && $insert) {
				// si le terme recherché est introuvable
				$term = array("vid" => $vocabulary->vid,"name" => $search);
				taxonomy_save_term($term);
				$term = (object)$term;
			}
		}
		$terms[$vid][$search] = $term;
	}
	return $terms[$vid][$search];
}


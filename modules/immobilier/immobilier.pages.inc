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
 * Résultat de recherche
 */
function immobilier_search_page($type, $params) {
	global $pager_page_array, $pager_total, $pager_total_items;

	// objet recherche 
	$search = new stdClass();
	$search->type 	= $type;
	$search->params = $params;
	$search->path 	= $type.'/search/'.immobilier_search_serialize($params);
	$search->title 	= immobilier_search_title($type, $params);
	$search->limit 	= immobilier_variable_get($type."_search_pager_limit", immobilier_variable_get("search_pager_limit"));

	// requete 
	$query = immobilier_search_sql($type,$params);
	if($search->limit > 0) {
		$count_query = preg_replace(array('/SELECT.*?FROM /As', '/ORDER BY .*/', '/GROUP BY .*/'), array('SELECT COUNT(DISTINCT annonce.vid) FROM ', '', ''), $query);
		$result = pager_query($query,$search->limit,0, $count_query);
		$search->total 	= $pager_total_items[0];
		$search->page 	= $pager_page_array[0];
	} else {
		$result = db_query($query);
		$search->total 	= db_affected_rows($result);
		$search->page 	= 0;
	}
	
	// nodes 
	$nodes = array();
	$key = 0;
	while ($node = db_fetch_object($result)) {
		$node->search = clone $search;
		$node->search->index = $search->page * $search->limit + ($key++);
		$nodes[] = $node;
	}
	
	// breadcrumb
	$search_title_no_type = immobilier_search_title(false, $params);
	$breadcrumb = array(
		l(t('Home'), NULL), 
		l(immobilier_get_types("name", $type), $type), 
		$search_title_no_type ? ucfirst($search_title_no_type) : drupal_get_title()
	);
	drupal_set_breadcrumb($breadcrumb);
	
	// title, feed
	drupal_set_title($search->title);
	drupal_add_feed(url($search->path.'/rss'), $search->title);

	return theme("immobilier_search", $nodes, $search);
}
 
/**
 * Détail d'un résultat de recherche
 */
function immobilier_search_detail_page($type, $params) {
	global $pager_page_array, $pager_total, $pager_total_items;

	// requete 
	$query = immobilier_search_sql($type,$params);
	$count_query = preg_replace(array('/SELECT.*?FROM /As', '/ORDER BY .*/', '/GROUP BY .*/'), array('SELECT COUNT(DISTINCT annonce.vid) FROM ', '', ''), $query);
	$result = pager_query($query,1,0, $count_query);
	
	// objet recherche
	$search = new stdClass();
	$search->type 	= $type;
	$search->params = $params;
	$search->path 	= $type.'/search/'.immobilier_search_serialize($params);
	$search->title 	= immobilier_search_title($type, $params);
	$search->limit 	= immobilier_variable_get($type."_search_pager_limit", immobilier_variable_get("search_pager_limit"));
	$search->index 	= $pager_page_array[0];
	$search->page 	= $search->limit > 0 ? floor($search->index/$search->limit) : 0;
	
	// node
	$node = db_fetch_object($result);
	if(!$node) { return drupal_not_found(); }
	$node = node_load($node->nid);
	$node->search = $search;
	//$GLOBALS["node"] = $node;

	// breadcrumb
	$search_title_no_type = immobilier_search_title(false, $params);
	$breadcrumb = array(
		l(t('Home'), NULL), 
		l(immobilier_get_types("name", $type), $type),
		l($search_title_no_type ? ucfirst($search_title_no_type) : 'Résultats de votre recherche', $search->path, array('query' => array('page' => $search->page))),
		$node->title
	);
	drupal_set_breadcrumb($breadcrumb);
	drupal_set_title($node->title);
	
	
	//$_GET["q"] = "node/".$node->nid;
	return node_show($node, false);
}
 
/**
 * Résultats GeoXML
 */
function immobilier_search_gxml($type, $params, $keys = false) {
	$query = immobilier_search_sql($type,$params, array("where" => array("annonce.longitude <> 0 AND annonce.latitude <> 0")));
	$result = db_query($query);
	
	if(!$keys)
		$keys = array('nid','title','type','categorie','url','latitude','longitude');
	
	$resultset = '';
	$index = 0;
	while($node = db_fetch_object($result)) {
		$node->url =  url($type.'/search/'.immobilier_search_serialize($params).'/detail/'.(++$index), array('absolute' => true)); 
		$resultset .= '<result>';
		foreach($keys as $key) 
			$resultset .= "<$key>".utf8_encode($node->$key)."</$key>";
		$resultset .= '</result>';
	}
	
	$output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	$output .= "<resultset>\n";
	$output .= $resultset;
	$output .= "</resultset>\n";

	drupal_set_header('Content-Type: text/xml; charset=utf-8');
	print $output;	
}


/**
 * Fil RSS
 */

function immobilier_search_feed($type, $params, $channel = array()) {
	global $base_url;
	
	$query = immobilier_search_sql($type,$params, false, "annonce.created DESC");
	$result = db_query($query);
	
	$items = '';
	while($node = db_fetch_object($result)) {
		$node->link = url("node/".$node->nid, array('absolute' => TRUE));
		$extra = array("pubDate" => gmdate('r', $node->created));
		if($node->filepath)
			$extra[] = array(
				'key' => 'enclosure', 
				'attributes' => array(
					'url' => url($node->filepath, array('absolute' => TRUE)),
					'type' => $node->filemime
				)
			);
		$items .= format_rss_item($node->title, $node->link, $node->body, $extra);
	}
	
	$channel_defaults = array(
		'version'     => '2.0',
		'title'       => variable_get('site_name', 'Drupal'),
		'link'        => $base_url,
		'description' => variable_get('site_mission', ''),
		'language'    => $language->language
	);
	$channel = array_merge($channel_defaults, $channel);

	$namespaces = array();
	$output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	$output .= "<rss version=\"". $channel["version"] ."\" xml:base=\"". $base_url ."\" ". drupal_attributes($namespaces) .">\n";
	$output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
	$output .= "</rss>\n";

	drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
	print $output;
}

/**
 * Fil RSS
 */
function immobilier_export_viteloge() {
	global $base_url;

	
	$query = immobilier_annonce_sql("n.status = 1 AND n.type IN ('vente', 'location')");
	$result = db_query($query);
	
	$output = '<?xml version="1.0" encoding="utf-8"?>'."\n";
	$output .= '<xml date="'.date("Y-m-d H:i:s").'">'."\n";
	
	// agence
	/*$agence = array(
		"nom"		=> "Acreat",
		"adresse" 	=> "3 rue Jules Simon",
		"codepostal"=> "35000",
		"ville"		=> "Rennes",
		"tel"		=> "0820208282",
		"fax"		=> "",
		"mail"		=> "contact@acreat.com",
		"url"		=> "http://www.acreat.com"
	);
	$output .= "<agence>".format_xml_elements($agence)."</agence>\n";*/
	
	// biens
	$output .= "<biens>\n";
	while($node = db_fetch_object($result)) {
		$node = node_load($node->nid);

		$bien = array(
			"reference" 	=> $node->reference,
			"transaction" 	=> $node->type,
			"type" 			=> $node->categorie,
			"pieces" 		=> $node->pieces,
			"surface" 		=> $node->surface,
			"chambres" 		=> $node->chambres,
			"sallesdeb" 	=> $node->sallesdb,
			"terrain" 		=> $node->terrain,
			"annee" 		=> $node->annee,
			"prix" 			=> $node->prix,
			"description" 	=> filter_xss($node->body, array()),
			"localisation" 	=> array(
				"adresse" 		=> $node->adresse,
				"codepostal" 	=> $node->code_postal,
				"quartier" 		=> $node->quartier,
				"ville" 		=> $node->ville,
				"coords" 		=> $node->latitude.",".$node->longitude
			),
			"medias" 		=> array(),
			"url" 			=> url("node/".$node->nid, array('absolute' => TRUE))
		);
		
		if(isset($node->medias->image)) {
			foreach($node->medias->image as $image)
				$bien["medias"][] = array('key' => 'photo', 'value' => url($image->filepath, array('absolute' => TRUE)));
		}
		
		$output .= "<bien>".format_xml_elements($bien)."</bien>\n";
	}
	$output .= "</biens>\n";
	$output .= "</xml>\n";
	
	drupal_set_header('Content-Type: text/xml; charset=utf-8');
	print $output;
}


/*******************************************************************************
 * THEMES
 ******************************************************************************/

/*******************************************************************************
 * DIVERS
 ******************************************************************************/

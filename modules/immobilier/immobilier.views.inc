<?php
// $Id: 

/*
*  This file is used to tell the views module about the new node_example table.
*
* Database definition:
* @code
*   CREATE TABLE node_example (
*     vid int(10) unsigned NOT NULL default '0',
*     nid int(10) unsigned NOT NULL default '0',
*     color varchar(255) NOT NULL default '',
*     quantity int(10) unsigned NOT NULL default '0',
*     PRIMARY KEY (vid, nid),
*     KEY <code>node_example_nid</code> (nid)
*   )
* @endcode
*/

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function immobilier_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'immobilier') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_immobilier_fid' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'views_handler_filter_immobilier_fid' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'views_handler_field_immobilier_picture' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_filter_immobilier_picture' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}
	
function immobilier_views_data()  {
	// Basic table information.

	// ----------------------------------------------------------------
	// node_example table
	//  New group within Views called 'Example'

	$data = array();
	$data['immo_annonce']['table']['group'] = 'Immobilier';
	$data['immo_annonce']['table']['join'] = array(
		'node' => array(
			'left_field' => 'nid',
			'field' => 'nid',
		),
		'node_revisions' => array(
		  'left_field' => 'vid',
		  'field' => 'vid',
		),
	);
	
	
/*
	// The 'group' index will be used as a prefix in the UI for any of this
	// table's fields, sort criteria, etc. so it's easy to tell where they came
	// from.
	$data['immo_annonce']['table']['group'] = 'Immobilier';
	$data['immo_files']['table']['group'] 	= 'Immobilier fichier';

	// tables + fields that can be used for SQL Joins
	$data['immo_annonce']['table']['join'] = array(
		// Index this array by the table name to which this table refers.
		// 'left_field' is the primary key in the referenced table.
		// 'field' is the foreign key in this table.
		'node_revisions' => array(
		  'left_field' => 'vid',
		  'field' => 'vid',
		),
		'node' => array(
			'left_field' => 'nid',
			'field' => 'nid',
		),
		'immo_files' => array(
			'left_field' => 'nid',
			'field' => 'nid',
		)
	);
	
	// tables + fields that can be used for SQL Joins
	$data['immo_files']['table']['join'] = array(
		'node' => array(
			'left_table' => 'immo_annonce',
			'left_field' => 'nid',
			'field' => 'nid',
		)
	);
	
	*/
	
	
	
	// Next, describe each of the individual fields in this table to Views. For
	// each field, you may define what field, sort, argument, and/or filter
	// handlers it supports. This will determine where in the Views interface you
	// may use the field.

	// Example plain text field.
	$data['immo_annonce']['reference'] = array(
		'title' => 'Référence',
		'help' => 'Référence',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['type'] = array(
		'title' => 'Type',
		'help' => 'Type',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['pieces'] = array(
		'title' => 'Nombre de pièces',
		'help' => 'Nombre de pièces',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['chambres'] = array(
		'title' => 'Nombre de chambres',
		'help' => 'Nombre de chambres',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);

	// Example numeric text field.
	$data['immo_annonce']['sallesdb'] = array(
		'title' => 'Nombre de salles de bain',
		'help' => 'Nombre de salles de bain',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['surface'] = array(
		'title' => 'Surface',
		'help' => 'Surface',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['surface_sejour'] = array(
		'title' => 'Surface du séjour',
		'help' => 'Surface du séjour',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['chauffage'] = array(
		'title' => 'Chauffage',
		'help' => 'Chauffage',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['exposition'] = array(
		'title' => 'Exposition',
		'help' => 'Exposition',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['annee'] = array(
		'title' => 'Année de construction',
		'help' => 'Année de construction',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['niveaux'] = array(
		'title' => 'Nombre de niveaux',
		'help' => 'Nombre de niveaux',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['etage'] = array(
		'title' => 'Etage',
		'help' => 'Etage',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['terrain'] = array(
		'title' => 'Surface du terrain',
		'help' => 'Surface du terrain',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['prix'] = array(
		'title' => 'Prix',
		'help' => 'Prix',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['charges'] = array(
		'title' => 'Charges',
		'help' => 'Charges',
		'field' => array(
		  'handler' => 'views_handler_field_numeric',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_numeric',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
  // Example boolean field.
	$data['immo_annonce']['exclusif'] = array(
		'title' => 'Exclusivité',
		'help' => 'Exclusivité',
		'field' => array(
			'handler' => 'views_handler_field_boolean',
			'click sortable' => TRUE,
		),
		'filter' => array(
			'handler' => 'views_handler_filter_boolean_operator',
			'label' => t('Published'),
			'type' => 'yes-no',
		),
		'sort' => array(
			'handler' => 'views_handler_sort',
		),
	);
	
	// Example numeric text field.
	$data['immo_annonce']['disponibilite'] = array(
		'title' => 'Date de disponibilité',
		'help' => 'Date de disponibilité',
		'field' => array(
		  'handler' => 'views_handler_field_date',
		  'click sortable' => TRUE,
		 ),
		'filter' => array(
		  'handler' => 'views_handler_filter_date',
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['adresse'] = array(
		'title' => 'Adresse',
		'help' => 'Adresse',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['code_postal'] = array(
		'title' => 'Code Postal',
		'help' => 'Code Postal',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['ville'] = array(
		'title' => 'Ville',
		'help' => 'Ville',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	
	// Example plain text field.
	$data['immo_annonce']['latitude'] = array(
		'title' => 'Latitude',
		'help' => 'Latitude',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	// Example plain text field.
	$data['immo_annonce']['longitude'] = array(
		'title' => 'Longitude',
		'help' => 'Longitude',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);

	
	// Example plain text field.
	$data['immo_annonce']['source'] = array(
		'title' => 'Source',
		'help' => 'Source',
		'field' => array(
		  'handler' => 'views_handler_field',
		  'click sortable' => TRUE,
		),
		'sort' => array(
		  'handler' => 'views_handler_sort',
		),
		'filter' => array(
		  'handler' => 'views_handler_filter_string',
		),
		'argument' => array(
		  'handler' => 'views_handler_argument_string',
		),
	);
	
	return $data;
}



/**
 * Implementation of hook_views_data_alter()
 */
function immobilier_views_data_alter(&$data) {
	$data['immo_annonce']['fid'] = array(
		'title' => t('Attached files'),
		'help' => 'Tous les fichiers attachés à l\'annonce',
		'real field' => 'vid',
		'field' => array(
			'handler' => 'views_handler_field_immobilier_fid',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_immobilier_fid',
			'title' => t('Has attached files'),
			'type' => 'yes-no',
			'help' => t('Only display items with attached files. This can cause duplicates if there are multiple attached files.'),
		),
		'relationship' => array(
			'title' => t('Attached files'),
			'help' => t('Add a relationship to gain access to more file data for files uploaded by immobilier.module. Note that this relationship will cause duplicate nodes if there are multiple files attached to the node.'),
			'relationship table' => 'immo_files',
			'relationship field' => 'fid',
			'base' => 'files',
			'base field' => 'fid',
			'handler' => 'views_handler_relationship',
			'label' => t('Files'),
		),
	);
	
	$data['immo_annonce']['picture'] = array(
		'title' => 'Photo',
		'help' => "Photo principale de l'annonce",
		'real field' => 'vid',
		'field' => array(
			'handler' => 'views_handler_field_immobilier_picture',
		),
		'filter' => array(
			'handler' => 'views_handler_filter_immobilier_picture',
			'title' => 'Au moins une photo',
			'type' => 'yes-no',
			'help' => 'Affiche uniquement les annonces possèdant au moins une photo',
		),
	);
	
	$data['immo_annonce']['secteur'] = array(
		'title' => 'Secteur',
		'help' => "Secteur de l'annonce",
		'real field' => 'secteur_tid',
		'field' => array(
			'handler' => 'views_handler_field',
		),
		'relationship' => array(
			'title' => t('Secteur'),
			'help' => 'Ajouter une relationship pour accèder au secteur',
			'relationship field' => 'secteur_tid',
			'base' => 'term_data',
			'base field' => 'tid',
			'handler' => 'views_handler_relationship',
			'label' => t('Secteur'),
		),
	);
	
	$data['immo_annonce']['categorie'] = array(
		'title' => 'Catégorie',
		'help' => "Catégorie de l'annonce",
		'real field' => 'categorie_tid',
		'field' => array(
			'handler' => 'views_handler_field',
		),
		'relationship' => array(
			'title' => t('Catégorie'),
			'help' => 'Ajouter une relationship pour accèder à la catégorie',
			'relationship field' => 'categorie_tid',
			'base' => 'term_data',
			'base field' => 'tid',
			'handler' => 'views_handler_relationship',
			'label' => t('Catégorie'),
		),
	);
}



?>
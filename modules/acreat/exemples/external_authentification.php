<?

/**
* Implementation of hook_form_alter()
*/
function kekoli_m_form_alter(&$form, $form_state, $form_id) {
	
	// formulaire de login
	if(in_array($form_id, array("user_login", "user_login_block"))) {
		if(isset($form_state['post']['name'])) {
			$array_key = array_search('user_login_authenticate_validate', $form['#validate']);
			array_splice($form['#validate'], $array_key+1, 0, 'kekoli_m_login_validate');
		}
	}
}

/*******************************************************************************
 * PAGES / CALLBACK
 ******************************************************************************/

/**
* kekoli_m_login_validate
* Validation du login 
* @see kekoli_m_form_alter
*/
function kekoli_m_login_validate($form, &$form_state) {
	global $user;
	if(!form_get_errors()) {
		kekoli_m_authenticate($form_state['values']);
	}
} 
 
/*******************************************************************************
 * DIVERS
 ******************************************************************************/
 
/* ---
* kekoli_m_authenticate
* authentification d'un utilisateur Kekoli
*/
function kekoli_m_authenticate($form_values) {
	$username = $form_values['name'];
	$pass = $form_values['pass'];
	
	$rs = kekoli_db_query("SELECT * FROM utilisateur WHERE emailUtilisateur = '%s' AND passwordUtilisateur = MD5('%s')", array($username,$pass));
	$row = db_fetch_array($rs);
	if($row) {
		// Enregistre l'utilisateur s'il n'existe pas encore
		user_external_login_register($username,'kekoli');
		// Ecrit la session, update le timestamp, lancer le hook 'login'
		return user_authenticate_finalize($form_values);
	}
	
	return false;
}

/* ---
* kekoli_db_query
* execute un db_query sur la bdd de Kekoli
*/
function kekoli_db_query() {
	db_set_active('kekoli');	// on passe à la bdd de sygefor
	$args =  func_get_args();
	$r = call_user_func_array ( "db_query", $args );
	db_set_active('default');	// on repasse à la bdd de drupal
	return $r;
}
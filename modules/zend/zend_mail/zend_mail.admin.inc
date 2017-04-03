<?php
// $Id: zend_mail.admin.inc,v 1.1.2.4 2008/06/01 17:15:56 mustafau Exp $

/**
 * @file
 * Administration page for Zend_Mail.
 */

/**
 * Administration settings for Zend Mail.
 */
function zend_mail_admin() {
  $form['zend_mail_transport'] = array(
    '#type' => 'select',
    '#title' => t('Transport'),
    '#description' => t('Which transport to use for sending emails.'),
    '#default_value' => variable_get('zend_mail_transport', 'none'),
  	'#options' => array(
      'none' => t('None'),
      'sendmail' => t('Send Mail'),
      'smtp' => t('SMTP'),
    ),
  );
  $form['zend_mail_smtp_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('SMTP'),
    '#description' => t('Configuration options for the SMTP transport.'),
    '#collapsed' => variable_get('zend_mail_transport', 'none') != 'smtp',
    '#collapsible' => TRUE,
  );
  $form['zend_mail_smtp_fieldset']['zend_mail_smtp_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t('The host SMTP server to connect to.'),
  	'#default_value' => variable_get('zend_mail_smtp_host', 'localhost'),
  );
  $form['zend_mail_smtp_fieldset']['zend_mail_smtp_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('The port to connect to the SMTP server with.'),
  	'#default_value' => variable_get('zend_mail_smtp_port', 25),
  );
  $form['zend_mail_smtp_fieldset']['zend_mail_smtp_encryption'] = array(
    '#type' => 'select',
    '#title' => t('Encryption'),
    '#description' => t('Secure the SMTP transport by using either TLS or SSL.'),
  	'#default_value' => variable_get('zend_mail_smtp_encryption', 'none'),
    '#options' => array(
      'none' => t('None'),
      'tls' => t('TLS'),
      'ssl' => t('SSL'),
    ),
  );
  $form['#submit'][] = 'zend_mail_admin_submit';
  return system_settings_form($form);
}

/**
 * Process zend_mail_admin form submissions.
 */
function zend_mail_admin_submit($form, $form_values) {
  if ($form_values['values']['zend_mail_transport'] != 'none') {
    variable_set('smtp_library', drupal_get_path('module', 'zend_mail') . '/zend_mail.inc');
  }
  else {
    variable_set('smtp_library', '');
  }
}

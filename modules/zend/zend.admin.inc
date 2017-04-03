<?php
// $Id: zend.admin.inc,v 1.1.2.1.2.5 2008/06/01 17:15:56 mustafau Exp $

/**
 * @file
 * Administration pages for the Zend Framework.
 */

/**
 * Administration settings for the Zend Framework.
 */
function zend_admin() {
  $zend_ready = zend_ready(FALSE);
  $zend_status = $zend_ready ? 'ok' : 'error';
  $zend_version = $zend_ready ? $zend_ready : t('Not found');
  $form['zend_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Installation Path'),
    '#description' => t('The location where the Zend Framework is installed.  This should be the subdirectory from where it is installed.  If you put the Zend Framework into %moduledir/Zend, then here you would put in %moduledir. Status: <span class="@zendstatus">%zendversion</span>.', array('%moduledir' => drupal_get_path('module', 'zend'), '@zendstatus' => $zend_status, '%zendversion' => $zend_version)),
    '#default_value' => zend_get_path(),
    '#attributes' => array('class' => $zend_status),
  );
  return system_settings_form($form);
}

<?php
// $Id: zend_mail.inc,v 1.1.2.3 2008/06/01 16:54:48 mustafau Exp $

/**
 * Uses Zend_Mail to send emails.
 */
function drupal_mail_wrapper($message) {
  if (zend_initialize('Zend_Mail')) {
    $transport = NULL;
    if (variable_get('zend_mail_transport', NULL) == 'smtp') {
      zend_initialize('Zend_Mail_Transport_Smtp');
      $host = variable_get('zend_mail_smtp_host', 'localhost');
      $port = variable_get('zend_mail_smtp_port', 25);
      $config = array('port' => $port);
      if (variable_get('zend_mail_smtp_encryption', 'none') != 'none') {
        $config['ssl'] = variable_get('zend_mail_smtp_encryption', 'none');
      }
      $transport = new Zend_Mail_Transport_Smtp($host, $config);
      $transport->connect();
    }
    
    $mail = new Zend_Mail();
    foreach ($message['headers'] as $header => $value) {
      if ($header != 'Return-Path' && $header != 'From') {
        $mail->addHeader($header, $value);
      }
    }
    $mail->setBodyText($message['body']);
    $mail->setFrom($message['from']);
    $mail->addTo($message['to']);
    $mail->setSubject($message['subject']);
    try {
      @$mail->send($transport);
    }
    catch (Zend_Mail_Transport_Exception $ex) {
      // Do nothing.
    }
    if ($transport) {
      $transport->disconnect();
    }
    return TRUE;
  }
  return FALSE;
}

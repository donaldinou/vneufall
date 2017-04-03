<?php
// $Id: views_handler_filter_upload_fid.inc,v 1.1 2008/09/03 19:21:30 merlinofchaos Exp $

/**
 * Filter by whether or not a node has attached files from the upload module
 */
class views_handler_filter_immobilier_picture extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = 'Posséde au moins une photo';
  }

  function query() {
    $this->ensure_my_table();
	
	$join = new views_join();
    $join->definition = array(
      'left_table' => 'node',
      'left_field' => 'nid',
      'table' => 'immo_files',
      'field' => 'nid',
	  'extra' => "f.type = 'image'"
    );
	$join->construct();
	
	$this->query->add_relationship('f', $join, 'immo_files', $this->relationship);
	$this->query->add_where($this->options['group'], empty($this->value) ? "f.fid IS NULL" : "f.fid IS NOT NULL");
	$this->query->add_groupby("node.nid");
	
    //$this->query->add_where($this->options['group'], "(SELECT COUNT(*) FROM {immo_files} i WHERE i.vid = $this->table_alias.$this->real_field AND i.type = 'image') " . (empty($this->value) ? '=' : '<>') . " 0");
  }
}
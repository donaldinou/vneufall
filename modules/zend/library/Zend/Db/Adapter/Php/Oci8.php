<?php
require_once 'Zend/Db/Adapter/Php/Dbphp.php';
// {{{ class Zend_Db_Adapter_Php_Oci8 extends Zend_Db_Adapter_Php_Dbphp
/**
 * @package   Zend_Db
 * @copyright Agora Production 2002-2006 (http://agoraproduction.com)
 * @license   New BSD Licence
 * @author    David Coallier <davidc@agoraproduction.com>
 */
class Zend_Db_Adapter_Php_Oci8 extends Zend_Db_Adapter_Php_Dbphp
{
    const ZEND_DB_ORACLE_DBTYPE = 'oci8';
    /**
     * Constructor
     */
    public function __construct($config)
    {
        $config['type'] = self::ZEND_DB_ORACLE_DBTYPE;
        parent::__construct($config);
    }
    // }}}
    // {{{ public function desribeTable
    /**
     * Describe a table
     *
     * This function will return the information
     * related to a table.
     *
     * @access public
     * @param  string $tablename The table name
     * @return array  $res       The table structure
     */
    public function describeTable($tablename)
    {
        $sql = 'DESC ' . $tablename;

        $res = $this->fetchAll($sql);
        return $res;
    }
    // }}}
}
// }}}

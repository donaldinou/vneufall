<?php
require_once 'Zend/Db/Adapter/Php/Dbphp.php';
// {{{ class Zend_Db_Adapter_Php_Mssql extends Zend_Db_Adaptter_Php_Dbphp
/**
 * @package   Zend_Db
 * @copyright Agora Production 2002-2006 (http://agoraproduction.com)
 * @license   New BSD Licence
 * @author    David Coallier <davidc@agoraproduction.com>
 */
class Zend_Db_Adapter_Php_Mssql extends Zend_Db_Adapter_Php_Dbphp
{
    const ZEND_DB_MSSQL_DBTYPE = 'mssql';

    // {{{ Constructor
    public function __construct($config)
    {
        $config['type'] = self::ZEND_DB_MSSQL_DBTYPE;
        parent::__construct($config);
    }
    // }}}
    // {{{ public function describeTable
    /**
     * Describe a table
     *
     * This function describes a table
     * fields, etc.
     *
     * @access public
     * @param  string $tablename  The table to query.
     * @return array  $res        The table infos
     */
    public function describeTable($tablename)
    {
        $sql = "sp_help '$tablename'";

        $res = $this->fetchAll($sql);
        return (array)$res;
    }
    // }}}
}
// }}}

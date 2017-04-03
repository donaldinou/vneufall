<?php
require_once 'Zend/Db/Adapter/Php/Dbphp.php';
// {{{ class Zend_Db_Adapter_Php_Pgsql extends Zend_Db_Adapter_Php_Dbphp
/**
 * @package   Zend_Db
 * @copyright Agora Production 2002-2006 (http://agoraproduction.com)
 * @license   New BSD Licence
 * @author    David Coallier <davidc@agoraproduction.com>
 */
class Zend_Db_Adapter_Php_Pgsql extends Zend_Db_Adapter_Php_Dbphp
{
    const ZEND_DB_PGSQL_DBTYPE = 'pgsql';
    /**
     * Constructor
     */
    public function __construct($config)
    {
        $config['type'] = self::ZEND_DB_PGSQL_DBTYPE;
        parent::__construct($config);
    }
    // }}}
    // {{{ public function describeTable
    /**
     * Returns the column descriptions for a table.
     *
     * @access public
     * @param string $table The table name to lookup
     * @return array
     */
    public function describeTable($table)
    {
        $sql = "
        SELECT column_name, data_type
         FROM information_schema.columns
         WHERE table_name = 'foo'
          ORDER BY  ordinal_position;
        ";
        $res = $this->fetchAll($sql);

        return $res;
    }
    // }}}

}
// }}}

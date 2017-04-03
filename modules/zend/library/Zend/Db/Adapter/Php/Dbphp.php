<?php
/**
 * Zend_Dbphp
 *
 * LICENSE:
 *
 * BSD License
 *
 * Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. Neither the name of Agora Production. nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @category   Zend
 * @package    Zend_Db
 * @subpackage Adapter
 * @author     David Coallier <davidc@agoraproduction.com>
 * @copyright  Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * @license    http://www.opensource.org/licenses/bsd-license.php
 *             BSD License
 */

/**
 * Zend_Db_Adapter_Abstract
 */
require_once 'Zend/Db/Adapter/Abstract.php';

/**
 * Zend_Db_Adapter_Php_Exception
 */
require_once 'Zend/Db/Adapter/Php/Exception.php';

/**
 * MDB2
 */
require_once 'Zend/Db/Adapter/Php/MDB2/MDB2.php';

// {{{ class Zend_Db_Adapter_Php_Dbphp extends Zend_Db_Adapter_Abstract
/**
 * Class for the zend db php adapter
 *
 * This class has the functions that are wrapping around
 * MDB2 in order to allow Zend_Db to use MDB2 somewhat
 * cleanly..
 *
 * @category   Zend
 * @package    Zend_Db
 * @subpackage Adapter
 * @copyright  Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * @license    http://www.opensource.org/licenses/bsd-license.php
 *             BSD License
 * @author     David Coallier <davidc@agoraproduction.com>
 * @author     Firman Wandayandi <firman@php.net>
 */
abstract class Zend_Db_Adapter_Php_Dbphp extends Zend_Db_Adapter_Abstract
{
    // {{{ properties

    /**
     * Adapter name
     *
     * This is the external adapter
     * name.
     *
     * @var ZEND_DB_EXTERNAL_ADAPTER_NAME mdb2 by default
     */
    const ZEND_DB_EXTERNAL_ADAPTER_NAME = 'mdb2';

    /**
     * Fetchmode
     *
     * This is the fetchmode used through the
     * code and to retrieve data when arrays
     * are being called
     *
     * @access protected
     * @var int  fetchmode
     */
    protected $_fetchMode = MDB2_FETCHMODE_ASSOC;

    /**
     * DSN Infos
     *
     * This holds the dsn infos passed from the
     * driver wanted.
     *
     * @access protected
     * @var    array  dsn informations
     */
    protected $_dsn = array();

    // }}}
    // {{{ constructor
    public function __construct($config)
    {
        
        $this->_dsn['phptype']  = $config['type'];
        $this->_dsn['hostspec'] = isset($config['host'])     ? $config['host']     : '';
        $this->_dsn['username'] = isset($config['username']) ? $config['username'] : '';
        $this->_dsn['password'] = isset($config['password']) ? $config['password'] : '';
        $this->_dsn['database'] = $config['dbname'];

        // Attempt to connect
        $this->_connect();

        // keep the config
        $this->_config = array_merge($this->_config, $config);

        // create a profiler object
        $enabled = false;
        if (array_key_exists('profiler', $this->_config)) {
            $enabled = (bool) $this->_config['profiler'];
            unset($this->_config['profiler']);
        }

        $this->_profiler = new Zend_Db_Profiler($enabled);
    }
    // }}}
    // {{{ protected function _connect
    /**
     * Connect to the databae
     *
     * This function will instantiate
     * the connection to the database
     *
     * @access protected
     * @see    $this->_connection
     * @return bool True if the connection is set
     */
    protected function _connect()
    {
        if ($this->_connection) {
            return true;
        }

        /**
         * Here we connect to the
         * database using debug true
         */
        $options = array(
            'debug'       => 'true',
            'portability' => (MDB2_PORTABILITY_ALL ^ MDB2_PORTABILITY_EMPTY_TO_NULL),
        );

        $db = MDB2::singleton($this->_dsn, $options);
        if (MDB2::isError($db)) {
            throw new Zend_Db_Exception($db);
        }

        $this->_connection = $db;
        $this->setFetchMode($this->_fetchMode);
    }
    // }}}
    // {{{ getConnection
    /**
     * Returns the underlying database connection object or resource.  If not
     * presently connected, this may return null.
     *
     * @return object|resource|null
     */
    public function getConnection()
    {
        return $this->_connection;
    }
    // }}}
    // {{{ public function fetchAll
    /**
     * Fetch all results
     *
     * This function will fetch all the results
     * of the query and return an associative
     * array of the values.
     *
     * @access public
     * @param  string $sql  The query to execute
     * @param  string $bind Binder
     * @return array  $result  The query results
     */
    public function fetchAll($sql, $bind = null)
    {
        $this->_connect();
        $result = $this->_connection->queryAll($sql);
        return (array)$result;
    }
    // }}}
    // {{{ public function fetchCol
    /**
     * Fetch a column
     *
     * This function fetches the results
     * of a column and return it's result
     * in an array.
     *
     * @access public
     * @param  string $sql  The query to execute
     * @param  string $bind Binder
     * @return array  $result  The query results
     */
    public function fetchCol($sql, $bind = null)
    {
        $this->_connect();
        $result = $this->_connection->queryCol($sql);
        return (array)$result;
    }
    // }}}
    // {{{ public function fetchOne
    /**
     * Fetch a result
     *
     * This function fetches one result.
     *
     * @access public
     * @param  string $sql  The query to execute
     * @param  string $bind Binder
     * @return string $result  The query results
     */
    public function fetchOne($sql, $bind = null)
    {
        $this->_connect();
        $result = $this->_connection->queryOne($sql);
        return $result;
    }
    // }}}
    // {{{ public function fetchRow
    /**
     * Fetch a row
     *
     * This function fetches the results
     * of a row and return it's result
     * in an array.
     *
     * @access public
     * @param  string $sql  The query to execute
     * @param  string $bind Binder
     * @return array  $result  The query results
     */
    public function fetchRow($sql, $bind = null)
    {
        $this->_connect();
        $result = $this->_connection->queryRow($sql);
        return (array)$result;
    }
    // }}}
    // {{{ public function query
    /**
     * Queries the database
     *
     * This function will execute a query
     * on the database.
     *
     * @access public
     * @param  string $sql  The sql query to execute
     * @param  array  $bind The binder
     * @return error  Returns an error if fails, nothing if not.
     */
    public function query($sql, $bind = array())
    {
        // connect to the database if needed
        $this->_connect();

        // is the $sql a Zend_Db_Select object?
        if ($sql instanceof Zend_Db_Select) {
            $sql = $sql->__toString();
        }

        $stmt = $this->prepare($sql);
        $result = $stmt->execute((array) $bind);
        if (MDB2::isError($result)) {
            throw new Zend_Db_Exception($result->getMessage());
        }

        return $result;
    }
    // }}}
    // {{{ public function insert
    /**
     * Inserts data to the database
     *
     * This function will take an array and insert
     * it's data to the database.
     *
     * @access public
     * @param  string  $table The table to update
     * @param  string  $bind  The fields to update
     * @return int The number of affected rows.
     */
    public function insert($table, $bind = null)
    {
        $this->_connect();

        // col names come from the array keys
        $cols = array_keys($bind);

        foreach ($bind as $val) {
            if (is_null($val)) {
                $vals[] = 'null';
            } else {
                $vals[] = $this->quote($val);
            }
        }

        // build the statement
        $sql = "INSERT INTO $table "
             . '(' . implode(', ', $cols) . ') '
             . 'VALUES (' . implode(', ', $vals) . ')';

        // execute the statement and return the number of affected rows
        $result = $this->_connection->exec($sql);
        if (PEAR::isError($result)) {
            throw new Zend_Db_Adapter_Php_Exception($result);
        }

        return $result;

    }
    // }}}
    // {{{ public function update
    /**
     * Updates data to the database
     *
     * This function will take an array and update
     * it's data to the database.
     *
     * @access public
     * @todo Fix so it works ?
     * @param  string  $table The table to update
     * @param  string  $bind  The fields to update
     * @return int The number of affected rows.
     */
    public function update($table, $bind = null, $where)
    {
        $this->_connect();

        // build "col = :col" pairs for the statement
        $set = array();
        foreach ($bind as $col => $val) {
            $set[] = $this->quoteInto("$col = ?", $val);
        }

        // build the statement
        $sql = "UPDATE $table "
             . 'SET ' . implode(', ', $set)
             . (($where) ? " WHERE $where" : '');

        // execute the statement and return the number of affected rows
        $result = $this->_connection->exec($sql);
        if (PEAR::isError($result)) {
            throw new Zend_Db_Adapter_Php_Exception($result);
        }

        return $result;
    }
    // }}}
    // {{{ public function delete
    /**
     * Deletes table rows based on a WHERE clause.
     *
     * @param string $table The table to udpate.
     * @param string $where DELETE WHERE clause.
     * @return int The number of affected rows.
     */
    public function delete($table, $where)
    {
        $this->_connect();

        // build the statement
        $sql = "DELETE FROM $table"
             . (($where) ? " WHERE $where" : '');

        // execute the statement and return the number of affected rows
        $result = $this->_connection->exec($sql);
        if (PEAR::isError($result)) {
            throw new Zend_Db_Adapter_Php_Exception($result);
        }

        return $result;
    }

    // }}}
    // {{{ public function setFetchMode
    /**
     * Set the fetch mode
     *
     * THis function will set the fetchmode
     * to use thorough the queries
     *
     * @access public
     * @param  int    $mode The fetchmode
     */
    public function setFetchMode($mode)
    {
        $this->_connect();

        switch ($mode) {
            case MDB2_FETCHMODE_OBJECT:
            case MDB2_FETCHMODE_ORDERED:
            case MDB2_FETCHMODE_ASSOC:
                $this->_fetchMode = $mode;
                $this->_connection->setFetchMode($this->_fetchMode);
                break;
            default:
                throw new Zend_Db_Adapter_Exception('Invalid fetch mode specified');
                break;
        }

    }
    // }}}
    // {{{ public function listTable
    /**
     * List tables
     *
     * This function will list the tables of
     * a database.
     *
     * @access public
     * @return array  $res  The list of tables;
     */
    public function listTables()
    {
        $this->_connect();
        $this->_connection->loadModule('Manager', null, true);
        $res = $this->_connection->manager->listTables();

        return $res;
    }
    // }}}
    public function beginTransaction() { }
    public function rollback() { }
    public function commit()  { }
    
    // {{{ select
    /**
     * New Zend_Db_Select Object
     * 
     * Creates and returns a new Zend_Db_Select object for this adapter.
     *
     * @access public
     * @return object Zend_Db_Select
     */
    public function select() 
    {
        return new Zend_Db_Select($this);
    }
    // }}}
    // {{{ quoteIdentifier()
    /**
     * Quotes an identifier.
     *
     * @param string $ident The identifier.
     * @return string The quoted identifier.
     */
    public function quoteIdentifier($string)
    {
        $this->_connect();
        return $this->_connection->quoteIdentifier($string);
    }
    // }}}
    // {{{ prepare()
    /**
     * Prepares an SQL statement.
     *
     * @param string $sql The SQL statement with placeholders.
     * @param array $bind An array of data to bind to the placeholders.
     * @return MDB2_Statement_*
     */
    public function prepare($sql)
    {
        $this->_connect();

        $res = $this->_connection->prepare($sql);
        if (MDB2::isError($res)) {
            throw new Zend_Db_Adapter_Exception($res);
        }
        return $res;
    }
    // }}}
    // {{{ lastInsertId()
    /**
     * Gets the last inserted ID.
     *
     * @param  string $tableName   name of table (or sequence) associated with sequence
     * @param  string $primaryKey  primary key in $tableName
     * @return integer
     */
    public function lastInsertId($tableName = null, $primaryKey = null)
    {
        $this->_connect();
        return (int) $this->_connection->lastInsertID($tableName, $primaryKey);
    }
    // }}}
    // {{{ _beginTransaction()
    /**
     * Begin a transaction.
     */
    protected function _beginTransaction()
    {
        $this->_connect();
        $this->_connection->beginTransaction();
    }
    // }}}
    // {{{ _commit()
    /**
     * Commit a transaction.
     */
    protected function _commit()
    {
        $this->_connect();
        $this->_connection->commit();
    }
    // }}}
    // {{{ _rollBack()
    /**
     * Roll-back a transaction.
     */
    protected function _rollBack()
    {
        $this->_connect();
        $this->_connection->rollback();
    }
    // }}}
    // {{{ _quote()
    /**
     * Quote a raw string.
     *
     * @param string $value     Raw string
     * @return string           Quoted string
     */
    protected function _quote($value)
    {
        $this->_connect();
        return $this->_connection->quote($value);
    }
    // }}}
    // {{{ limit
    /**
     * Adds an adapter-specific LIMIT clause to the SELECT statement.
     *
     * @param mixed $sql
     * @param integer $count
     * @param integer $offset
     * @return string
     */
    public function limit($sql, $count, $offset = 0)
    {
        // Implement limit
    }
    // }}}
    // {{{ public function supportsParameters
    public function supportsParameters($type)
    {
        // @todo Implement it.
    }
     // }}}
    // {{{ public function closeConnection
    public function closeConnection() { }
    // }}}    
}
// }}}

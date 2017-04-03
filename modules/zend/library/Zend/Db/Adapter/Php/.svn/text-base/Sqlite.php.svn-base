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
 * @copyright  Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * @license    http://www.opensource.org/licenses/bsd-license.php
 *             BSD License
 */

/**
 * Zend_Db_Adapter_Php_Dbphp
 */
require_once 'Zend/Db/Adapter/Php/Dbphp.php';

// {{{ class Zend_Db_Adapter_Php_Sqlite extends Zend_Db_Adaptter_Php_Dbphp
/**
 * @category   Zend
 * @package    Zend_Db
 * @subpackage Adapter
 * @copyright  Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * @license    http://www.opensource.org/licenses/bsd-license.php
 *             BSD License
 * @author     David Coallier <davidc@agoraproduction.com>
 * @author     Firman Wandayandi <firman@php.net>
 */
class Zend_Db_Adapter_Php_Sqlite extends Zend_Db_Adapter_Php_Dbphp
{
    /**
     * Database type
     */
    const ZEND_DB_SQLITE_DBTYPE = 'sqlite';

    // {{{ Constructor
    public function __construct($config)
    {
        $config['type'] = self::ZEND_DB_SQLITE_DBTYPE;
        parent::__construct($config);
    }
    // }}}
    // {{{ public function describeTable
    /**
     * Returns the column descriptions for a table.
     *
     * The return value is an associative array keyed by the column name,
     * as returned by the RDBMS.
     *
     * The value of each array element is an associative array
     * with the following keys:
     *
     * SCHEMA_NAME => string; name of database or schema
     * TABLE_NAME  => string;
     * COLUMN_NAME => string; column name
     * COLUMN_POSITION => number; ordinal position of column in table
     * DATA_TYPE   => string; SQL datatype name of column
     * DEFAULT     => string; default expression of column, null if none
     * NULLABLE    => boolean; true if column can have nulls
     * LENGTH      => number; length of CHAR/VARCHAR
     * SCALE       => number; scale of NUMERIC/DECIMAL
     * PRECISION   => number; precision of NUMERIC/DECIMAL
     * UNSIGNED    => boolean; unsigned property of an integer type
     * PRIMARY     => boolean; true if column is part of the primary key
     *
     * @todo Discover column position.
     * @todo Discover integer unsigned property.
     *
     * @param string $tableName
     * @param string $schemaName OPTIONAL
     * @return array
     */
    public function describeTable($tableName, $schemaName = null)
    {
        $sql = "PRAGMA table_info($tableName)";
        $result = $this->fetchAll($sql);
        $desc = array();
        foreach ($result as $key => $row) {
            $desc[$row['name']] = array(
                'SCHEMA_NAME' => null,
                'TABLE_NAME'  => $tableName,
                'COLUMN_NAME' => $row['name'],
                'COLUMN_POSITION' => null, // @todo
                'DATA_TYPE'   => $row['type'],
                'DEFAULT'     => $row['dflt_value'],
                'NULLABLE'    => ! (bool) $row['notnull'],
                'LENGTH'      => null,
                'SCALE'       => null,
                'PRECISION'   => null,
                'UNSIGNED'    => null, // @todo
                'PRIMARY'     => (bool) $row['pk'],
            );
        }
        return $desc;
    }
    // }}}
    // {{{ limit
    /**
     * Adds an adapter-specific LIMIT clause to the SELECT statement.
     *
     * @param string $sql
     * @param integer $count
     * @param integer $offset OPTIONAL
     * @return string
     */
    public function limit($sql, $count, $offset = 0)
    {
        $count = intval($count);
        if ($count <= 0) {
            throw new Zend_Db_Adapter_Exception("LIMIT argument count=$count is not valid");
        }

        $offset = intval($offset);
        if ($offset < 0) {
            throw new Zend_Db_Adapter_Exception("LIMIT argument offset=$offset is not valid");
        }

        $sql .= " LIMIT $count";
        if ($offset > 0) {
            $sql .= " OFFSET $offset";
        }

        return $sql;
    }
    // }}}
}
// }}}

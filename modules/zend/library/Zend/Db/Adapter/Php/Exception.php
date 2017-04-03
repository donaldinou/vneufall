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
 * Zend_Db_Adapter_Exception
 */
require_once 'Zend/Db/Adapter/Exception.php';

// {{{ class Zend_Db_Adapter_Php_Exception extends Zend_Db_Adapter_Exception
/**
 * @category   Zend
 * @package    Zend_Db
 * @subpackage Adapter
 * @copyright  Copyright (c) 2002-2006 Agora Production. (http://agoraproduction.com)
 * @license    http://www.opensource.org/licenses/bsd-license.php
 *             BSD License
 * @author     Firman Wandayandi <firman@php.net>
 */
class Zend_Db_Adapter_Php_Exception extends Zend_Db_Adapter_Exception
{
    protected $message = 'Unknown exception';
    protected $code = 0;

    public function __construct($error = null, $code = 0)
    {
        if ($error instanceof MDB2_Error) {
            $this->code = $error->getCode();
            $this->message = $error->getMessage() . '(' . $error->getUserInfo() . ')';
        }
    }
}
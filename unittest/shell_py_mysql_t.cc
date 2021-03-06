/*
* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; version 2 of the
* License.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
* 02110-1301  USA
*/

#include "shell_script_tester.h"
#include "utils/utils_general.h"
#include <boost/lexical_cast.hpp>

namespace shcore
{
  class Shell_py_mysql_tests : public Shell_py_script_tester
  {
  protected:
    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp()
    {
      Shell_py_script_tester::SetUp();

      int port = 3306, pwd_found;
      std::string protocol, user, password, host, sock, schema, ssl_ca, ssl_cert, ssl_key;
      shcore::parse_mysql_connstring(_uri, protocol, user, password, host, port, sock, schema, pwd_found, ssl_ca, ssl_cert, ssl_key);

      // Setups some variables on the JS context, these will be used on some test cases
      if (_mysql_port.empty())
        _mysql_port = "3306";

      std::string code = "__user = '" + user + "';";
      exec_and_out_equals(code);
      code = "__pwd = '" + password + "';";
      exec_and_out_equals(code);
      code = "__host = '" + host + "';";
      exec_and_out_equals(code);
      code = "__port = " + _mysql_port + ";";
      exec_and_out_equals(code);
      code = "__schema = 'mysql';";
      exec_and_out_equals(code);
      code = "__uri = '" + user + "@" + host + ":" + _mysql_port + "';";
      exec_and_out_equals(code);
      code = "__uripwd = '" + user + ":" + password + "@" + host + ":" + _mysql_port + "';";
      exec_and_out_equals(code);
      code = "__displayuri = '" + user + "@" + host + ":" + _mysql_port + "';";
      exec_and_out_equals(code);
      code = "__displayuridb = '" + user + "@" + host + ":" + _mysql_port + "/mysql';";
      exec_and_out_equals(code);

      // All of the test cases share the same config folder
      // and setup script
      set_config_folder("py_devapi");
      set_setup_script("setup.py");
    }
  };
  TEST_F(Shell_py_mysql_tests, mysql_module)
  {
    validate_interactive("mysql_module.py");
  }

  TEST_F(Shell_py_mysql_tests, mysql_session)
  {
    validate_interactive("mysql_session.py");
  }

  TEST_F(Shell_py_mysql_tests, mysql_schema)
  {
    validate_interactive("mysql_schema.py");
  }

  TEST_F(Shell_py_mysql_tests, mysql_table)
  {
    validate_interactive("mysql_table.py");
  }

  TEST_F(Shell_py_mysql_tests, mysql_view)
  {
    validate_interactive("mysql_view.py");
  }

  TEST_F(Shell_py_mysql_tests, mysql_resultset)
  {
    validate_interactive("mysql_resultset.py");
  }
}
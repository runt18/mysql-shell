/* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include <gtest/gtest.h>
#include "shellcore/lang_base.h"
#include "shellcore/shell_core.h"
#include "shellcore/common.h"
#include <set>
#include "src/interactive_shell.h"

class Shell_test_output_handler
{
public:
  // You can define per-test set-up and tear-down logic as usual.
  Shell_test_output_handler();

  virtual void TearDown(){}

  static void deleg_print(void *user_data, const char *text);
  static void deleg_print_error(void *user_data, const char *text);
  static bool deleg_prompt(void *user_data, const char *UNUSED(prompt), std::string &ret);
  static bool deleg_password(void *user_data, const char *UNUSED(prompt), std::string &ret);

  void wipe_out() { std_out.clear(); }
  void wipe_err() { std_err.clear(); }
  void wipe_all() { std_out.clear(); std_err.clear(); }

  shcore::Interpreter_delegate deleg;
  std::string std_err;
  std::string std_out;
  std::string ret_pwd;

  void validate_stdout_content(const std::string& content, bool expected);
  void validate_stderr_content(const std::string& content, bool expected);

  std::list<std::string> prompts;
};

#define MY_EXPECT_STDOUT_CONTAINS(x) output_handler.validate_stdout_content(x,true)
#define MY_EXPECT_STDERR_CONTAINS(x) output_handler.validate_stderr_content(x,true)
#define MY_EXPECT_STDOUT_NOT_CONTAINS(x) output_handler.validate_stdout_content(x,false)
#define MY_EXPECT_STDERR_NOT_CONTAINS(x) output_handler.validate_stderr_content(x,false)

class Shell_core_test_wrapper : public ::testing::Test
{
protected:
  // You can define per-test set-up and tear-down logic as usual.
  virtual void SetUp();
  virtual void TearDown();

  // void process_result(shcore::Value result);
  shcore::Value execute(const std::string& code);
  shcore::Value exec_and_out_equals(const std::string& code, const std::string& out = "", const std::string& err = "");
  shcore::Value exec_and_out_contains(const std::string& code, const std::string& out = "", const std::string& err = "");

  // This can be use to reinitialize the interactive shell with different options
  // First set the options on _options
  void reset_options()
  {
    char **argv = NULL;
    _options.reset(new Shell_command_line_options(0, argv));
  }

  virtual void set_options()  {};

  void reset_shell()
  {
    _interactive_shell.reset(new Interactive_shell(*_options.get(), &output_handler.deleg));
  }

  Shell_test_output_handler output_handler;
  std::shared_ptr<Interactive_shell> _interactive_shell;
  std::shared_ptr<Shell_command_line_options> _options;
  void wipe_out() { output_handler.wipe_out(); }
  void wipe_err() { output_handler.wipe_err(); }
  void wipe_all() { output_handler.wipe_all(); }

  std::string _port;
  std::string _uri;
  std::string _uri_nopasswd;
  std::string _pwd;
  std::string _mysql_port;
  std::string _mysql_uri;
  std::string _mysql_uri_nopasswd;

  shcore::Value _returned_value;

  shcore::Interpreter_delegate deleg;
};

// Helper class to ease the creation of tests on the CRUD operations
// Specially on the chained methods
class Crud_test_wrapper : public ::Shell_core_test_wrapper
{
protected:
  std::set<std::string> _functions;

  // Sets the functions that will be available for chaining
  // in a CRUD operation
  void set_functions(const std::string &functions);

  // Validates only the specified functions are available
  // non listed functions are validated for unavailability
  void ensure_available_functions(const std::string& functions);
};

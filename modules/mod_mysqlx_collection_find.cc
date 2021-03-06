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
#include <boost/format.hpp>
#include "mod_mysqlx_collection_find.h"
#include "mod_mysqlx_collection.h"
#include "mod_mysqlx_resultset.h"
#include "shellcore/common.h"
#include "mod_mysqlx_expression.h"
#include "mysqlxtest/common/expr_parser.h"
#include "utils/utils_time.h"

using namespace std::placeholders;
using namespace mysh::mysqlx;
using namespace shcore;

CollectionFind::CollectionFind(std::shared_ptr<Collection> owner)
  : Collection_crud_definition(std::static_pointer_cast<DatabaseObject>(owner))
{
  // Exposes the methods available for chaining
  add_method("find", std::bind(&CollectionFind::find, this, _1), "data");
  add_method("fields", std::bind(&CollectionFind::fields, this, _1), "data");
  add_method("groupBy", std::bind(&CollectionFind::group_by, this, _1), "data");
  add_method("having", std::bind(&CollectionFind::having, this, _1), "data");
  add_method("sort", std::bind(&CollectionFind::sort, this, _1), "data");
  add_method("skip", std::bind(&CollectionFind::skip, this, _1), "data");
  add_method("limit", std::bind(&CollectionFind::limit, this, _1), "data");
  add_method("bind", std::bind(&CollectionFind::bind, this, _1), "data");

  // Registers the dynamic function behavior
  register_dynamic_function("find", "");
  register_dynamic_function("fields", "find");
  register_dynamic_function("groupBy", "find, fields");
  register_dynamic_function("having", "groupBy");
  register_dynamic_function("sort", "find, fields, groupBy, having");
  register_dynamic_function("limit", "find, fields, groupBy, having, sort");
  register_dynamic_function("skip", "limit");
  register_dynamic_function("bind", "find, fields, groupBy, having, sort, skip, limit, bind");
  register_dynamic_function("execute", "find, fields, groupBy, having, sort, skip, limit, bind");
  register_dynamic_function("__shell_hook__", "find, fields, groupBy, having, sort, skip, limit, bind");

  // Initial function update
  update_functions("");
}


//! Sets the search condition to identify the Documents to be retrieved from the owner Collection.
#if DOXYGEN_CPP
//! \param args may contain an optional string with the filter expression of the documents to be retrieved.
#else
//! \param searchCondition An optional string with the filter expression of the documents to be retrieved.
#endif
/**
* if not specified all the documents will be included on the result unless a limit is set.
* \return This CollectionFind object.
*
* The searchCondition supports \a [Parameter Binding](param_binding.html).
*
* This function is called automatically when Collection.find(searchCondition) is called.
*
* #### Method Chaining
*
* After this function invocation, the following functions can be invoked:
*
* - fields(List projectedSearchExprStr)
* - groupBy(List searchExprStr)
* - sort(List sortExprStr)
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::find(String searchCondition){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::find(str searchCondition){}
#endif
shcore::Value CollectionFind::find(const shcore::Argument_list &args)
{
  // Each method validates the received parameters
  args.ensure_count(0, 1, "CollectionFind.find");

  std::shared_ptr<Collection> collection(std::static_pointer_cast<Collection>(_owner.lock()));

  if (collection)
  {
    try
    {
      std::string search_condition;
      if (args.size())
        search_condition = args.string_at(0);

      _find_statement.reset(new ::mysqlx::FindStatement(collection->_collection_impl->find(search_condition)));

      // Updates the exposed functions
      update_functions("find");
    }
    CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.find");
  }

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

#if DOXYGEN_CPP
/**
 * Sets a document field filter or a projection defining the ourput format.
 * \param args: may contain two different values depending on the desired output:
 *
 * \li To select document fields: should contain list of string expressions identifying the fields to be extracted, alias support is enabled on these fields.
 * \li To define output format: define a expression representing the layout of the documents to be returned. (mysqlx.expr)
 *
 * \return This CollectionFind object.
 *
 * Calling this function is allowed only for the first time and only if the search criteria has been set by calling CollectionFind.find(searchCriteria), after that its usage is forbidden since the internal class state has been updated to handle the rest of the Find operation.
 *
 * #### Method Chaining
 *
 * This function can be invoked only once after:
 * - find(String searchCondition)
 *
 * After this function invocation, the following functions can be invoked:
 *
 * - groupBy(List searchExprStr)
 * - sort(List sortExprStr)
 * - limit(Integer numberOfRows)
 * - bind(String name, Value value)
 * - execute()
 *
 * \sa Usage examples at execute().
 */
#else
/**
* Sets a document field filter.
* \param projectedSearchExprStr: A list of string expressions identifying the fields to be extracted, alias support is enabled on these fields.
* \return This CollectionFind object.
*
* If called, the CollectionFind operation will only return the fields that were included on the filter.
*
* Calling this function is allowed only for the first time and only if the search criteria has been set by calling CollectionFind.find(searchCriteria), after that its usage is forbidden since the internal class state has been updated to handle the rest of the Find operation.
*
* #### Method Chaining
*
* This function can be invoked only once after:
* - find(String searchCondition)
*
* After this function invocation, the following functions can be invoked:
*
* - groupBy(List searchExprStr)
* - sort(List sortExprStr)
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::fields(List projectedSearchExprStr){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::fields(list projectedSearchExprStr){}
#endif

/**
* Sets a document field projection.
* \param projection: An expression representing the layout of the documents to be returned.
* \return This CollectionFind object.
*
* If called, the CollectionFind operation will return the fields specified on the document expression, the value of each field on the document
* will be calculated executing the indicated operations for each field.
*
* Calling this function is allowed only for the first time and only if the search criteria has been set by calling CollectionFind.find(searchCriteria), after that its usage is forbidden since the internal class state has been updated to handle the rest of the Find operation.
*
* #### Method Chaining
*
* This function can be invoked only once after:
* - find(String searchCondition)
*
* After this function invocation, the following functions can be invoked:
*
* - groupBy(List searchExprStr)
* - sort(List sortExprStr)
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::fields(DocExpression projection);
#elif DOXYGEN_PY
CollectionFind CollectionFind::fields(DocExpression projection);
#endif
#endif
shcore::Value CollectionFind::fields(const shcore::Argument_list &args)
{
  args.ensure_count(1, "CollectionFind.fields");

  try
  {
    if (args[0].type == Array)
    {
      std::vector<std::string> fields;
      parse_string_list(args, fields);

      if (fields.size() == 0)
        throw shcore::Exception::argument_error("Field selection criteria can not be empty");

      _find_statement->fields(fields);
    }
    else if (args[0].type == Object && args[0].as_object()->class_name() == "Expression")
    {
      std::shared_ptr<mysqlx::Expression> expression = std::static_pointer_cast<mysqlx::Expression>(args[0].as_object());
      ::mysqlx::Expr_parser parser(expression->get_data());
      std::unique_ptr<Mysqlx::Expr::Expr> expr_obj(parser.expr());

      // Parsing is done just to validate it is a valid JSON expression
      if (expr_obj->type() == Mysqlx::Expr::Expr_Type_OBJECT)
        _find_statement->fields(expression->get_data());
      else
        throw shcore::Exception::argument_error("Argument #1 is expected to be a JSON expression");
    }
    else
      throw shcore::Exception::argument_error("Argument #1 is expected to be an array or JSON expression");

    update_functions("fields");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.fields");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Sets a grouping criteria for the resultset.
#if DOXYGEN_CPP
//! \param args should contain a list of string expressions identifying the grouping criteria.
#else
//! \param searchExprStr: A list of string expressions identifying the grouping criteria.
#endif
/**
* \return This CollectionFind object.
*
* If used, the CollectionFind operation will group the records using the stablished criteria.
*
* #### Method Chaining
*
* This function can be only once invoked after:
* - find(String searchCondition)
* - fields(List projectedSearchExprStr)
*
* After this function invocation the following functions can be invoked:
*
* - having(String searchCondition)
* - sort(List sortExprStr)
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::groupBy(List searchExprStr){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::group_by(list searchExprStr){}
#endif
shcore::Value CollectionFind::group_by(const shcore::Argument_list &args)
{
  args.ensure_count(1, get_function_name("groupBy").c_str());

  try
  {
    std::vector<std::string> fields;

    parse_string_list(args, fields);

    if (fields.size() == 0)
      throw shcore::Exception::argument_error("Grouping criteria can not be empty");

    _find_statement->groupBy(fields);

    update_functions("groupBy");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION(get_function_name("groupBy"));

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Sets a condition for records to be considered in agregate function operations.
#if DOXYGEN_CPP
//! \param args should contain a condition on the agregate functions used on the grouping criteria.
#else
//! \param searchCondition: A condition on the agregate functions used on the grouping criteria.
#endif
/**
* \return This CollectionFind object.
*
* If used the CollectionFind operation will only consider the records matching the stablished criteria.
*
* The searchCondition supports \a [Parameter Binding](param_binding.html).
*
* #### Method Chaining
*
* This function can be invoked only once after:
*
* - groupBy(List searchExprStr)
*
* After this function invocation, the following functions can be invoked:
*
* - sort(List sortExprStr)
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::having(String searchCondition){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::having(str searchCondition){}
#endif
shcore::Value CollectionFind::having(const shcore::Argument_list &args)
{
  args.ensure_count(1, "CollectionFind.having");

  try
  {
    _find_statement->having(args.string_at(0));

    update_functions("having");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.having");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Sets the sorting criteria to be used on the DocResult.
#if DOXYGEN_CPP
//! \param args should contain a a list of expression strings defining the sort criteria for the returned documents.
#else
//! \param sortExprStr: A list of expression strings defining the sort criteria for the returned documents.
#endif
/**
* \return This CollectionFind object.
*
* If used the CollectionFind operation will return the records sorted with the defined criteria.
*
* The elements of sortExprStr list are strings defining the column name on which the sorting will be based in the form of "attribute [ ASC | DESC ]".
* If no order criteria is specified, ascending will be used by default.
*
* #### Method Chaining
*
* This function can be invoked only once after:
*
* - find(String searchCondition)
* - fields(List projectedSearchExprStr)
* - groupBy(List searchExprStr)
* - having(String searchCondition)
*
* After this function invocation, the following functions can be invoked:
*
* - limit(Integer numberOfRows)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::sort(List sortExprStr){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::sort(list sortExprStr){}
#endif
shcore::Value CollectionFind::sort(const shcore::Argument_list &args)
{
  args.ensure_count(1, "CollectionFind.sort");

  try
  {
    std::vector<std::string> fields;

    parse_string_list(args, fields);

    if (fields.size() == 0)
      throw shcore::Exception::argument_error("Sort criteria can not be empty");

    _find_statement->sort(fields);

    update_functions("sort");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.sort");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Sets the maximum number of documents to be returned on the find operation.
#if DOXYGEN_CPP
//! \param args should contain the maximum number of documents to be retrieved.
#else
//! \param numberOfRows: The maximum number of documents to be retrieved.
#endif
/**
* \return This CollectionFind object.
*
* If used, the CollectionFind operation will return at most numberOfRows documents.
*
* #### Method Chaining
*
* This function can be invoked only once after:
*
* - find(String searchCondition)
* - fields(List projectedSearchExprStr)
* - groupBy(List searchExprStr)
* - having(String searchCondition)
* - sort(List sortExprStr)
*
* After this function invocation, the following functions can be invoked:
*
* - skip(Integer limitOffset)
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::limit(Integer numberOfRows){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::limit(int numberOfRows){}
#endif
shcore::Value CollectionFind::limit(const shcore::Argument_list &args)
{
  args.ensure_count(1, "CollectionFind.limit");

  try
  {
    _find_statement->limit(args.uint_at(0));

    update_functions("limit");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.limit");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Sets number of documents to skip on the resultset when a limit has been defined.
#if DOXYGEN_CPP
//! \param args should contain the number of documents to skip before start including them on the DocResult.
#else
//! \param limitOffset: The number of documents to skip before start including them on the DocResult.
#endif
/**
* \return This CollectionFind object.
*
* #### Method Chaining
*
* This function can be invoked only once after:
*
* - limit(Integer numberOfRows)
*
* After this function invocation, the following functions can be invoked:
*
* - bind(String name, Value value)
* - execute()
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::skip(Integer limitOffset){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::skip(int limitOffset){}
#endif
shcore::Value CollectionFind::skip(const shcore::Argument_list &args)
{
  args.ensure_count(1, "CollectionFind.skip");

  try
  {
    _find_statement->skip(args.uint_at(0));

    update_functions("skip");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.skip");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

//! Binds a value to a specific placeholder used on this CollectionFind object.
#if DOXYGEN_CPP
//! \param args should contain the place holder name and the value to be bound on it.
#else
//! \param name: The name of the placeholder to which the value will be bound.
//! \param value: The value to be bound on the placeholder.
#endif
/**
* \return This CollectionFind object.
*
* #### Method Chaining
*
* This function can be invoked multiple times right before calling execute:
*
* After this function invocation, the following functions can be invoked:
*
* - bind(String name, Value value)
* - execute()
*
* An error will be raised if the placeholder indicated by name does not exist.
*
* This function must be called once for each used placeohlder or an error will be
* raised when the execute method is called.
*
* \sa Usage examples at execute().
*/
#if DOXYGEN_JS
CollectionFind CollectionFind::bind(String name, Value value){}
#elif DOXYGEN_PY
CollectionFind CollectionFind::bind(str name, Value value){}
#endif
shcore::Value CollectionFind::bind(const shcore::Argument_list &args)
{
  args.ensure_count(2, "CollectionFind.bind");

  try
  {
    _find_statement->bind(args.string_at(0), map_document_value(args[1]));

    update_functions("bind");
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.bind");

  return Value(std::static_pointer_cast<Object_bridge>(shared_from_this()));
}

/**
* Executes the Find operation with all the configured options and returns.
* \return CollectionResultset A DocResult object that can be used to traverse the collections returned by the find operation.
*
* #### Method Chaining
*
* This function can be invoked after any other function on this class.
*/
#if DOXYGEN_JS
/**
*
* #### Examples
* \dontinclude "js_devapi/scripts/mysqlx_collection_find.js"
* \skip //@ Collection.Find All
* \until print(columns[1], ':', record.InThreeYears, '\n');
*/
DocResult CollectionFind::execute(){}
#elif DOXYGEN_PY
/**
*
* #### Examples
* \dontinclude "py_devapi/scripts/mysqlx_collection_find.py"
* \skip #@ Collection.Find All
* \until print "%s: %s\n" % (columns[1], record.InThreeYears)
*/
DocResult CollectionFind::execute(){}
#endif
shcore::Value CollectionFind::execute(const shcore::Argument_list &args)
{
  mysqlx::DocResult *result = NULL;

  try
  {
    args.ensure_count(0, "CollectionFind.execute");
    MySQL_timer timer;
    timer.start();
    result = new mysqlx::DocResult(std::shared_ptr< ::mysqlx::Result>(_find_statement->execute()));
    timer.end();
    result->set_execution_time(timer.raw_duration());
  }
  CATCH_AND_TRANSLATE_CRUD_EXCEPTION("CollectionFind.execute");

  return result ? shcore::Value::wrap(result) : shcore::Value::Null();
}
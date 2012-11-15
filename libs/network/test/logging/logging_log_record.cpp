// Copyright (c) 2012 A. Joel Lamotte <mjklaim@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>

#define BOOST_TEST_MODULE logging log_record
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>

#include <network/logging/logging.hpp>

using namespace network::logging;

BOOST_AUTO_TEST_CASE(default_constructor) {
  log_record record;
  BOOST_CHECK( record.message() == "" );
  BOOST_CHECK( record.filename() == log_record::UNKNOWN_FILE_NAME );
  BOOST_CHECK( record.line() == 0 );
}

BOOST_AUTO_TEST_CASE(cstring_constructor) {
  const auto message = "This is a test.";
  log_record record( message );
  BOOST_CHECK( record.message() == message );
  BOOST_CHECK( record.filename() == log_record::UNKNOWN_FILE_NAME );
  BOOST_CHECK( record.line() == 0 );
}

BOOST_AUTO_TEST_CASE(string_constructor) {
  const std::string message("This is a test.");
  log_record record( message );
  BOOST_CHECK( record.message() == message );
  BOOST_CHECK( record.filename() == log_record::UNKNOWN_FILE_NAME );
  BOOST_CHECK( record.line() == 0 );
}

BOOST_AUTO_TEST_CASE(int_constructor) {
  const auto num = 42;
  log_record record( num );
  BOOST_CHECK( record.message() == std::to_string( num ) );
  BOOST_CHECK( record.filename() == log_record::UNKNOWN_FILE_NAME );
  BOOST_CHECK( record.line() == 0 );
}

BOOST_AUTO_TEST_CASE(info_constructor) {
  const auto line_num = 42;
  const auto file_name = "somewhere.cpp";
  log_record record( file_name, line_num );
  BOOST_CHECK( record.message() == "" );
  BOOST_CHECK( record.filename() == file_name );
  BOOST_CHECK( record.line() == line_num );
}

BOOST_AUTO_TEST_CASE(text_stream) {
  const auto line_num = 42;
  const auto file_name = "somewhere.cpp";
  const auto message = "At line " + std::to_string(line_num) + " we check the code.";
  log_record record( file_name, line_num );

  record << "At line " << line_num << " we check the code.";

  BOOST_CHECK( record.message() == message );
  BOOST_CHECK( record.filename() == file_name );
  BOOST_CHECK( record.line() == line_num );
}

BOOST_AUTO_TEST_CASE(raw_log) {
  log( "This is a raw log." );
}
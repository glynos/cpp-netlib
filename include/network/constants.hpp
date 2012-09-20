#ifndef NETWORK_CONSTANTS_HPP_20100808
#define NETWORK_CONSTANTS_HPP_20100808

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace network {

struct constants {
  static char const * crlf();
  static char const * dot();
  static char dot_char();
  static char const * http_slash();
  static char const * space();
  static char space_char();
  static char const * slash();
  static char slash_char();
  static char const * host();
  static char const * colon();  
  static char colon_char();
  static char const * accept();
  static char const * default_accept_mime();
  static char const * accept_encoding();
  static char const * default_accept_encoding();
  static char const * user_agent();
  static char const * default_user_agent();
  static char const * cpp_netlib_slash();
  static char question_mark_char();
  static char hash_char();
  static char const * connection();
  static char const * close();
  static char const * https();
};

} // namespace network

#endif // NETWORK_CONSTANTS_HPP_20100808

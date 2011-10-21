#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/message_base.hpp>

namespace boost { namespace network {

namespace impl {

struct header_directive {

  explicit header_directive(std::string const & header_name, 
                            std::string const & header_value) :
      _header_name(header_name),
      _header_value(header_value)
  { };

  void operator() (message_base & msg) const {
    msg.append_header(_header_name, _header_value);
  }

 private:

  std::string const & _header_name;
  std::string const & _header_value;
};

} // namespace impl

inline impl::header_directive
header(std::string const & header_name, std::string const & header_value) {
    return impl::header_directive(header_name, header_value);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

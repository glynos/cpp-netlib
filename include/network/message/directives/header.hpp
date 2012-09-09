#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/message/message_base.hpp>

namespace boost { namespace network {

namespace impl {

struct header_directive {
  explicit header_directive(std::string const & name,
                            std::string const & value);
  void operator() (message_base & msg) const;
 private:
  std::string const & name_;
  std::string const & value_;
};

} // namespace impl

inline impl::header_directive
header(std::string const & header_name, std::string const & header_value) {
    return impl::header_directive(header_name, header_value);
}
} // namespace network
} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <network/message/directives/header.ipp>
#endif

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

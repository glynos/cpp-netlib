// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

#include <network/message/message_base.hpp>

namespace network {

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

#endif // NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

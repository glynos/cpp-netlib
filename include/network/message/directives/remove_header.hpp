#ifndef BOOST_NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP_20111021
#define BOOST_NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP_20111021

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network {

namespace impl {

struct remove_header_directive {
  explicit remove_header_directive(std::string const & header_name);
  void operator() (message_base & msg) const;
 private:
  std::string const & header_name_;
};

} // namespace impl

inline impl::remove_header_directive const
remove_header(std::string const & header_name) {
    return impl::remove_header_directive(header_name);
}
} // namespace network
} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#undef BOOST_NETWORK_NO_LIB
#endif

#endif // BOOST_NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP_20111021

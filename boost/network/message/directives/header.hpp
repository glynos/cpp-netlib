#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message_base.hpp>

namespace boost { namespace network {

namespace impl {

template <class KeyType, class ValueType>
struct header_directive {

  explicit header_directive(KeyType const & header_name, 
                            ValueType const & header_value) :
      _header_name(header_name),
      _header_value(header_value)
  { };

  void operator() (message_base & msg) const {
    msg.append_header(_header_name, _header_value);
  }

 private:

  KeyType const & _header_name;
  ValueType const & _header_value;
};

} // namespace impl

template <class T1, class T2>
inline impl::header_directive<T1, T2>
header(T1 const & header_name, T2 const & header_value) {
    return impl::header_directive<T1, T2>(header_name, header_value);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__


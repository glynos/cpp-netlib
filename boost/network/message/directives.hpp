
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HPP__

/** Include all the various directive headers.
 */

#include <boost/network/message/directives/header.hpp>
#include <boost/network/message/directives/body.hpp>
#include <boost/network/message/directives/source.hpp>
#include <boost/network/message/directives/destination.hpp>
#include <boost/network/message/directives/remove_header.hpp>

namespace boost { namespace network {

    template <class Tag, class Directive>
        inline basic_message<Tag> &
        operator<< (basic_message<Tag> & message_, Directive const & directive) {
            directive(message_);
            return message_;
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_HPP__


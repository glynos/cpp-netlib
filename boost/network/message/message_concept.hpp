//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_INC__
# define __BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_INC__


# include <boost/concept_check.hpp>


namespace boost {
namespace network {
template <
    class M
    >
struct Message
    : DefaultConstructible<M>,
      CopyConstructible<M>,
      Assignable<M> {
    typedef typename M::string_type string_type;
    typedef typename M::headers_container_type headers_container_type;

    BOOST_CONCEPT_USAGE(Message) {
        M message_;
        swap(message, message_);
        
        headers_container_type headers_ = headers(message);
        string_type body_ = body(message);
        string_type source_ = source(message);
        string_type destination_ = destination(message);

        message << source(string_type())
            << destination(string_type())
            << header(string_type(), string_type())
            << body(string_type());

        (void)headers_;
        (void)body_;
        (void)source_;
        (void)destination_;
    }

private:

    M message;
};
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_INC__

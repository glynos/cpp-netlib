//            Copyright (c) Glyn Matthews 2010.
//            Copyright 2010 (c) Dean Michael Berris.
//            Copyright 2010 (c) Sinefunc, Inc.
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

        typedef typename traits::body<M>::type body_type;
        typedef typename traits::source<M>::type source_type;
        typedef typename traits::destination<M>::type destination_type;

        typedef typename traits::header_key<M>::type header_key_type;
        typedef typename traits::header_value<M>::type header_value_type;

        headers_container_type headers_ = headers(message);
        string_type body_ = body(message);
        string_type source_ = source(message);
        string_type destination_ = destination(message);

        message << source(source_type())
            << destination(destination_type())
            << header(string_type(), string_type())
            << body(body_type());

        add_header(message, string_type(), string_type());
        remove_header(message, string_type());
        clear_headers(message);
        source(message, source_type());
        destination(message, destination_type());
        body(message, body_type());

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


#ifndef BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_HPP_20100903
#define BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_HPP_20100903

// Copyright (c) Glyn Matthews 2010.
// Copyright 2010 (c) Dean Michael Berris.
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>
#include <network/message/wrappers.hpp>
#include <network/message/transformers.hpp>
#include <network/message/directives.hpp>
#include <map>

namespace boost { namespace network {
    
    template <class M>
    struct Message 
        : DefaultConstructible<M>, CopyConstructible<M>, Assignable<M> {

        BOOST_CONCEPT_USAGE(Message) {
            M message_;
            swap(message, message_);
            typedef std::string source_type;
            typedef std::string destination_type;
            typedef std::string body_type;
            typedef std::string header_key_type;
            typedef std::string header_value_type;

            std::multimap<std::string, std::string> headers_ = headers(message);
            std::string body_ = body(message);
            std::string source_ = source(message);
            std::string destination_ = destination(message);

            message << source(source_type())
                << destination(destination_type())
                << header(std::string(), std::string())
                << body(body_type());

            add_header(message, std::string(), std::string());
            remove_header(message, std::string());
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

#endif // BOOST_NETWORK_MESSAGE_MESSAGE_CONCEPT_HPP_20100903


#ifndef BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/network/protocol/http/traits/resolver_policy.hpp>
#include <boost/network/protocol/http/detail/connection_helper.hpp>
#include <boost/network/protocol/http/impl/sync_connection_base.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct simple_connection_policy : resolver_policy<Tag>::type {
        protected:

        typedef typename string<Tag>::type string_type;
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type &, string_type const &, string_type const &)> resolver_function_type;

        struct connection_impl {
            connection_impl(resolver_type & resolver, bool follow_redirect, string_type const & hostname, string_type const & port, resolver_function_type resolve, bool https) 
            : pimpl()
            , follow_redirect_(follow_redirect) 
            {
                pimpl.reset(impl::sync_connection_base<Tag,version_major,version_minor>::new_connection(resolver, resolve, https));
            }

            basic_response<Tag> send_request(string_type const & method, basic_request<Tag> request_, bool get_body) {
                basic_response<Tag> response_;
                do {
                    pimpl->init_socket(request_.host(), lexical_cast<string_type>(request_.port()));
                    pimpl->send_request_impl(method, request_);

                    response_ = basic_response<Tag>();
                    response_ << source(request_.host());

                    boost::asio::streambuf response_buffer;
                    pimpl->read_status(response_, response_buffer);
                    pimpl->read_headers(response_, response_buffer);
                    if (get_body) pimpl->read_body(response_, response_buffer);

                    if (follow_redirect_) {
                        boost::uint16_t status = response_.status();
                        if (status >= 300 && status <= 307) {
                            typename headers_range<http::basic_response<Tag> >::type location_range = headers(response_)["Location"];
                            typename range_iterator<typename headers_range<http::basic_request<Tag> >::type>::type location_header = begin(location_range);
                            if (location_header != end(location_range)) {
                                request_.uri(location_header->second);
                            } else throw std::runtime_error("Location header not defined in redirect response.");
                        } else break;
                    } else break;
                } while(true);

                return response_;
            }

            private:

            shared_ptr<http::impl::sync_connection_base<Tag,version_major,version_minor> > pimpl;
            bool follow_redirect_;

        };

        typedef boost::shared_ptr<connection_impl> connection_ptr;
        connection_ptr get_connection(resolver_type & resolver, basic_request<Tag> const & request_) {
            connection_ptr connection_(
                new connection_impl(
                    resolver
                    , follow_redirect_
                    , request_.host()
                    , lexical_cast<string_type>(request_.port())
                    , bind(
                        &simple_connection_policy<Tag,version_major,version_minor>::resolve,
                        this,
                        _1, _2, _3
                        )
                    , boost::iequals(request_.protocol(), string_type("https"))
                    )
                );
            return connection_;
        }

        void cleanup() { }

        simple_connection_policy(bool cache_resolved, bool follow_redirect) 
        : resolver_base(cache_resolved), follow_redirect_(follow_redirect) {}

        // member variables
        bool follow_redirect_;

    };

} // namespace http

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214


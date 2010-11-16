#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215

//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/network/traits/ostringstream.hpp>
#include <boost/network/protocol/http/message.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <ostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <map>

#include <boost/network/protocol/http/client/facade.hpp>
#include <boost/network/protocol/http/client/pimpl.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client
        : basic_client_facade<Tag, basic_client<Tag,version_major,version_minor> >
    {
    private:
        typedef basic_client_impl<Tag,version_major,version_minor> pimpl_type;
        typedef basic_client_facade<Tag, basic_client<Tag,version_major,version_minor> > base_facade_type;
    public:
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        typedef typename string<Tag>::type string_type;
        typedef Tag tag_type;

        struct cache_resolved_type { };

        static cache_resolved_type cache_resolved() {
            return cache_resolved_type();
        };

        struct follow_redirect_type { };

        static follow_redirect_type follow_redirects() {
            return follow_redirect_type();
        };

        static follow_redirect_type follow_redirect() {
            return follow_redirect_type();
        };

        // Constructors
        // =================================================================
        basic_client()
        : base_facade_type(), pimpl(new pimpl_type(false, false))
        {}

        explicit basic_client(cache_resolved_type (*)())
        : base_facade_type(), pimpl(new pimpl_type(true, false))
        {}

        explicit basic_client(follow_redirect_type (*)())
        : base_facade_type(), pimpl(new pimpl_type(false, true))
        {}

        basic_client(cache_resolved_type (*)(), follow_redirect_type (*)())
        : base_facade_type(), pimpl(new pimpl_type(true, true))
        {}

        explicit basic_client(boost::asio::io_service & io_service)
        : base_facade_type(), pimpl(new pimpl_type(false, false, io_service))
        {}

        //
        // =================================================================

        ~basic_client() 
        {}

    private:
        
        boost::shared_ptr<pimpl_type> pimpl;

        friend struct basic_client_facade<Tag,basic_client<Tag,version_major,version_minor> > ;

        basic_response<Tag> const request_skeleton(request const & request_, string_type method, bool get_body) {
            return pimpl->request_skeleton(request_, method, get_body);
        }

        void clear_resolved_cache() {
            pimpl->clear_resolved_cache();
        }

    };

    typedef basic_client<tags::http_default_8bit_udp_resolve, 1, 0> client;

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215

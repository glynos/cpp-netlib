#ifndef __BOOST_NETWORK_URI_HTTP_URI_INC__
# define __BOOST_NETWORK_URI_HTTP_URI_INC__


# include <boost/network/uri/uri.hpp>


namespace boost {
namespace network {
namespace uri {
namespace http {
template <
    class Tag
    >
class basic_uri
    : public boost::network::uri::basic_uri<Tag> {

    typedef boost::network::uri::basic_uri<Tag> base_type;

public:

    basic_uri() {

    }

    basic_uri(const typename base_type::string_type &uri) : base_type(uri) {

    }

    basic_uri &operator = (const typename base_type::string_type &uri) {
        basic_uri(uri).swap(*this);
        return *this;
    }

};


template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type port(const basic_uri<Tag> &uri) {
    typedef typename basic_uri<Tag>::range_type range_type;
    typedef typename basic_uri<Tag>::string_type string_type;

    static const char default_http_port[] = "80";
    static const char default_https_port[] = "443";

    range_type port = uri.port_range();
    string_type scheme = uri.scheme();

    if (boost::empty(port)) {
        if (scheme == "http") {
            return string_type(boost::begin(default_http_port),
                               boost::end(default_http_port));
        }
        else if (scheme == "https") {
            return string_type(boost::begin(default_https_port),
                               boost::end(default_https_port));
        }
    }
    return string_type(boost::begin(port), boost::end(port));
}

typedef basic_uri<tags::default_string> uri;
} // namespace http
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_HTTP_URI_INC__

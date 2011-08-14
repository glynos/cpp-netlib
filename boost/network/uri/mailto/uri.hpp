#ifndef __BOOST_NETWORK_URI_MAILTO_URI_INC__
# define __BOOST_NETWORK_URI_MAILTO_URI_INC__


# include <boost/network/uri/uri.hpp>


namespace boost {
namespace network {
namespace uri {
namespace mailto {
namespace details {
template <
    class String
    >
struct uri_parts
    : boost::fusion::vector<
      iterator_range<String>        // to
    // headers
    >
{ };
} // namespace details
} // namespace mailto
} // namespace uri
} // namespace network
} // namespace boost


namespace boost {
namespace network {
namespace uri {
namespace mailto {

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
} // namespace mailto

template <
    class Tag
    >
bool is_mailto(const mailto::basic_uri<Tag> &uri) {
    static const char scheme_mailto[] = {'m', 'a', 'i', 'l', 't', 'o'};
    return boost::equal(uri.scheme_range(), scheme_mailto);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_MAILTO_URI_INC__

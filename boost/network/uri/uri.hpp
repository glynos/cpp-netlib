// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_INC__
# define __BOOST_NETWORK_URI_INC__


# include <boost/network/traits/string.hpp>
# include <boost/network/tags.hpp>
# include <boost/network/constants.hpp>
# include <boost/network/uri/detail/uri_parts.hpp>
# ifdef BOOST_NETWORK_NO_LIB
#  include <boost/network/uri/detail/parse_uri.hpp>
# endif // #ifdef BOOST_NETWORK_NO_LIB
# include <boost/fusion/include/std_pair.hpp>
# include <boost/algorithm/string.hpp>
# include <boost/range/iterator_range.hpp>
# include <boost/operators.hpp>
# include <boost/utility/swap.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/optional.hpp>
# include <algorithm>


namespace boost {
namespace network {
namespace uri {
namespace detail {
bool parse(std::string::const_iterator first,
           std::string::const_iterator last,
           uri_parts<std::string> &parts);

bool parse(std::wstring::const_iterator first,
           std::wstring::const_iterator last,
           uri_parts<std::wstring> &parts);
} // namespace detail


template <
    class Tag
    >
class basic_uri
    : public boost::equality_comparable<basic_uri<Tag> > {

public:

    typedef typename string<Tag>::type string_type;
    typedef typename string_type::iterator iterator_type;
    typedef boost::iterator_range<iterator_type> range_type;
    typedef typename string_type::const_iterator const_iterator_type;
    typedef boost::iterator_range<const_iterator_type> const_range_type;

    basic_uri() : is_valid_(false) {

    }

    basic_uri(const string_type &uri)
        : uri_(uri), is_valid_(false) {
        parse();
    }

    basic_uri(const basic_uri &other)
        : uri_(other.uri_),
          uri_parts_(other.uri_parts_),
          is_valid_(other.is_valid_) {

    }

    basic_uri &operator = (const basic_uri &other) {
        basic_uri(other).swap(*this);
        return *this;
    }

    basic_uri &operator = (const string_type &uri) {
        basic_uri(uri).swap(*this);
        return *this;
    }

    ~basic_uri() {

    }

    void swap(basic_uri &other) {
        boost::swap(uri_, other.uri_);
        parse();
    }

    iterator_type begin() {
        return uri_.begin();
    }

    const_iterator_type begin() const {
        return uri_.begin();
    }

    iterator_type end() {
        return uri_.end();
    }

    const_iterator_type end() const {
        return uri_.end();
    }

    const_range_type scheme_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<0>(uri_parts_)),
                                at_c<1>(at_c<0>(uri_parts_)));
    }

    const_range_type user_info_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<0>(at_c<1>(uri_parts_))),
                                at_c<1>(at_c<0>(at_c<1>(uri_parts_))));
    }

    const_range_type host_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<1>(at_c<1>(uri_parts_))),
                                at_c<1>(at_c<1>(at_c<1>(uri_parts_))));
    }

    const_range_type port_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<2>(at_c<1>(uri_parts_))),
                                at_c<1>(at_c<2>(at_c<1>(uri_parts_))));
    }

    const_range_type path_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<3>(at_c<1>(uri_parts_))),
                                at_c<1>(at_c<3>(at_c<1>(uri_parts_))));
    }

    const_range_type query_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<2>(uri_parts_)),
                                at_c<1>(at_c<2>(uri_parts_)));
    }

    const_range_type fragment_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<3>(uri_parts_)),
                                at_c<1>(at_c<3>(uri_parts_)));
    }

    string_type scheme() const {
        const_range_type range = scheme_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type user_info() const {
        const_range_type range = user_info_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type host() const {
        const_range_type range = host_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type port() const {
        const_range_type range = port_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type path() const {
        const_range_type range = path_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type query() const {
        const_range_type range = query_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type fragment() const {
        const_range_type range = fragment_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type string() const {
        return uri_;
    }

    string_type raw() const {
        return string();
    }

    bool is_valid() const {
        return is_valid_;
    }

private:

    void parse();

    string_type uri_;
    detail::uri_parts<typename boost::network::string<Tag>::type> uri_parts_;
    bool is_valid_;

};

template <
    class Tag
    >
inline
void basic_uri<Tag>::parse() {
    const_iterator_type first(boost::begin(uri_)), last(boost::end(uri_));
    is_valid_ = detail::parse(first, last, uri_parts_);
}

template <
    class Tag
    >
inline
void swap(basic_uri<Tag> &lhs, basic_uri<Tag> &rhs) {
    lhs.swap(rhs);
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type scheme(const basic_uri<Tag> &uri) {
    return uri.scheme();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type user_info(const basic_uri<Tag> &uri) {
    return uri.user_info();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type host(const basic_uri<Tag> &uri) {
    return uri.host();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type port(const basic_uri<Tag> &uri) {
    return uri.port();
}

template <
    class Tag
    >
struct port_wrapper {
    const basic_uri<Tag> &uri;

    port_wrapper(const basic_uri<Tag> &uri) : uri(uri) {

    }

    boost::optional<unsigned short> to_optional() const {
        typename basic_uri<Tag>::string_type port = uri.port();
        return (port.empty())?
            boost::optional<unsigned short>() :
            boost::optional<unsigned short>(boost::lexical_cast<unsigned short>(port));
    }

    operator boost::optional<unsigned short> () const {
        return to_optional();
    }

    operator unsigned short () const {
        typedef typename string<Tag>::type string_type;
        typedef constants<Tag> consts;
        const boost::optional<unsigned short> &port = to_optional();
        if (port) return *port;
        return boost::iequals(uri.scheme_range(), string_type(consts::https())) ? 443 : 80;
    }
};

template <
    class Tag
    >
inline
port_wrapper<Tag> port_us(const basic_uri<Tag> &uri) {
    return port_wrapper<Tag>(uri);
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type path(const basic_uri<Tag> &uri) {
    return uri.path();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type query(const basic_uri<Tag> &uri) {
    return uri.query();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type fragment(const basic_uri<Tag> &uri) {
    return uri.fragment();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type authority(const basic_uri<Tag> &uri) {
    typename basic_uri<Tag>::const_range_type user_info(uri.user_info_range());
    typename basic_uri<Tag>::const_range_type port(uri.port_range());
    return typename basic_uri<Tag>::string_type(user_info.begin(), port.end());
}

template <
    class Tag
    >
inline
bool is_valid(const basic_uri<Tag> &uri) {
    return uri.is_valid();
}

template <
    class Tag
    >
inline
bool operator == (const basic_uri<Tag> &lhs, const basic_uri<Tag> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_INC__

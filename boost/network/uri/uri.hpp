// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_INC__
# define __BOOST_NETWORK_URI_INC__


# include <boost/network/traits/string.hpp>
# include <boost/network/constants.hpp>
# include <boost/network/uri/detail/uri_parts.hpp>
# include <boost/fusion/sequence/intrinsic/at_c.hpp>
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
    class String
    >
class basic_uri
    : public boost::equality_comparable<basic_uri<String> > {

public:

    typedef String string_type;
    typedef typename string_type::iterator iterator;
    typedef boost::iterator_range<iterator> range_type;
    typedef typename string_type::const_iterator const_iterator;
    typedef boost::iterator_range<const_iterator> const_range_type;

    basic_uri()
        : is_valid_(false) {

    }

    template <
        class FwdIter
        >
    basic_uri(const FwdIter &first, const FwdIter &last)
        : uri_(first, last), is_valid_(false) {
        parse();
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

    iterator begin() {
        return uri_.begin();
    }

    const_iterator begin() const {
        return uri_.begin();
    }

    iterator end() {
        return uri_.end();
    }

    const_iterator end() const {
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

    bool is_valid() const {
        return is_valid_;
    }

    void append(const string_type &data) {
        uri_.append(data);
        parse();
    }

    template <
        class FwdIter
        >
    void append(const FwdIter &first, const FwdIter &last) {
        uri_.append(first, last);
        parse();
    }

private:

    void parse();

    string_type uri_;
    detail::uri_parts<String> uri_parts_;
    bool is_valid_;

};

template <
    class String
    >
inline
void basic_uri<String>::parse() {
    const_iterator first(boost::begin(uri_)), last(boost::end(uri_));
    is_valid_ = detail::parse(first, last, uri_parts_);
}

template <
    class String
    >
inline
String scheme(const basic_uri<String> &uri) {
    return uri.scheme();
}

template <
    class String
    >
inline
String user_info(const basic_uri<String> &uri) {
    return uri.user_info();
}

template <
    class String
    >
inline
String host(const basic_uri<String> &uri) {
    return uri.host();
}

template <
    class String
    >
inline
String port(const basic_uri<String> &uri) {
    return uri.port();
}

template <
    class String
    >
inline
boost::optional<unsigned short> port_us(const basic_uri<String> &uri) {
    String port = uri.port();
    return (port.empty())?
        boost::optional<unsigned short>() :
        boost::optional<unsigned short>(boost::lexical_cast<unsigned short>(port));
}

template <
    class String
    >
inline
String path(const basic_uri<String> &uri) {
    return uri.path();
}

template <
    class String
    >
inline
String query(const basic_uri<String> &uri) {
    return uri.query();
}

template <
    class String
    >
inline
String fragment(const basic_uri<String> &uri) {
    return uri.fragment();
}

template <
    class String
    >
inline
String authority(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type user_info(uri.user_info_range());
    typename basic_uri<String>::const_range_type port(uri.port_range());
    return String(user_info.begin(), port.end());
}

template <
    class String
    >
inline
String netloc(const basic_uri<String> &uri) {
    return authority(uri);
}

template <
    class String
    >
inline
bool valid(const basic_uri<String> &uri) {
    return uri.is_valid();
}

template <
    class String
    >
inline
bool is_valid(const basic_uri<String> &uri) {
    return valid(uri);
}

template <
    class String
    >
inline
bool operator == (const basic_uri<String> &lhs, const basic_uri<String> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
} // namespace uri
} // namespace network

template <
    class String
    >
inline
typename network::uri::basic_uri<String>::iterator begin(network::uri::basic_uri<String> &uri) {
    return uri.begin();
}

template <
    class String
    >
inline
typename network::uri::basic_uri<String>::iterator end(network::uri::basic_uri<String> &uri) {
    return uri.end();
}

template <
    class String
    >
inline
typename network::uri::basic_uri<String>::const_iterator begin(const network::uri::basic_uri<String> &uri) {
    return uri.begin();
}

template <
    class String
    >
inline
typename network::uri::basic_uri<String>::const_iterator end(const network::uri::basic_uri<String> &uri) {
    return uri.end();
}

template <
    class String
    >
inline
void swap(network::uri::basic_uri<String> &lhs, network::uri::basic_uri<String> &rhs) {
    lhs.swap(rhs);
}
} // namespace boost


# include <boost/network/uri/accessors.hpp>
# include <boost/network/uri/directives.hpp>


#endif // __BOOST_NETWORK_URI_INC__

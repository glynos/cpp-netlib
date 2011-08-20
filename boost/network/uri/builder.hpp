//            Copyright (c) Glyn Matthews 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_BUILDER_INC__
# define __BOOST_NETWORK_URI_BUILDER_INC__


# include <boost/network/uri/uri.hpp>


namespace boost {
namespace network {
namespace uri {
namespace builder {
template <
    class T
    >
struct scheme_t {
    scheme_t(const T &scheme)
        : scheme(scheme) {

    }

    const T &scheme;
};

template <
    class T
    >
inline
scheme_t<T> scheme(const T &scheme) {
    return scheme_t<T>(scheme);
}

template <
    class T
    >
struct host_t {
    host_t(const T &host)
        : host(host) {

    }

    const T &host;
};

template <
    class T
    >
inline
host_t<T> host(const T &host) {
    return host_t<T>(host);
}

template <
    class T
    >
struct path_t {
    path_t(const T &path)
        : path(path) {

    }

    const T &path;
};

template <
    class T
    >
inline
path_t<T> path(const T &path) {
    return path_t<T>(path);
}
} // namespace builder

template <
	class Tag
	>
class basic_builder {

public:

public:

	basic_builder(basic_uri<Tag> &uri)
		: uri_(uri) {

	}

	const basic_uri<Tag> &uri() const {
		return uri_;
	}

private:
	basic_uri<Tag> &uri_;
};

template <
    class Tag
    >
inline
basic_builder<Tag> operator << (basic_uri<Tag> &uri, const builder::scheme_t<typename string<Tag>::type> &scheme) {
    // uri.set_scheme(
    return basic_builder<Tag>(uri);
}

template <
    class Tag
    >
inline
basic_builder<Tag> &operator << (basic_builder<Tag> &builder, const builder::scheme_t<typename string<Tag>::type> &scheme) {
    // uri.set_scheme(
    return builder;
}

template <
    class Tag
    >
inline
basic_builder<Tag> &operator << (basic_builder<Tag> &builder, const builder::host_t<typename string<Tag>::type> &host) {
    // uri.set_host(
    return builder;
}

template <
    class Tag
    >
inline
basic_builder<Tag> &operator << (basic_builder<Tag> &builder, const builder::path_t<typename string<Tag>::type> &path) {
    // uri.set_path(
    return builder;
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_BUILDER_INC__

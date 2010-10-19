//          Copyright Dean Michael Berris 2008, 2009.
//                    Glyn Matthews 2009
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NETWORK_TAG_INCLUDED_20100808
#define BOOST_NETWORK_TAG_INCLUDED_20100808

#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost { namespace network { namespace tags {

    struct pod {};
    struct async {};
    struct tcp {};
    struct udp {};
    struct sync {};
    struct default_string {};
    struct default_wstring {};


    template <class Tag>
    struct components;

    // Tag Definition Macro Helper
#ifndef BOOST_NETWORK_DEFINE_TAG
#define BOOST_NETWORK_DEFINE_TAG(name)                                      \
    typedef mpl::inherit_linearly<                                          \
                name##_tags,                                                \
                mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>  \
            >::type name;                                                   \
    template <> struct components<name> {             \
        typedef name##_tags type;                                           \
    };
#endif // BOOST_NETWORK_DEFINE_TAG

    typedef default_string default_;

} // namespace tags

} // namespace network

} // namespace boost

#endif // __BOOST_NETWORK_TAGS_INC__

//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//[ boost_network_uri
namespace boost { namespace network {
template <
    class Tag
    >
class basic_uri {
public:
    /*<< Defines the underlying string type. >>*/
    typedef typename string<Tag>::type string_type;

    /*<< Constructor. >>*/
    basic_uri(const string_type &uri);
    /*<< Copy constructor. >>*/
    basic_uri(const basic_uri &);
    /*<< Assignment operator. >>*/
    basic_uri &operator = (const basic_uri &);
    /*<< Swap function. >>*/
    void swap(basic_uri &);
    /*<< Returns the URI scheme. >>*/
    string_type scheme() const;
    /*<< Returns the rest of the URI, apart from the scheme. >>*/
    string_type rest() const;
    /*<< Returns true only if the URI was parsed successfully. >>*/
    bool valid() const;
};

typedef basic_uri<tags::default_> uri;
}}
//]

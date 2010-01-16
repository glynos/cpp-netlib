//            Copyright (c) Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//[ boost_network_message
namespace boost { namespace network {
template <
    class Tag
    >
class basic_message {
public:
    /*<< Defines the underlying string type, so that the message can
         be specialized for different text encodings >>*/
    typedef typename string<Tag>::type string_type;
    /*<< Defines a container for message headers >>*/
    typedef typename headers_container<Tag>::type headers_container_type;

    /*<< Copy constructor >>*/
    basic_message(const basic_message &);
    /*<< Assignment operator >>*/
    basic_message &operator = (const basic_message &);
    /*<< Swap function >>*/
    void swap(basic_message &);

    /*<< Accesses the message headers >>*/
    headers_container_type & headers() const;
    /*<< Accesses the message body >>*/
    string_type & body() const;
    /*<< Accesses the source of the message >>*/
    string_type & source() const;
    /*<< Accesses the destination of the message >>*/
    string_type & destination() const;
};

/*<< Defines a message with default parameters. >>*/
typedef basic_message<tags::default_> message;
}}
//]

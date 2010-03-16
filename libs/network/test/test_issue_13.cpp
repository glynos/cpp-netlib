//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



# define BOOST_TEST_MODULE Client and server includes
# include <boost/test/unit_test.hpp>
# include <boost/network/protocol/client.hpp>
# include <boost/network/protocol/server.hpp>


BOOST_AUTO_TEST_CASE(test1) {
    typedef boost::network::http::basic_client<
    boost::network::tags::http_keepalive_8bit_udp_resolve, 1, 1> http_client;
    http_client client;
}

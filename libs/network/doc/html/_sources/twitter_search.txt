.. _twitter_search:

****************
 Twitter search
****************

The code
========

.. code-block:: c++

   #include <boost/network/protocol/http/client.hpp>
   #include <boost/foreach.hpp>
   #include <iostream>

   int main(int argc, char * argv[]) {
       using namespace boost::network;

       if (argc != 2) {
           std::cout << "Usage: " << argv[0] << " <query>" << std::endl;
           return 1;
       }

       try {
           http::client client;

           uri::uri search_uri("http://search.twitter.com/search.json");

   		std::cout << "Searching Twitter for query: " << argv[1] << std::endl;
           uri::uri search_1;
           search_1 << search_uri << uri::query("q", uri::encoded(argv[1]));
           http::client::request request(search_1);
           http::client::response response = client.get(request);

           std::cout << body(response) << std::endl;
       }
       catch (std::exception &e) {
           std::cerr << e.what() << std::endl;
       }

       return 0;
   }

Diving into the code
====================

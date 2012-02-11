.. _atom_reader:

*************
 Atom reader
*************

The code
========

.. code-block:: c++

   #include "atom.hpp"
   #include <boost/network/protocol/http/client.hpp>
   #include <boost/foreach.hpp>
   #include <iostream>

   int main(int argc, char * argv[]) {
       using namespace boost::network;

       if (argc != 2) {
           std::cout << "Usage: " << argv[0] << " <url>" << std::endl;
           return 1;
       }

       try {
           http::client client;
           http::client::request request(argv[1]);
           request << header("Connection", "close");
           http::client::response response = client.get(request);
           atom::feed feed(response);

           std::cout << "Feed: " << feed.title()
	   	     << " (" << feed.subtitle() << ")" << std::endl;
           BOOST_FOREACH(const atom::entry &entry, feed) {
               std::cout << entry.title()
	       		 << " (" << entry.published() << ")" << std::endl;
           }
       }
       catch (std::exception &e) {
           std::cerr << e.what() << std::endl;
       }

       return 0;
   }

Diving into the code
====================

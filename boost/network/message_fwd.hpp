//            Copyright (c) Glyn Matthews 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __2008817MESSAGE_FWD_INC__
# define __2008817MESSAGE_FWD_INC__


// Includes 
# include "boost/network/message/tags.hpp"


namespace boost {
namespace network {
template <
    class Tag
    >
class basic_message;



typedef basic_message<tags::default_> message;
} // namespace boost
} // namespace network


#endif // __2008817MESSAGE_FWD_INC__

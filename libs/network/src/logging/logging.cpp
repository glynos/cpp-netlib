// Copyright 2011 A. Joel Lamotte <mjklaim@gmail.com>.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef NETWORK_NO_LIB
#undef NETWORK_NO_LIB
#endif

#include <iostream>
#include <memory>
#include <network/logging/logging.hpp>

namespace network { namespace logging { 

namespace handler 
{
  namespace
  {
    void std_log_handler( const log_record& log )
    {
      std::cerr << log.full_message() << std::endl;
    }
  }

  log_record_handler get_std_log_handler() { return &std_log_handler; }
  log_record_handler get_default_log_handler() { return &std_log_handler; }
}


namespace 
{
  // the log handler have to manage itself the thread safety on call
  static auto current_log_record_handler = std::make_shared<log_record_handler>( &handler::std_log_handler );

}

  
void set_log_record_handler( log_record_handler handler )
{
  current_log_record_handler = std::make_shared<log_record_handler>( handler );
}

void log( const log_record& log )
{
  auto log_handler = current_log_record_handler;
  if( log_handler )
  {
	(*log_handler)( log );
  }
}


}}
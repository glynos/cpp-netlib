
#ifdef NETWORK_NO_LIB
#undef NETWORK_NO_LIB
#endif

#include <iostream>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
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
		log_record_handler current_log_record_handler = handler::std_log_handler;
		boost::upgrade_mutex mutex_log_handler; // we still need to not change the log handler concurrently
	}

	
	void set_log_record_handler( log_record_handler handler )
	{
		boost::lock_guard<boost::upgrade_mutex> write_lock( mutex_log_handler );
		current_log_record_handler = handler;
	}

	void log( const log_record& log )
	{
		boost::shared_lock<boost::upgrade_mutex> read_lock( mutex_log_handler );
		if( current_log_record_handler )
		{
			current_log_record_handler( log );
		}
	}


}}
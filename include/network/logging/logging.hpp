// Copyright (c) 2012 A. Joel Lamotte <mjklaim@gmail.com>.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_LOGGING_HPP_20121112
#define NETWORK_LOGGING_HPP_20121112

#include <sstream>
#include <functional>

namespace network { namespace logging { 

class log_record;

//using log_record_handler = std::function< void (const std::string&) >; // use this when VS can compile it...
typedef std::function< void (const log_record&) > log_record_handler;

void set_log_record_handler( log_record_handler handler );
void log( const log_record& message );

namespace handler
{
  log_record_handler get_std_log_handler();
  log_record_handler get_default_log_handler();
}

/** Helper to build a log record as a stream. */
class log_record
{
public:
  log_record()
    : m_filename( UNKNOWN_FILE_NAME )
    , m_line(0)
  {} // = default;

  static const char* UNKNOWN_FILE_NAME;

  // Implicit construction from anything serializable to text.
  template< typename TypeOfSomething >
  log_record( TypeOfSomething&& message ) 
      : m_filename( UNKNOWN_FILE_NAME )
      , m_line(0)
  {
    write( std::forward<TypeOfSomething>(message) );
  }

  // Construction with recording context informations.
  log_record( std::string filename, unsigned long line )
      : m_filename( filename )
      , m_line( line )
  {
  }

  ~log_record()
  { 
    log( *this );
  }
  
  template< typename TypeOfSomething >
  log_record& write( TypeOfSomething&& something )
  {
    m_text_stream << something;
    return *this;
  }
  
  template< typename TypeOfSomething >
  inline log_record& operator<<( TypeOfSomething&& something )
  {
    return write( std::forward<TypeOfSomething>(something) );
  }

  std::string message() const { return m_text_stream.str(); }
  const std::string& filename() const { return m_filename; }
  unsigned long line() const { return m_line; }

private:

  // disable copy
  log_record( const log_record& ); // = delete;
  log_record& operator=( const log_record& ); // = delete;

  std::ostringstream m_text_stream; // stream in which we build the message
  std::string m_filename; // = UNKNOWN_FILE_NAME;
  unsigned long m_line; // = 0;
};

}}


#endif /* end of include guard: NETWORK_LOGGING_HPP_20121112 */

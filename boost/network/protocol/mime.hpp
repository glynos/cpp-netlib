//          Copyright Allister Levi Sanchez 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __NETWORK_PROTOCOL_MIME__
#define __NETWORK_PROTOCOL_MIME__

#include <string>
#include <boost/network/message/tags.hpp>
#include <boost/network/message/traits.hpp>

namespace boost { namespace network { namespace mime {

    template <typename Tag>
    class mime {
        
        friend typename ostringstream<Tag> &
            operator << (typename ostringstream<Tag> & out, const typename mime<Tag> & m);

    public:

        typedef typename headers_container<Tag>::type parameter_map;
        typedef typename string<Tag>::type            string_type;

        std::string type() { return _type; }
        string_type body() { return _body; }
        std::string encoding() { return _encoding; }
        std::string boundary() { return _boundary; }
        std::string disposition() { return _disposition; }
	const parameter_map & type_parameters() { return _type_params; }
	const parameter_map & disposition_parameters() { return _disposition_params; }

        void set_disposition(std::string d) { _disposition = d; }
        void set_disposition_parameter(std::string param, std::string value) { _disposition_params[param] = value; }
        void set_type(std::string t) { _type = t; }
        void set_type_parameter(std::string param, std::string value) { _type_params[param] = value; }
        void set_body(std::string b) { _body = b; }
        void set_encoding(encoding_type e) { _encoding = e; }
        void set_boundary(std::string b) { _boundary = b; }

    private:

        std::string   _disposition;
        parameter_map _disposition_params;
        std::string   _type;
        parameter_map _type_params
        string_type   _body;
        std::string   _boundary;
        std::string   _encoding;
    };


    template <class Tag>
        ostringstream<Tag> & operator << (ostringstream<Tag> & out, const mime & m)
        {
            std::string CRLF = "\r\n";
            out << "--" << m._boundary << CRLF << CRLF;
            if (m._type.size() > 0)
                out << "Content-Type: " << m._type;
            else
                out << "Content-Type: text/plain";
            
            typedef typename headers_container<Tag>::type map_t;
            typedef typename map_t::const_iterator iter_t;

            if (m._type_params.empty())
                out << "; charset=us-ascii" << CRLF;
            else {
                for (iter_t i = m._type_params.begin(); i != m._type_params.end(); ++i)
                {
                    out << "; " << i->first << "=" << i->second;
                }
                out << CRLF;
            }

            if (m._encoding.size() > 0)
                out << "Content-Type-Encoding: " << m._encoding << CRLF;
            else
                out << "Content-Type-Encoding: 7bit";

            if (m._disposition.size() > 0) {
                out << "Content-Disposition: " << m._disposition;

                for (iter_t i = m._disposition_params.begin(); i != m._disposition_params.end(); ++i)
                {
                    out << "; " << i->first << "=" << i->second;
                }
                out << CRLF;
            }
	    out << CRLF;
	    out << m._body;
	    out << CRLF;
	    out << "--" << m._boundary << CRLF << CRLF;
        }
} // namespace mime

} // namespace network

} // namespace boost

#endif // __NETWORK_PROTOCOL_MIME__

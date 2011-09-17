#ifndef __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__


# include <boost/cstdint.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
struct port_directive {

    explicit port_directive(const std::string &port)
        : port(port)
    {}

    void operator () (uri &uri_) const {
        static const char separator[] = {':'};
        uri_.append(boost::begin(separator), boost::end(separator));
        uri_.append(port);
    }

    std::string port;

};


struct port_directive_us {

    explicit port_directive_us(boost::uint16_t port)
        : port(port)
    {}

    void operator () (uri &uri_) const {
        static const char separator[] = {':'};
        uri_.append(boost::begin(separator), boost::end(separator));
        std::string port_ = boost::lexical_cast<std::string>(port);
        uri_.append(port_);
    }

    boost::uint16_t port;

};

inline
port_directive port(const std::string &port)  {
    return port_directive(port);
}

inline
port_directive_us port(boost::uint16_t port) {
    return port_directive_us(port);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__

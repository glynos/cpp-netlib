#ifndef __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
struct user_info_directive {

    explicit user_info_directive(const std::string &user_info)
        : user_info(user_info)
    {}

    void operator () (uri &uri_) const {
        static const char separator[] = {'@'};
        uri_.append(user_info);
        uri_.append(boost::begin(separator), boost::end(separator));
    }

    std::string user_info;

};

inline
user_info_directive user_info(const std::string &user_info)  {
    return user_info_directive(user_info);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__

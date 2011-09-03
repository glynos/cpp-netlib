#ifndef __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class ValueType
    >
struct user_info_directive {

    explicit user_info_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        static const char separator[] = {'@'};
        uri.append(value);
        uri.append(boost::begin(separator), boost::end(separator));
    }

    const ValueType &value;

};

template <
    class T
    >
inline
user_info_directive<T> user_info(const T &value)  {
    return user_info_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_USER_INFO_INC__

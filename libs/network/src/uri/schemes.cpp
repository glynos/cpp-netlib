//              Copyright 2012 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <boost/network/uri/schemes.hpp>


namespace boost {
namespace network {
namespace uri {
boost::unordered_set<std::string> hierarchical_schemes::schemes_;

void hierarchical_schemes::register_(const std::string &scheme) {
    schemes_.insert(scheme);
}

bool hierarchical_schemes::exists(const std::string &scheme) {
    return schemes_.end() != schemes_.find(scheme);
}

boost::unordered_set<std::string> non_hierarchical_schemes::schemes_;

void non_hierarchical_schemes::register_(const std::string &scheme) {
    schemes_.insert(scheme);
}

bool non_hierarchical_schemes::exists(const std::string &scheme) {
    return schemes_.end() != schemes_.find(scheme);
}

namespace {
bool register_hierarchical_schemes() {
    hierarchical_schemes::register_("http");
    hierarchical_schemes::register_("https");
    hierarchical_schemes::register_("shttp");
    hierarchical_schemes::register_("ftp");
    hierarchical_schemes::register_("file");
    hierarchical_schemes::register_("dns");
    hierarchical_schemes::register_("nfs");
    hierarchical_schemes::register_("imap");
    hierarchical_schemes::register_("nntp");
    hierarchical_schemes::register_("pop");
    hierarchical_schemes::register_("rsync");
    hierarchical_schemes::register_("snmp");
    hierarchical_schemes::register_("telnet");
    hierarchical_schemes::register_("svn");
    hierarchical_schemes::register_("svn+ssh");
    hierarchical_schemes::register_("git");
    hierarchical_schemes::register_("git+ssh");
    return true;
}

bool register_non_hierarchical_schemes() {
    non_hierarchical_schemes::register_("mailto");
    non_hierarchical_schemes::register_("news");
    non_hierarchical_schemes::register_("im");
    non_hierarchical_schemes::register_("sip");
    non_hierarchical_schemes::register_("sms");
    non_hierarchical_schemes::register_("xmpp");
    return true;
}


static bool hierarchical = register_hierarchical_schemes();
static bool non_hierarchical = register_non_hierarchical_schemes();
} // namespace
} // namespace uri
} // namespace network
} // namespace boost

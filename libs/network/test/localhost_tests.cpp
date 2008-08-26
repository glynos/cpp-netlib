//
//          Copyright Divye Kapoor 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_TEST_MODULE http 1.0 localhost tests

#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <boost/cast.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>

// Platform Specific Headers
// Includes for MSVC/Windows Combination
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(MIN)
    #define _WIN32_DCOM 
    #include <windows.h>
    #include <shellapi.h> // for ShellExecute
    #include <objbase.h>  // for CoInitialize and CoUninitialize
    
    // MSVC Specific inclusion of libraries
    #pragma comment( lib, "shell32" ) // for ShellExecute
    #pragma comment( lib, "ole32" )   // for CoInitialize, CoUninitialize

    SHELLEXECUTEINFO server_info = {
        sizeof(server_info),            // cbSize
        SEE_MASK_NOCLOSEPROCESS,        // fMask
        NULL,                           // hwnd
        "open",                         // lpVerb
        "python.exe",                   // lpFile
        "cgi_server.py",                // lpParameters
        "libs\\network\\test\\server",  // lpDirectory
        SW_SHOWNOACTIVATE,              // nShow, use SW_HIDE if you don't like popup windows
        0,                              // hInstApp
        NULL,                           // lpIDList
        NULL,                           // lpClass
        NULL,                           // hkeyClass
        0,                              // dwHotKey
        NULL,                           // hIcon
        NULL                            // hProcess
    };

    SHELLEXECUTEINFO server_info_alt = {
       sizeof(server_info_alt),         // cbSize
        SEE_MASK_NOCLOSEPROCESS,        // fMask
        NULL,                           // hwnd
        "open",                         // lpVerb
        "python.exe",                   // lpFile
        "cgi_server.py",                // lpParameters
        "server",                       // lpDirectory
        SW_SHOWNOACTIVATE,              // nShow, use SW_HIDE if you don't like popup windows
        0,                              // hInstApp
        NULL,                           // lpIDList
        NULL,                           // lpClass
        NULL,                           // hkeyClass
        0,                              // dwHotKey
        NULL,                           // hIcon
        NULL                            // hProcess
    };
    
#else 
    #include <unistd.h>     // fork, execlp etc.
    #include <sys/types.h>
    #include <sys/wait.h>   // for waitpid
    #include <sys/stat.h>   // for chmod
    #include <signal.h>     // for kill 

    pid_t server_child = (pid_t) 0; // Process id of the python server
#endif

using std::cout;
using std::endl;

std::string base_url = "http://localhost:8000";
std::string cgi_url = base_url + "/cgi-bin/requestinfo.py";

BOOST_AUTO_TEST_CASE(server_start) {
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        // Executes the Python Server
        // See http://msdn.microsoft.com/en-us/library/bb762153(VS.85).aspx for a description of ShellExecute
        CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        BOOST_REQUIRE_MESSAGE( ShellExecuteEx(&server_info) && ShellExecuteEx(&server_info_alt), "Python server execution failed.\n" );
        CoUninitialize();
    #else 
        // Try general Unix code
        server_child = fork();
        BOOST_REQUIRE_MESSAGE(server_child >= 0, "Failed to fork!");
        if( server_child == 0 ) { // child process
            bool server_script_ok = true;
            if ( access("server/cgi_server.py", F_OK)==0 ) {
                // if executed from $CPP_NETLIB_HOME/libs/network/test
                chdir("server");
            }
            else if ( access("libs/network/test/server/cgi_server.py", F_OK)==0 ) {
                // if executed from $CPP_NETLIB_HOME
                chdir("libs/network/test/server");
            }
            else { server_script_ok = false; }
            BOOST_REQUIRE_MESSAGE(server_script_ok, "Cannot find python server script!");

            // set the CGI script execute permission
            bool handler_script_ok = true;
            if ( access("cgi-bin/requestinfo.py", R_OK|X_OK) != 0 )
                handler_script_ok = chmod("cgi-bin/requestinfo.py", S_IWUSR|S_IXUSR|S_IXGRP|S_IXOTH|S_IRUSR|S_IRGRP|S_IROTH) == 0;
            BOOST_REQUIRE_MESSAGE(handler_script_ok, "Cannot execute request handler script!");

            BOOST_REQUIRE_MESSAGE(execlp("python", "python", "cgi_server.py", (char *)NULL) != -1, 
                    "Python server execution failed!\n");
        } else { // parent
            sleep(1);
        }
    #endif 
}

BOOST_AUTO_TEST_CASE(body_test) {
    // Tests presence of body in  http responses
    using namespace boost::network;
    http::request request_(base_url);
    http::client client_;
    http::response response_ = client_.get(request_);
    BOOST_CHECK(body(response_).length() != 0);
}

BOOST_AUTO_TEST_CASE(text_content_type_test) {
    // Tests correct parsing of the content-type header sent by the server
    using namespace boost::network;
    http::request request_(base_url);
    http::client client_;
    http::response response_ = client_.get(request_);
    BOOST_REQUIRE(headers(response_).count("Content-type") != 0);
    headers_range<http::response>::type range = headers(response_)["Content-type"];
    BOOST_CHECK(begin(range)->first == "Content-type");
    BOOST_CHECK(begin(range)->second == "text/html");
}

BOOST_AUTO_TEST_CASE(binary_content_type_test) {
    // Tests correct parsing of content-type for binary files such as .zip files
    using namespace boost::network;
    http::request request_(base_url + "/boost.jpg");
    http::client client_;
    http::response response_ = client_.get(request_);
    BOOST_REQUIRE(headers(response_).count("Content-type") != 0);
    headers_range<http::response>::type range = headers(response_)["Content-type"];
    BOOST_CHECK(begin(range)->first == "Content-type");
    BOOST_CHECK(begin(range)->second == "image/jpeg");
}

BOOST_AUTO_TEST_CASE(content_length_header_test) {
    // Uses the test.xml file to ensure that the file was received at the correct length for a text encoding
    using namespace boost::network;
    http::request request_(base_url + "/test.xml");
    http::client client_;
    http::response response_ = client_.get(request_);
    BOOST_REQUIRE(headers(response_).count("Content-Length") != 0);
    headers_range<http::response>::type range = headers(response_)["Content-Length"];
    BOOST_CHECK_EQUAL(begin(range)->first, "Content-Length");
    BOOST_CHECK_EQUAL(begin(range)->second, "117");
	BOOST_CHECK(body(response_).length() != 0);
}

BOOST_AUTO_TEST_CASE(text_file_query) {
    // Tests proper transfer of a text file
    using namespace boost::network;
    http::request request_(base_url + "/test.xml");
    http::client client_;
    http::response response_ = client_.get(request_);
    
    BOOST_CHECK(body(response_).length() != 0);

    char *memblock = 0;
    try {
        using std::ios;

        std::ifstream file("libs/network/test/server/test.xml", ios::in | ios::binary | ios::ate);
        if( ! file ) {
            file.clear();
            file.open("server/test.xml", ios::in | ios::binary | ios::ate);
        }
        
        BOOST_REQUIRE_MESSAGE( file, "Could not open local test.xml");

        size_t size = file.tellg();
        BOOST_CHECK(size != 0);
        BOOST_CHECK_EQUAL(body(response_).length(), size);

        memblock = new char [size+1];

        file.seekg(0, ios::beg);
        file.read(memblock, size);
        memblock[size] = '\0';
        
        std::pair<char *,std::string::iterator> diff_pos = std::mismatch(memblock, memblock+size,body(response_).begin());
        BOOST_CHECK_EQUAL(diff_pos.first - memblock, boost::numeric_cast<int>(size));
        
        delete[] memblock;
    } catch(...) { 
        delete[] memblock; 
        memblock = 0;
    }

}

BOOST_AUTO_TEST_CASE(binary_file_query) {
    // Tests proper transfer of a binary image
    using namespace boost::network;
    http::request request_(base_url + "/boost.jpg");
    http::client client_;
    http::response response_;
    BOOST_CHECK_NO_THROW(response_ = client_.get(request_));
    
    BOOST_CHECK(body(response_).length() != 0);

    char *memblock = 0;
    try {
        using std::ios;
        
        std::ifstream file("libs/network/test/server/boost.jpg", ios::in | ios::binary | ios::ate);
        if( ! file ) {
            file.clear();
            file.open("server/boost.jpg", ios::in | ios::binary | ios::ate);
        }
        
        BOOST_REQUIRE_MESSAGE( file, "Could not open boost.jpg locally");

        size_t size = file.tellg();
        BOOST_CHECK(size != 0);
        BOOST_CHECK_EQUAL(body(response_).length(), size);

        memblock = new char [size+1];

        file.seekg(0, ios::beg);
        file.read(memblock, size);
        memblock[size] = '\0';
        
        std::pair<char *,std::string::iterator> diff_pos = std::mismatch(memblock, memblock+size,body(response_).begin());
        BOOST_CHECK_EQUAL(diff_pos.first - memblock, boost::numeric_cast<int>(size));
        
        file.close();
        delete[] memblock;
    } catch(...) { 
        delete[] memblock; 
        memblock = 0;
    }
}

BOOST_AUTO_TEST_CASE(cgi_query) {
    // Get a dynamic request with no Content-Length header
    // Ensure that we have a body
    using namespace boost::network;

    http::request req(cgi_url + "?query=1");
    http::client c;
    http::response r;
    BOOST_REQUIRE_NO_THROW(r = c.get(req));
    BOOST_CHECK(body(r).length() != 0);
    BOOST_CHECK(headers(r)["Content-Type"].begin() != headers(r)["Content-Type"].end());
}

BOOST_AUTO_TEST_CASE(cgi_multi_line_headers) {
    using namespace boost::network;

    http::request req(base_url + "/cgi-bin/multiline-header.py?query=1");
    http::client c;
    http::response r;
    BOOST_REQUIRE_NO_THROW(r = c.get(req));
    BOOST_CHECK(body(r).length() != 0);
    BOOST_CHECK(headers(r)["Content-Type"].begin() != headers(r)["Content-Type"].end());
    headers_range<http::response>::type range=headers(r)["X-CppNetlib-Test"];
    BOOST_REQUIRE(begin(range) != end(range));
    BOOST_REQUIRE(distance(range) == 2);
    BOOST_CHECK_EQUAL(begin(range)->second, std::string("multi-line-header"));
    BOOST_CHECK_EQUAL((++begin(range))->second, std::string("that-should-concatenate"));
}

BOOST_AUTO_TEST_CASE(file_not_found) {
    // Request for a non existing file.
    // Ensure that we have a body even in the presence of an error response
    using namespace boost::network;

    http::request req(base_url + "/file_not_found");
    http::client c;
    http::response r = c.get(req);

    BOOST_CHECK(body(r).length() != 0);
}

BOOST_AUTO_TEST_CASE(head_test) {
	using namespace boost::network;
    http::request request_(base_url + "/test.xml");
    http::client client_;
    http::response response_ = client_.head(request_);
    BOOST_REQUIRE(headers(response_).count("Content-Length") != 0);
    headers_range<http::response>::type range = headers(response_)["Content-Length"];
    BOOST_CHECK_EQUAL(begin(range)->first, "Content-Length");
    BOOST_CHECK_EQUAL(begin(range)->second, "117");
	BOOST_CHECK(body(response_).length() == 0);
}

BOOST_AUTO_TEST_CASE(server_kill) {
    #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        UINT exitcode = 0;
        BOOST_REQUIRE_MESSAGE( TerminateProcess(server_info.hProcess, exitcode) || TerminateProcess(server_info_alt.hProcess, exitcode),
                               "Failed to shutdown local server!\n" );
        CloseHandle(server_info.hProcess);
        CloseHandle(server_info_alt.hProcess);
    #else
        BOOST_REQUIRE_MESSAGE( kill(server_child, SIGTERM)==0 , "Failed to shutdown local server!\n"); 
    #endif 
}

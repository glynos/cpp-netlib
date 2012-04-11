//          Copyright Fredrik Olofsson 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef __CPPNETLIB_NETWORK_URI_UTILITY__
#define __CPPNETLIB_NETWORK_URI_UTILITY__

#include <string>

namespace boost
{
	namespace network
	{
		namespace uri
		{
			/**
			* Normalize the given path and returns it. The path will be resolved and any attempt
			* to point to an file above (www) root will be removed. The returned string will allways
			* start with an slash ("/"). The implemenation in its current form does not handle procent
			* coding (%20). Examples:
			*
			* "/test/test/../"	: -> "/test"
			* "../../../"		: -> "/"
			*/
			std::string normalize(std::string);
		}
	}
}



namespace boost
{
	namespace network
	{
		namespace uri
		{
			std::string normalize(std::string path)
			{
				if(path.empty() || path.front() != '/')
						path.insert(0, "/");

				std::size_t query_offset = path.find("?"),
							temp = 0,
							pos = 0,
							counter = 0;

				while((pos = path.find("//")) != std::string::npos && pos < query_offset)
					path.replace(pos,2,"/");

					
				while((pos = path.find("../")) != std::string::npos && pos < query_offset)
				{
					temp = pos;
					while(0 < temp && counter < 2)
					{
						if(path[--temp] == '/')
							++counter;
					}
					path.replace(temp, pos-temp+3,"/"); //length of ../ is 3
					counter = 0;//back to default
				}

				//remove the last '/' in the path section if not the last on.
				pos = ((query_offset == std::string::npos) ? path.size() : query_offset) - 1;
				if(path[pos] == '/' && pos != 0)
					path.erase(pos,1);

				return path;
			}
		}
	}
}


#endif 
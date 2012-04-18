//          Copyright Fredrik Olofsson 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
#ifndef __CPPNETLIB_NETWORK_URI_NORMALIZE__
#define __CPPNETLIB_NETWORK_URI_NORMALIZE__

#pragma once

#include <string>

namespace boost
{
	namespace network
	{
		namespace uri
		{
			/**
			* Normalize a given path and returns it. The path will be resolved and any attempt to
			* point to a file above (www) root will be removed. The returned string will allways
			* start with a slash ("/"). The path must be an instance of std::basic_string<>. The
			* function does handle wide characters as well, so both std::string and std::wstring
			* are accepted. Examples:
			*
			* "/test/test/../"	: -> "/test"
			* "../../../"		: -> "/"
			*/
			template <typename Type>
			inline std::basic_string<Type> normalize(std::basic_string<Type> path)
			{
				//why pass by value? See http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
				
				typedef std::basic_string<Type> StringType;

				//so the method can handle wide characters as well, otherwise must we use L"" or "" depending on char/wchar_t.
				static StringType slash1(1, '/');
				static StringType slash2(2, '/');
				static StringType dot2(2, '.');
				static StringType dot2_slash1(dot2 + slash1);
				
				if(path.empty() || path.front() != '/')
						path.insert(0, slash1);

				std::size_t //query_offset = path.find(question),
							temp = 0,
							pos = 0,
							counter = 0;

				while((pos = path.find(slash2)) != StringType::npos) // && pos < query_offset)
					path.replace(pos,2,slash1);

					
				while((pos = path.find(dot2_slash1)) != StringType::npos)// && pos < query_offset)
				{
					temp = pos;
					while(0 < temp && counter < 2)
					{
						if(path[--temp] == '/')
							++counter;
					}
					path.replace(temp, pos-temp+3,slash1);
					counter = 0;
				}

				pos = /*((query_offset == StringType::npos) ? */ path.size() /*: query_offset)*/ - 1;
				if(path[pos] == '/' && pos != 0)
					path.erase(pos,1);

				return path;
			}

			//A simple helper method, deduce const wchar_t/char * to std::basic_string<char/wchar_t> 
			template <typename Type>
			inline std::basic_string<Type> normalize(const Type* path)
			{
				return normalize(std::basic_string<Type>(path));
			}
		}
	}
}

#endif 
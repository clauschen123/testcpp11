#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <hash_map>
#include <unordered_map>
#include <functional>

typedef unsigned __int64 uint64_t;

namespace z1
{ //{{{

	template <class T>
	inline std::size_t hash_combine(std::size_t& seed, const T & v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}

	struct equal_to
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) == 0;
		}
	};

	struct hash_string
		: public std::unary_function<std::string, std::size_t>
	{
		enum { bucket_size = 4 };
		std::size_t
		operator()(const std::string& __s) const
		{ 
			std::size_t seed = 0;
			return hash_combine( seed, __s);
			//return std::_Fnv_hash<>::hash(__s.data(), __s.length()); 
		}
	};


	struct hash_charptr
		: public std::unary_function<const char*, std::size_t>
	{
		std::size_t
		operator()(const char* __s) const
		{
			std::size_t seed = 0;
			return hash_combine(seed, __s);
			//std::size_t seed = 0;
			//hash_combine(seed, boost::hash_value(p.name));
			//boost::hash_combine(seed, boost::hash_value(p.age));
			//return std::tr1::Fnv_hash<>::hash(__s, strlen(__s));
		}
	};
}//}}}

typedef std::list<std::string> string_list;
typedef std::map<std::string, int> string_map;
//typedef std::hash_map<std::string, int, z1::hash_string> string_hash_map;
typedef std::hash_map<std::string, int> string_hash_map;
typedef std::unordered_map<std::string, int> string_unordered_map;

void fill_list(string_list& slist, size_t count);
uint64_t current_usec();

void map_main()
{
	size_t count = 10000;
	bool rehash = false;

    int argc = 3;
	if (argc == 3)
	{
		rehash = true;
	}

	string_map smap;
	string_hash_map shash_map;
	string_unordered_map sunordered_map;

	if (rehash)
	{
		sunordered_map.rehash(count);
	}

	string_list slist;
	fill_list(slist, count);

	uint64_t start = 0;
	uint64_t end = 0;

	uint64_t map_insert_us = 0;
	uint64_t hash_map_insert_us = 0;
	uint64_t unordered_map_insert_us = 0;

	uint64_t map_traverse_us = 0;
	uint64_t hash_map_traverse_us = 0;
	uint64_t unordered_map_traverse_us = 0;

	uint64_t map_find_us = 0;
	uint64_t hash_map_find_us = 0;
	uint64_t unordered_map_find_us = 0;

	uint64_t map_delete_us = 0;
	uint64_t hash_map_delete_us = 0;
	uint64_t unordered_map_delete_us = 0;

	// Insert test
	{//{{{
		string_list::iterator it(slist.begin());
		string_list::iterator ite(slist.end());

		//map insert
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			smap[*it] = i;
		}
		end = current_usec();
		map_insert_us = end - start;

		//hash_map insert
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map[*it] = i;
		}
		end = current_usec();
		hash_map_insert_us = end - start;

		//unordered_map insert
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map[*it] = i;
		}
		end = current_usec();
		unordered_map_insert_us = end - start;
	}//}}}

	// Traverse test
	{//{{{
		//map traverse 
		{
			string_map::iterator it(smap.begin());
			string_map::iterator ite(smap.end());
			start = current_usec();
			for (int i = 0; it != ite; ++it)
			{
				i++;
			}
			end = current_usec();
			map_traverse_us = end - start;
		}

		//hash_map traverse 
		{
			string_hash_map::iterator it(shash_map.begin());
			string_hash_map::iterator ite(shash_map.end());
			start = current_usec();
			for (int i = 0; it != ite; ++it)
			{
				i++;
			}
			end = current_usec();
			hash_map_traverse_us = end - start;
		}

		//unordered_map traverse 
		{
			string_unordered_map::iterator it(sunordered_map.begin());
			string_unordered_map::iterator ite(sunordered_map.end());
			start = current_usec();
			for (int i = 0; it != ite; ++it)
			{
				i++;
			}
			end = current_usec();
			unordered_map_traverse_us = end - start;
		}
	}//}}}

	// Find test
	{//{{{
		string_list::iterator it(slist.begin());
		string_list::iterator ite(slist.end());

		//map find
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			smap[*it] = i;
		}
		end = current_usec();
		map_find_us = end - start;

		//hash_map find
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map[*it] = i;
		}
		end = current_usec();
		hash_map_find_us = end - start;

		//unordered_map find
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map[*it] = i;
		}
		end = current_usec();
		unordered_map_find_us = end - start;
	}//}}}

	// Delete test
	{//{{{
		string_list::iterator it(slist.begin());
		string_list::iterator ite(slist.end());

		//map delete
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			smap.erase(*it);
		}
		end = current_usec();
		map_delete_us = end - start;

		//hash_map delete
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map.erase(*it);
		}
		end = current_usec();
		hash_map_delete_us = end - start;

		//unordered_map delete
		it = slist.begin();
		start = current_usec();
		for (int i = 0; it != ite; ++it, ++i)
		{
			shash_map.erase(*it);
		}
		end = current_usec();
		unordered_map_delete_us = end - start;
	}//}}}

	//stat output
	std::cout << " insert, count " << count << std::endl;
	std::cout << " std::map " << map_insert_us << " us\n";
	std::cout << " std::ext/hash_map " << hash_map_insert_us << " us\n";
	std::cout << "std::unordered_map " << unordered_map_insert_us << " us\n";

	std::cout << "\n";
	std::cout << " traverse, count " << count << std::endl;
	std::cout << " std::map " << map_traverse_us << " us\n";
	std::cout << " std::ext/hash_map " << hash_map_traverse_us << " us\n";
	std::cout << "std::unordered_map " << unordered_map_traverse_us << " us\n";

	std::cout << "\n";
	std::cout << " find, count " << count << std::endl;
	std::cout << " std::map " << map_find_us << " us\n";
	std::cout << " std::ext/hash_map " << hash_map_find_us << " us\n";
	std::cout << "std::unordered_map " << unordered_map_find_us << " us\n";

	std::cout << "\n";
	std::cout << " delete, count " << count << std::endl;
	std::cout << " std::map " << map_delete_us << " us\n";
	std::cout << " std::ext/hash_map " << hash_map_delete_us << " us\n";
	std::cout << "std::unordered_map " << unordered_map_delete_us << " us\n";
}

void fill_list(string_list& slist, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		std::ostringstream oss;
		oss << i;
		//slist.push_back(MD5::getHexMD5(oss.str().c_str(), oss.str().length()));
		slist.push_back(oss.str());
	}
}

#include <time.h>
uint64_t current_usec()
{
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
	return clock();
}
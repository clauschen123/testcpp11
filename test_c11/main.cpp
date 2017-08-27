// test_c11.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <memory>
#include <functional>
#include <iterator>
#include <string>
#include <numeric>
#include <fstream>

using namespace std;
void map_main();
void lambda_main();
void ppl_main();
void rvalue_main();
void stl_main();
void other_main();
void concurrency_main();
void feature_main();
void std11_main();
void asio_main();
void tuple_main();
void reflect_main();
void meta_main();
void leetcode_main();
void morderncpp_main();
void coroutine_main();
void boost_main();
void json_main();

int _tmain(int argc, _TCHAR* argv[])
{
    std::string filename = "test.bin";
    std::fstream s(filename, ios::binary | ios::trunc | ios::in | ios::out);
    if (!s.is_open()) {
        std::cout << "failed to open " << filename << '\n';
    }
    else {
        // write
        double d = 3.14;
//         s.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
        s << /*123 <<*/ "abc";                              // text output

        // for fstream, this moves the file position pointer (both put and get)
        s.seekp(0);

        // read
//         s.read(reinterpret_cast<char*>(&d), sizeof d); // binary input
        int n=1;
        std::string str;
        if (s >> /*n >>*/ str)                             // text input
            std::cout << "read back from file: " << d << ' ' << n << ' ' << str << '\n';
    }

    map <string, int> m;//multimap的创建
    m.insert(pair<string, int>("Jack", 1));//插入
    m.insert(pair<string, int>("Jack", 2));
    m.insert(pair<string, int>("Body", 1));
    m.insert(pair<string, int>("Navy", 4));
    m.insert(pair<string, int>("Demo", 3));

    for (auto& p : m)
    {
        m.insert(pair<string, int>("ccc", 3));
        m.insert(pair<string, int>("aaa", 3));

    }
// 
//     multimap<string, int>::iterator iter;
//     for (iter = m.begin(); iter != m.end(); ++iter)//遍历
//     {
//         cout << (*iter).first << "  " << (*iter).second << endl;
//     }
//     m.erase("Navy");//multimap的删除
//     cout << "The element after delete:" << endl;
//     for (iter = m.begin(); iter != m.end(); ++iter)
//     {
//         cout << (*iter).first << "  " << (*iter).second << endl;
//     }
//     //multimap元素的查找 
//     multimap<string, int>::iterator it;
// //     int num = m.count("Jack");
// //     it = m.find("Jack");
// //     cout << "the search result is :" << endl;
// //     for (int i = 1; i <= num; i++)
// //     {
// //         cout << (*it).first << "  " << (*it).second << endl;
// //         it++;
// //     }
//     auto pos = m.equal_range("Jack");
//     it = pos.first;
//     while (it != pos.second)
//     {
//         cout << (*it).first << "  " << (*it).second << endl;
//         ++it;
//     }

    json_main();

	::system("pause");
	return 0;
}


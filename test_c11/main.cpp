// test_c11.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
// #include "perftimer.h"

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
#include <unordered_map>

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
    asio_main();

	::system("pause");
	return 0;
}


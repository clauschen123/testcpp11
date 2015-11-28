// test_c11.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <memory>
#include <functional>
#include <iterator>
#include <string>
#include <numeric>

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

using namespace std;

template<class T>
void test(T&& v)
{
    v = 100;
}
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;

template<class T>
UINT32 Increase(int addval, int byteoffset)
{
    static UINT32 totalval = 0x11111111;
    UINT8 bitoffset = 8 * byteoffset;
    T subval = T((totalval >> bitoffset) & T(-1)) + addval;
    totalval = (totalval & ~(T(-1) << bitoffset)) | ((UINT32)subval << bitoffset);

    return totalval;
}

///////////////////////////////////////////
enum EFindLowHP  {};
struct demo{
    vector<int> vec=vector<int>(1, 100);

    int val = 0;
    bool isman = true;
    demo()
    {}
    demo(int _val, bool _isman = true) : val(_val), isman(_isman)
    {}
    bool isMan() const { return isman; }
    bool isDog() const { return !isman; }
    int getVal() const { return val; }
    bool operator() (const demo& d, EFindLowHP)
    {
        if (isMan() && !d.isMan())
            return true;
        if (!isMan() && d.isMan())
            return false;
        return getVal() > d.getVal();
    }
};
///////////////////////////////////////////////////////

void printS(string&& str)
{
    string a = std::move(str);
    cout << str;
}
int _tmain(int argc, _TCHAR* argv[])
{
    printS("sldfjlsfd)");
    array<int, 10> arr;
    demo d;
    do 
    {
        {
            int i = 1;
            vector<demo> v;
            v.emplace_back(i++, false);
            v.emplace_back(i++);
            v.emplace_back(i++,false);
            v.emplace_back(i++);
            v.emplace_back(i++, false);
            sort(v.begin(), v.end(), [](demo & left, demo & right) {
                return left(right, EFindLowHP(0));
            });
            for_each(v.begin(), v.end(), [](demo const& e){ cout << e.getVal() << "," << e.isMan() << endl; });

            break;
        }
    {
        vector<int> c1{ 1, 2, 3, 4 };
        vector<int> c2{ 4, 5, 7 };
        set_difference(c2.cbegin(), c2.cend(),
            c1.cbegin(), c1.cend(),
            ostream_iterator<int>(cout, " "));
    }
    {
        vector<int> uidlist_{ 2, 3, 2, 3 };
        int uid = 3;

        uidlist_.erase(remove(uidlist_.begin(), uidlist_.end(), uid), uidlist_.end());
    }

        int zone[] = { 3 };
        int uid[] = { 2532 };
        unsigned long long rid = 0;
        for (int i = 0; i < 1; i++){
            rid = (((unsigned long long)zone[i]) << 52);
            rid += uid[i];
            cout << rid << endl;
        }
    } while (0);

	::system("pause");
	return 0;
}


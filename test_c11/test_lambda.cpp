#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
class CLambda
{
public:
	CLambda() :m_nData(20) { NULL; }

	void TestLambda()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		
		{//无参数
			for_each(v.begin(), v.end(), [](int v){cout << v << endl; });
		}
		{//以值方式传递作用域内可见的局部变量（包括this)
			int a = 10;
			for_each(v.begin(), v.end(), [=](int v){cout << v + a<< endl; });
		}
		{//以引用方式传递作用域内可见的局部变量（包括this)
			int a = 10;
			for_each(v.begin(), v.end(), [&](int v){cout << v + a++ << endl; });
			cout << a << endl;
		}
		{//以值方式传递局部变量a
			int a = 10;
			for_each(v.begin(), v.end(), [a](int v)mutable{cout << v + a << endl; a++;  });
			cout << a << endl;
		}
		{//以引用方式传递局部变量a
			int a = 10;
			for_each(v.begin(), v.end(), [&a](int v){cout << v + a++ << endl; });
			cout << a << endl;
		}
		{//传递this
			for_each(v.begin(), v.end(), [this](int v){cout << v + m_nData << endl; });
		}
		{//b按引用方式传递，其他按值
			int a = 10; int b = 12;
			for_each(v.begin(), v.end(), [=, &b](int v){cout << v + a << endl; b++; });
			cout << b << endl;
		}

		{//操作符重载函数参数按引用传递
			for_each(v.begin(), v.end(), [](int& v){ cout << v++ << endl; });
			for_each(v.begin(), v.end(), [](int v){cout << v << endl; });
		}

		{//empty lambda
			[](){ cout << "empty lambda 1 " << endl;  }();
			[]{cout << "empty lambda 2 " << endl; }();

			cout << []()mutable->int{ return 19; }() << endl;
			cout << []()->int{ return 190; }() << endl;
		}
	}

private:
	int m_nData;
};

void lambda_main()
{
	CLambda lam;
	lam.TestLambda();
}
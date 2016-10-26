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
		
		{//�޲���
			for_each(v.begin(), v.end(), [](int v){cout << v << endl; });
		}
		{//��ֵ��ʽ�����������ڿɼ��ľֲ�����������this)
			int a = 10;
			for_each(v.begin(), v.end(), [=](int v){cout << v + a<< endl; });
		}
		{//�����÷�ʽ�����������ڿɼ��ľֲ�����������this)
			int a = 10;
			for_each(v.begin(), v.end(), [&](int v){cout << v + a++ << endl; });
			cout << a << endl;
		}
		{//��ֵ��ʽ���ݾֲ�����a
			int a = 10;
			for_each(v.begin(), v.end(), [a](int v)mutable{cout << v + a << endl; a++;  });
			cout << a << endl;
		}
		{//�����÷�ʽ���ݾֲ�����a
			int a = 10;
			for_each(v.begin(), v.end(), [&a](int v){cout << v + a++ << endl; });
			cout << a << endl;
		}
		{//����this
			for_each(v.begin(), v.end(), [this](int v){cout << v + m_nData << endl; });
		}
		{//b�����÷�ʽ���ݣ�������ֵ
			int a = 10; int b = 12;
			for_each(v.begin(), v.end(), [=, &b](int v){cout << v + a << endl; b++; });
			cout << b << endl;
		}

		{//���������غ������������ô���
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
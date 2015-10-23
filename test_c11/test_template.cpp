#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <deque>
#include <map>
#include <typeinfo>

using namespace std;
#include "test_template.h"

//<<C++模板简体中文版>>

//page 32 : Nontype Template Parameters
template <class T ,T defT>
class A
{
public:
	A() :t(defT){}
	void print()
	{
		cout << t << endl;
	}
	T t;
};

//page 32 : Nontype Function Template Parameters
template <typename T, int VAL>
T addValue(T const& x)
{
	return x + VAL;
}

//class,float,double,const char* IS NOT ALLOWED as Nontype Template Parameters
//template <float d>
class B
{

};

template<int N>
void printBitset(std::bitset<N> const& bs)
{
	//std::cout << bs.template to_string<char, char_traits<char>, allocator<char> >();
	std::cout << bs.to_string<char, char_traits<char>, allocator<char> >();
}

template <typename T>
class Base {
public:
	void exit(){ cout << "Base::exit" << endl; }
};

template <typename T>
class Derived : public Base<T> {
public:
	void foo() {
		exit();
	}
};

//page41 template template parameter
template <typename T,
		template <typename ELEM,
				  typename  = std::allocator<ELEM > >
		class CONT = std::deque>
class Stack {
private:
	CONT<T> elems;
public:
	void push(T const& t)
	{
		elems.push_back(t);
		cout << "stack::push" << t << endl;
		cout << "size is:" << elems.size()<<endl;
	}
};

//page47 : zero initalized
template <typename T>
void foo()
{
	T x = T(); // 如果 T 是内建类型，则 x 被初始化为 0 或 false
}

template <typename	T> 
class MyClass
{
private:
	T x;
public:
	MyClass() : x() { // 这么做可以确保：即使 T 为内建类型，x 也能被初始化。
	}
};

template <typename T>
inline T const& max1(T const& a, T const& b)
{
	return a ;
}

template <typename T>
inline T const& max2(T a, T b)
{
	return a;
}
template <typename T, int N, int M>
T const* max(T const (&a)[N], T const (&b)[M])
{
	return a < b ? b : a;
}

template <typename T>
void ref(T const& x)
{
	std::cout << "x in ref(T const&): "
		<< typeid(x).name() << std::endl;
}
template <typename T>
void nonref(T x)
{
	std::cout << "x in nonref(T):"
		<< typeid(x).name() << std::endl;
}

// template template parameter
template <template<typename T, T*> class Buf>
class Lexer {
	static char storage[5];
	Buf<char, &Lexer<Buf>::storage[0]> buf;
};

template <template<typename T> class List>
class Node {
	//static T* storage; // ERROR：这里不能使用 template template parameters 的参数 T
};

typedef char RT1;
typedef struct { char a[2]; } RT2;
template<typename T> RT1 test2(typename T::X const*)
{
	cout << "rt1 test" << endl;
	return RT1;
}

template<typename T> RT2 test(...)
{
	RT2 rt = { 0 };
	cout << "rt2 test" << endl;
	return rt;
}

template <typename Func, typename T>
void apply(Func func_ptr, T x)
{
	func_ptr(x);
}

#define type_has_member_type_X(T) \
	(sizeof(test<T>(0)) == 1)

template<int I> void f1(int(&arr)[24 / (4 - I)] )
{
	cout << "func:" <<arr[0]<< endl;
}

//template<int I> void f(int(&)[24 / (4 + I)]);
void test1()
{
	int n[8];
	f1<1>(n);
	&test<int>;

	cout << "sizeof test<T>(0) = " << sizeof(test<int>(0)) << endl;
	cout << type_has_member_type_X(int) << endl;
	////////////////////////////////////////////////////////

	typedef void (CA<int>::*push_ptr)(int n);

	CA<int>  ca;
	cout << std::hex <<&CA<int>::push << endl;
	cout << std::hex <<&CA<int>::push2 << endl;
	printf("push addr: 0x%08lx\n", &CA<int>::push);
	printf("push addr: 0x%08lx\n", &CA<float>::push);
	printf("push addr: 0x%08lx\n", &CA<int>::push3);
	ca.push3(2);

	CB<int>  cb;
	printf("pushB addr: 0x%08lx\n", &CB<int>::push3);

	auto ptr = &CA<int>::push;
	decltype(ptr) vv = ptr;
	std::cout << std::hex << ptr;
	////////////////////////////////////////////

	Derived<int> d1;
	Stack<int, vector > vStack1;
	ref(d1);
	ref(vStack1);

	ref("hi");
	nonref("hi");

	std::string s;
	::max1("apple", "peach");
	// OK：类型相同
	::max2("apple", "tomato");
	// ERROR：类型不同
	//::max("apple", s);

	Stack<int, vector > vStack;
	vStack.push(100);
	vStack.push(200);

	Derived<int> d;
	d.foo();

	std::bitset<12> bs;
	printBitset(bs);

	A<int, 1090> aint;

	aint.print();

	vector<int> source(2, 100);
	vector<int> dest(2, 200);
	std::transform(source.begin(), source.end(),
		// 来源端起止位置
		dest.begin(),
		// 目的端起始位置
		addValue<int, 5>);

}

//page 96
template <typename T, T nontype_param,typename T2, T2 nontype_param2>
//template <typename T, T nontype_param>
class C
{
public:
	void push()
	{
		T t = nontype_param ;
		cout << t << endl;
		printf("0x%08lx\n", t);
		cout << typeid(t).name() << endl;

		int * p;
		cout << typeid(p).name() << endl;
		//t = nontype_param;
		//cout << n_param << endl;
	}
};

void f();
void f(int);

class X 
{
public:
	short n;
	short l;
	static bool b;
	void func(){
	}
};
bool X::b = false;

template<typename T>
void templ_func()
{

}
int a;

class Base1 {
public:
	int i;
} base;
class Derived1 : public Base1 {
} derived_obj;

template <template <typename T,
					typename = std::allocator<T> >
					class CONT = std::vector>

class CC
{};

vector<int> gVec;
//template <typename T, vector<T, std::allocator<T> > v> //非类型模板参数“v”的类型非法
class CC2
{};
//模板参数的各种类型
void test2()
{
	CC<> cc;

#if 0
	C<int, 33>* c1;								// 整数型（integer type）

	C<int*, &a>* c2;							// 外部变量的地址

	C<void(*)(), f>* c31;
	C<void(*)(int), f>* c3;						// 一个函数名称。重载解析规则在此选用 f(int)。& 会被隐寓加入。

	//C<bool, X::b>* c41;						//指向变量的必须是指针或引用
	C<bool&, X::b>* c4;							// static class members 都可接受
#endif

	//C<short X::*, &X::l> c5;					// pointer-to-member 常数亦可接受
	printf("push addr: 0x%08lx\n", &X::n);
	printf("push addr: 0x%08lx\n", &X::l);
	printf("push addr: 0x%08lx\n", &X::func);
	C<short X::*, &X::n, short X::*, &X::l> c5;	// pointer-to-member 常数亦可接受
	c5.push();

#if 0
	C<void(*)(), &templ_func<double> >* c6;		// function template 具现体也是一种函数
#endif

	//C<Base1*, &derived_obj>* err1;			// ERROR：「derived-to-base 转换」不被考虑
	//C<int&, base.i>* err2;					// ERROR：成员变数不被考虑
	int a[10];
	//C<int*, &a[0]>* err3;						// ERROR：不能使用 array 内某个元素的地址
}

namespace args
{
//     template<typename ... Args>
//     int get_count(Args args)
//     {
//         return sizeof...(args);
//     }

    template<typename T>
    void Print(T value)
    {
     cout << value << endl;
    }

    template<typename Head, typename... Rail>
    void Print(Head head, Rail... rail)
    {
     cout << head << ",";
     Print(rail...);
    }

    void main()
    {
     Print(1);                  // 输出：1
     Print(1, "hello");         // 输出：1,Hello
     Print(1, "hello", 'H');    // 输出：1,Hello,H
    }
}

void template_main()
{
    args::main();
}
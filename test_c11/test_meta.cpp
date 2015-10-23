#include <iostream>
#include <type_traits>
using namespace std;

namespace EnableIf{
    template <class T>
    typename std::enable_if<std::is_arithmetic<T>::value, int>::type foo(T &t)
    {
        return static_cast<int>(t);
    }

    template <class T>
    typename std::enable_if<std::is_class<T>::value, T>::type & foo(T &t)
    {
        return t;
    }

    // foo1 overloads are enabled via the return type
    template<class T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
        foo1(T t)
    {
        cout << typeid(is_floating_point<T>::value).name() << endl;
        cout << typeid(enable_if<is_floating_point<T>::value, T>::type).name() << endl;
        std::cout << "foo1: float\n";
        return t;
    }

    template<class T>
    std::enable_if_t<std::is_integral<T>::value, T> //Using helper type
        foo1(T t)
    {
        std::cout << "foo1: int\n";
        return t;
    }

    // foo2 overload is enabled via a parameter
    template<class T>
    T foo2(T t, typename std::enable_if<std::is_integral<T>::value >::type* = 0)
    {
        return t;
    }

    // foo3 overload is enabled via a template parameter
    template<class T,
        typename std::enable_if<std::is_integral<T>::value>::type* = nullptr >
        T foo3(T t) // note, function signature is unmodified
    {
        return t;
    }

    // A is enabled via a template parameter
    template<class T, class Enable = void>
    class A; // undefined

    template<class T>
    class A < T, typename std::enable_if<std::is_floating_point<T>::value >::type > {
    }; // note: for this use case, static_assert may be more appropriate

    void main(){
        EnableIf::foo1(1.2); // OK, calls the first version of foo1()
        EnableIf::foo1(10); // OK, calls the second version of foo1()

        //  foo2(0.1); // compile-time error
        EnableIf::foo2(7); // OK

        //  foo3(1.2); // compile-time error
        //  foo3(34); // OK

        //  A<int> a1; // compile-time error
        EnableIf::A<double> a1; // OK
    }
}

namespace SFINAE{
    ///////////////////////////////////////////////////////////////////////////////
    // http://www.cnblogs.com/xusd-null/p/3761239.html
    // 比如我们想实现一个通用的函数叫AnyToString, 他可以实现任意类型的数据转成字符串:
    // 我们更希望这个函数能检查ValueType类型自己有没有ToString方法, 如果有就直接调用, 
    // 没有的话就采取通用的处理方案.但是C++没有反射机制, 不能像C#那样通过TypeInfo来检
    // 查, 更没有像Java那样纯粹的OOP，从最基类就定义了ToString方法，下面的子类只用负责重载
    // 所以我们希望能有一种方法能让C++也能检查某个类型是否定义了某个成员函数, 这就可以用到SFINAE
    //////////////////////////////////////////////////////////////////////////////
    struct A { char* ToString() const { return "A tostring"; }
    };
    struct B {
    };
    struct C { static char* ToString() { return ""; }
    };

    template < typename ClassType>
    struct HasToStringFunction {
        typedef struct { char a[2] ; } Yes;
        typedef struct { char a[1]; } No;

        template < typename FooType, char* (FooType::* )() const>
        struct FuncMatcher;

        template < typename FooType, char* (FooType::* const)() >
        struct FuncMatcher1;

        template < typename FooType>
        static Yes Tester(FuncMatcher1<FooType, FooType::ToString>*);

        template < typename FooType>
        static Yes Tester(FuncMatcher<FooType, &FooType::ToString>*);

        template < typename FooType>
        static No Tester(...);

        enum { Result = sizeof(Tester<ClassType>(NULL)) == sizeof(Yes) };
    };
    bool a_has_tostring = HasToStringFunction<A>::Result;   // True
    bool b_has_tostring = HasToStringFunction<B>::Result;   // False
    bool c_has_tostring = HasToStringFunction<C>::Result;   // False

    template<bool>
    struct AnyToStringAdviser;

    template<>
    struct AnyToStringAdviser < true > 
    {
        template<typename ValueType>
        static char* ToString(const ValueType& value) {
//             return "lsjdfl";
             return value.ToString();
        }
    };

    template<>
    struct AnyToStringAdviser < false >
    {
        template<typename ValueType>
        static char* ToString(const ValueType& value) {
            /* Generic process */
            return "";
        }
    };

    template<typename ValueType>
    char* AnyToString(const ValueType& value)
    {
        bool result = HasToStringFunction<ValueType>::Result;
        return AnyToStringAdviser<HasToStringFunction<ValueType>::Result >::ToString(value);
    }

    // 再写一个常用的使用了该方法的traits工具类
//     template <typename T>
//     struct is_class
//     {
//         typedef char _one;
//         typedef struct{ char a[2]; } __two__;
// 
//         template <typename U>
//         static _one test(int U::*){ }
// 
//         template <typename U>
//         static __two__ test(...){ }
// 
//         const static bool value = (sizeof(test<T>(NULL)) == sizeof(_one));
//     };

    void main()
    {
        cout << "   a has ToString " << a_has_tostring << endl;
        cout << "   b has ToString " << b_has_tostring << endl;
        cout << "   c has ToString " << c_has_tostring << endl;

        A a;
        cout << "  anything to string: " << AnyToString(a) << endl;
    }

#if 0
    // http://www.cnblogs.com/mightofcode/archive/2013/04/07/3005849.html
    // 使用SFINAE判断模板参数是否是class(在很多场合这个trick非常有用):
    template<typename T>
    class isClassA
    {
        typedef char _One;
        typedef struct{ char a[2]; }_Two;
        template<typename T>
        static _One isClass(int T::* p);
        template<typename T>
        static _Two isClass(...);
    public:
        static const int value = sizeof(isClass<T>(NULL)) == sizeof(_One);
    };
    // 使用SFINAE判断类是否有某个函数:
    template<typename T>
    class HasFuncFoo
    {
        typedef char _One;
        typedef struct{ char a[2]; }_Two;
        template<typename T, void (T::*)()>
        struct FuncMatcher;
        template<typename T>
        static _One hasFunc(FuncMatcher<T, &T::fooFunc>*);
        template<typename T>
        static _Two hasFunc(...);
    public:
        static const int value = sizeof(hasFunc<T>(NULL)) == sizeof(_One);
    };
//     如何根据自己的需要写SFINAE ?
//         基本思路是想办法把要判断的东西放到函数参数中, 比如上面的FuncMathcer就是很好的例子 : 把fooFunc作为一个模板类的模板参数, 然后以这个类作为SFINAE函数的参数
//         SFINAE应用
//         1 tr1的enable_if用了很多SFIANE, 遇到类似的场景可以考虑使用SFIANE
//         2 其他应用有待研究
//         ps 看到一个很有意思的c++wikibook:http ://en.wikibooks.org/wiki/More_C%2B%2B_Idioms
#endif
}

void test(...)
{
    cout << "sdflkjowijio" << endl;
}

void meta_main()
{
    test(100, 300);
    SFINAE::main();
}
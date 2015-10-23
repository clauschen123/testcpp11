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
    // ����������ʵ��һ��ͨ�õĺ�����AnyToString, ������ʵ���������͵�����ת���ַ���:
    // ���Ǹ�ϣ����������ܼ��ValueType�����Լ���û��ToString����, ����о�ֱ�ӵ���, 
    // û�еĻ��Ͳ�ȡͨ�õĴ�����.����C++û�з������, ������C#����ͨ��TypeInfo����
    // ��, ��û����Java���������OOP���������Ͷ�����ToString���������������ֻ�ø�������
    // ��������ϣ������һ�ַ�������C++Ҳ�ܼ��ĳ�������Ƿ�����ĳ����Ա����, ��Ϳ����õ�SFINAE
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

    // ��дһ�����õ�ʹ���˸÷�����traits������
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
    // ʹ��SFINAE�ж�ģ������Ƿ���class(�ںܶೡ�����trick�ǳ�����):
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
    // ʹ��SFINAE�ж����Ƿ���ĳ������:
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
//     ��θ����Լ�����ҪдSFINAE ?
//         ����˼·����취��Ҫ�жϵĶ����ŵ�����������, ���������FuncMathcer���Ǻܺõ����� : ��fooFunc��Ϊһ��ģ�����ģ�����, Ȼ�����������ΪSFINAE�����Ĳ���
//         SFINAEӦ��
//         1 tr1��enable_if���˺ܶ�SFIANE, �������Ƶĳ������Կ���ʹ��SFIANE
//         2 ����Ӧ���д��о�
//         ps ����һ��������˼��c++wikibook:http ://en.wikibooks.org/wiki/More_C%2B%2B_Idioms
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
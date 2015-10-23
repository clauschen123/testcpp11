#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <boost/typeof/typeof.hpp>
#include <boost/progress.hpp>
using namespace std;
typedef unsigned long long UINT64;
typedef unsigned int UINT32;

namespace rvalue
{
    using namespace std;
    /******************************************
    作为最重要的一项语言特性，右值引用(rvalue references)
    被引入到 C++0x中。我们可以通过操作符“&&”来声明一个右值
    引用，原先在C++中使用“&”操作符声明的引用现在被称为左值
    引用。

    左值引用和右值引用的表现行为基本一致，它们唯一的差别就是右
    值引用可以绑定到一个临时对象(右值)上，而左值引用不可以。

    右值是无名的数据，例如函数的返回值一般说来就是右值。当对右
    值进行操作的时候，右值本身往往没有必要保留，因此在某些情况
    下可以直接“移动”之。通过右值引用，程序可以明确的区分出传
    入的参数是否为右值，从而避免了不必要的拷贝，程序的效率也就
    得到了提高。我们考虑一个简单的数据交换的小程序，从中来体会
    右值引用所带来的效率提升
    *******************************************/
    template <class T>
    T&& move(T&& a)
    {
        return a;
    }
    /********************************************
    在这段重新实现的代码中，我们使用了一个move()函数来代替
    对象的赋值操作符“ = ”, move()只是简单地接受一个右值
    引用或者左值引用作为参数，然后直接返回相应对象的右值引
    用。这一过程不会产生拷贝(Copy)操作，而只会将源对象移
    动(Move)到目标对象。

    正是拷贝(Copy)和移动(Move)的差别，使得右值引用成为
    C++0x中最激动人心的新特性之一。从实践角度讲，它能够完
    美是解决C++中长久以来为人所诟病的临时对象的效率问题。
    从语言本身讲，它健全了C++中的引用类型在左值右值方面
    的缺陷。从库设计者的角度讲，它给库设计者又带来了一把
    利器。而对于广大的库使用者而言，不动一兵一卒便能够获
    得“免费的”效率提升
    *********************************************/
    template <class T>
    void _swap(T& a, T& b)
    {
        T tmp(std::move(a)); // 对象a被移动到对象tmp，a被清空 ??
        a = std::move(b);    // 对象b被移动到对象a，b被清空
        b = std::move(tmp);  // 对象tmp被移动到对象b
    }

    class Demo;
    struct DemoMem
    {
        Demo& demo;
        DemoMem(Demo& d) : demo(d)
        {
        }

        DemoMem& operator=(const DemoMem& mem)
        {
            //why go here?
            cout << "DemoMem Copy assign";
            return *this;
        }
    };

    class Demo
    {
    public:
        Demo(int n) : count_(n), mem_(*this)
        {
            ptr_ = new int(n);
            cout << "Demo:" << count_ << endl;
        };

        ~Demo()
        {
            cout << "~Demo:" << count_ << endl;
        };

        Demo(const Demo& l,int a) : mem_(*this)
        {
            //why go here?
            cout << "Demo Copy constructor :" << l.count_ << endl;
            count_ = l.count_;
            mem_ = l.mem_;
        }

        Demo& operator=(const Demo& l)
        {
            //why go here?
            cout << "Demo Copy assign:" << l.count_ << endl;
            count_ = l.count_;
            return *this;
        }
        Demo& operator =(Demo&& l)
        {
            count_ = l.count_;
            ptr_ = l.ptr_;
            l.ptr_ = 0;
            cout << "Demo Copy move:" << l.count_ << endl;
            return *this;
        }
    private:
        int count_;
        int* ptr_;
        DemoMem mem_;
    };

    Demo readFile()
    {
        //编译器优化掉了两次拷贝
        Demo retv(100);
        // fill retv
        return retv;
    }
    //////////////////////////////////////////
    /// std::foward
    //////////////////////////////////////////
    void overloaded(int const &arg) { std::cout << "  by lvalue\n"; }
    void overloaded(int && arg) { std::cout << "  by rvalue\n"; }

    template< typename t >
    /* "t &&" with "t" being template param is special, and  adjusts "t" to be
    (for example) "int &" or non-ref "int" so std::forward knows what to do. */
    void forwarding(t && arg) {

        std::cout << "via std::forward: ";
        overloaded(std::forward< t >(arg));

        std::cout << "via std::move: ";
        overloaded(std::move(arg)); // conceptually this would invalidate arg

        std::cout << "by simple passing: ";
        overloaded(arg);
    }

    void foward_main()
    {
        forwarding(5);

        int x = 5;
        forwarding(x);
    }


    void move_main()
    {
        Demo a1(1);
        Demo b1(a1);

        //int& a_lvref = int();      // error C2440: 'initializing' : cannot convert from 'int' to 'int &'    
        int&& b_rvref = int();  // OK!
        b_rvref++;
        cout << b_rvref << endl;
        {
            Demo a(1), b(2), c(3);
            //Demo d = readFile();
            a = std::move(b);
            c = a;
        }
        {
            string a("1111"), b("22222"), c("333333");
            //Demo d = readFile();
            a = std::move(b);
            c = a;
        }
        int a = 1;
        int b = 2;

        _swap(a, b);
        cout << a << "  " << b << endl;
    }

    ////////////////////////////////////////////////////////////////
    ///  http://darkc.at/cxx-perfect-forwarding/
    ////////////////////////////////////////////////////////////////
    namespace mutouyun
    {
        class Foo {};

        void processValue(Foo&)         { std::cout << "  左值" << std::endl; }
        void processValue(const Foo&)   { std::cout << "  常左值" << std::endl; }
        void processValue(Foo&&)        { std::cout << "  右值" << std::endl; }
        void processValue(const Foo&&)  { std::cout << "  常右值" << std::endl; }

        Foo&        lvalue(void)        { static       Foo xx; return xx; }
        const Foo& clvalue(void)        { static const Foo xx; return xx; }
        Foo         rvalue(void)        { return Foo(); }
        const Foo  crvalue(void)        { return Foo(); }

        template <typename T>
        void forwardValue(T& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;
            processValue(val);
        }

        template <typename T>
        void forwardValue(const T& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;
            processValue(val);
        }

        static int main0(void)
        {
            cout << __FUNCTION__ << endl;
            forwardValue(lvalue());
            forwardValue(clvalue());
            forwardValue(rvalue());
            forwardValue(crvalue());
            return 0;
        }
        template <typename T>
        struct fvalue
        {
            typedef T type_t;

            type_t content_;

            fvalue(type_t r)
                : content_(r)
            {}

            operator type_t () const
            {
                return content_;
            }
        };

        template <typename T>
        struct fvalue<T&&>
        {
            typedef T&& type_t;

            type_t content_;

            fvalue(type_t r)
                : content_(std::move(r))
            {
            }

            operator type_t () const
            {
                return std::move(content_);
            }
        };

        template <typename T> inline fvalue<T&>        pass(T& r)        { return fvalue<T&>(r); }
        template <typename T> inline fvalue<const T&>  pass(const T& r)  { return fvalue<const T&>(r); }
        template <typename T> inline fvalue<T&&>       pass(T&& r)       { return fvalue<T&&>(std::move(r)); }
        template <typename T> inline fvalue<const T&&> pass(const T&& r) { return fvalue<const T&&>(std::move(r)); }

        template <typename T> inline T  forward(fvalue<T>& r) { return r; }
        template <typename T> inline T& forward(T& r)         { return r; }

        template <typename T>
        void forwardValue1(T val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;
            processValue(forward(val));
        }

        int main1(void)
        {
            cout << __FUNCTION__ << endl;
            forwardValue1(pass(lvalue()));
            forwardValue1(pass(clvalue()));
            forwardValue1(pass(rvalue()));
            forwardValue1(pass(crvalue()));
            return 0;
        }
        template <typename T> struct type_check;
        template <typename T> struct type_check<T&>        { type_check() { std::cout << "  左值   -> "; } };
        template <typename T> struct type_check<const T&>  { type_check() { std::cout << "  常左值 -> "; } };
        template <typename T> struct type_check<T&&>       { type_check() { std::cout << "  右值   -> "; } };
        template <typename T> struct type_check<const T&&> { type_check() { std::cout << "  常右值 -> "; } };

        template <typename T>
        void forwardValue2(T&& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;
            type_check<T&&>();
            processValue(val);
        }

        int main2(void)
        {
            cout << __FUNCTION__ << endl;
            forwardValue2(lvalue());
            forwardValue2(clvalue());
            forwardValue2(rvalue());
            forwardValue2(crvalue());
            return 0;
        }
        template <typename T>
        void forwardValue3(T&& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;

            // 由于具名的右值引用其实是一个左值，因此T&&类型的形参本身，将会丢失右值属性。
            // 为了能够还原它原本的属性，我们需要做一下类型转换：
            type_check<T&&>();
            processValue(static_cast<T&&>(val));
        }
        int main3(void)
        {
            cout << __FUNCTION__ << endl;
            forwardValue3(lvalue());
            forwardValue3(clvalue());
            forwardValue3(rvalue());
            forwardValue3(crvalue());
            return 0;
        }

        template <typename T>
        void forwardValue4(T&& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;

            type_check<T&&>();
            processValue(std::forward<T>(val));
        }
        int main4(void)
        {
            cout << __FUNCTION__ << endl;

            forwardValue4(lvalue());
            forwardValue4(clvalue());
            forwardValue4(rvalue());
            forwardValue4(crvalue());
            return 0;
        }

        /// std::forward的山寨版本
        template<typename T>
        T&& forward5(T& a)
        {
            return static_cast<T&&>(a);
        }

        template <typename T>
        void forwardValue5(T&& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;
            /// 原因在于val始终是一个左值，因此forward自动推导出来的T将是非引用类型，T&&将始终都是一个右值引用
            type_check<T&&>();
            processValue(forward5(val));
        }
        int main5(void)
        {
            cout << __FUNCTION__ << endl;

            forwardValue5(lvalue());
            forwardValue5(clvalue());
            forwardValue5(rvalue());
            forwardValue5(crvalue());
            return 0;
        }

        /// std::forward的山寨版本
        template<typename T>
        T&& forward6(T& a)
        {
            return static_cast<T&&>(a);
        }
        template <typename T>
        void forwardValue6(T&& val)
        {
            cout << "  " << typeid(T).name() << endl;
            cout << "  " << typeid(val).name() << endl;

            type_check<T&&>();
            processValue(forward6<T>(val));
        }
        int main6(void)
        {
            cout << __FUNCTION__ << endl;

            forwardValue6(lvalue());
            forwardValue6(clvalue());
            forwardValue6(rvalue());
            forwardValue6(crvalue());
            return 0;
        }
        // 为了禁止forward的自动推导，可以写成这样：
        // 当然了，这只是简单的forward实现方式。实际中还要考虑很多其他情况，因此std的forward实现会比这个要复杂。
        template<typename T>
        T&& forward_(typename std::identity<T>::type& a)
        {
            return static_cast<T&&>(a);
        }
        void main_()
        {
            main0();
            main1();
            main2();
            main3();
            main4();
            main5();
            main6();
        }
    }

    ////////////////////////////////////////////////////////
    /// http://www.cnblogs.com/qicosmos/p/3376241.html
    ////////////////////////////////////////////////////////
    namespace jiangnan
    {
        template<typename T>
        void PrintT(T& t)
        {
            cout << "  lvaue, ";
            t = 99;
        }

        template<typename T>
        void PrintT(T && t)
        {
            cout << "  rvalue, ";
            t = 100;
        }

        template<typename T>
        void TestForward(T && v)
        {
            PrintT(v);
             cout << "  value is " << v << endl;

//             PrintT(std::forward<T>(v));
//             cout << "  value is " << v << endl;
// 
//             PrintT(std::move(v));
//             cout << "  value is " << v << endl;
        }

        template<typename T>
        void PrintTypeName(T&& t)
        {
            cout << "  " << typeid(std::forward<T>(t)).name() << endl;
        }
        void main0()
        {
            int x = 1;
//             PrintTypeName(x);
// 
//             int* x3 = &x;//引用类型打印不出来
//             cout << "    " << typeid(int&).name() << endl;
// 
//             auto x1 = std::forward<int>(x);
//             cout << "    " << typeid(x1).name() << endl;
//             PrintTypeName(x1); 
// 
//             auto x2 = std::move(x);
//             cout << "    " << typeid(x2).name() << endl;
//             PrintTypeName(x2);

            cout << "TestForward(1)" << endl;
            TestForward(1);// l, r, r
            cout << "  value is " << x << endl;

            x = 1;
            cout << "TestForward(x)" << endl;
            TestForward(x); // l , l, r
            cout << "  value is " << x << endl;

            x = 1;
            cout << "TestForward(std::forward<int>(x))" << endl;
            TestForward(std::forward<int>(x)); // l, r ,r
            cout << "  value is " << x << endl;

            x = 1;
            cout << "TestForward(std::move(x))" << endl;
            TestForward(std::move(x)); // l, r ,r
            cout << "  value is " << x << endl;

            x = 1;
            cout << "TestForward(std::forward<int&>(x))" << endl;
            TestForward(std::forward<int&>(x));
            cout << "  value is " << x << endl;

            x = 1;
            cout << "TestForward(std::forward<int&&>(x))" << endl;
            TestForward(std::forward<int&&>(x));
            cout << "  value is " << x << endl;

        }

        template<class Function, class ... Args>
        inline auto FuncWrapper(Function && f, Args && ... args) -> decltype(f(std::forward<Args>(args)...))
        {
            //typedef decltype(f(std::forward<Args>(args)...)) ReturnType;
            return f(std::forward<Args>(args)...);
            //your code; you can use the above typedef.
        }

        void test0() { cout << "void" << endl; }
        int test1() { cout << 1 << endl; return 1; }
        int test2(int x) { cout << x << endl; return x; }
        string test3(string s1, string s2) { cout << s1.c_str() << " ," << s2.c_str() << endl; return s1 += s2; }

        void main1()
        {
            vector<int> v;
            v.emplace_back(1);

            FuncWrapper(test0);  //没有返回值，打印1
            FuncWrapper(test1); //返回1
            FuncWrapper(test2, 1); //返回1
            FuncWrapper(test3, "aa", "bb"); //返回"aabb"
        }
        void main_()
        {
            //         int w1, w2;
            //         auto&& v1 = w1;
            //         decltype(w1) && v2 = w2;
            main0();
            main1();
        }
    }
    namespace ctj
    {
        class demo
        {
        public:
            int x_ = 0;
            vector<string> v_;

        public:
            demo()
            {
                v_.assign(1000, "ackslffdsf");
            }

            demo(demo&& l) : v_(std::move(l.v_))
            {
                //             cout << "demo copy rvalue func" << endl;
            }

            demo& operator=(const demo&& l)
            {
                //             v_ = std::move(l.v_);
                //             cout << "demo assign rvalue func" << endl;
            }
        };
        
        void test_move_perf(demo& d)
        {
            demo d1 = d;
        }

        void test_move_perf(demo&& d)
        {
            demo d1 = std::move(d);
            //         demo d1 = std::forward<demo&&>(d);
        }

        template<class T>
        void ff(T val)
        {
            val++;
            cout << typeid(T).name() << " , " << typeid(val).name() << endl;
        }

        void main0()
        {
            const int a = 100;
            const int& b = a;
            int const* p = &a;
            ctj::ff(a);
            ctj::ff(b);
            ctj::ff(p);

            cout << typeid(a).name() << endl;
            cout << a << ", " << b << endl;
        }

        //     template<class T>
        //     void test_move_perf(T&& v)
        //     {
        //         vector<string> v2;
        //         v2 = v;
        //         v2 = std::move(v);
        //         v2 = std::forward< vector<string> >(v);
        //     }

        void main1()
        {
            {
                demo d;
                bool b = true;
                int count = 100;
                boost::progress_timer t;
                while (count--){
                    test_move_perf(d);
                }
            }
            {
                demo d;
                int count = 100;
                boost::progress_timer t;
                while (count--) test_move_perf(std::forward<demo>(std::move(d)));
            }
            {
                demo d;
                int count = 100;
                boost::progress_timer t;
                while (count--){
                    test_move_perf(std::move(d));
                }
            }
            {
                demo d;
                int count = 100;
                boost::progress_timer t;
                while (count--) test_move_perf(d);
            }
        }

        inline double f(double tf) { return 5.0*(tf - 32.0) / 9.0; };
        void main_()
        {
            {
                using namespace std;
                double tc = 21.5;
                double && rd1 = 7.07;
                double && rd2 = 1.8 * tc + 32.0;
                double && rd3 = f(rd2);
                cout << " tc value and address: " << tc << ", " << &tc << endl;
                cout << "rd1 value and address: " << rd1 << ", " << &rd1 << endl;
                cout << "rd2 value and address: " << rd2 << ", " << &rd2 << endl;
                cout << "rd3 value and address: " << rd3 << ", " << &rd3 << endl;
                cin.get();
            }
            main1();
        }
        ///////////////////////////////////////////////////////////////////////////
        template<class T>
        class shared_func : public std::shared_ptr<T>
        {
        public:
            template<class Y>
            shared_func(Y&& y) : std::shared_ptr<T>(std::forward<Y>(y)) {}

            template<class Arg1>
            void operator()(Arg1&& arg1) const 
            {
                auto a1 = *this;
                auto a2 = *a1;
                return a2(std::forward<Arg1>(arg1));
                return (**this)(std::forward<Arg1>(arg1)); 
            }
        };
        struct SGetRoleDetailFromGSAck
        {
            SGetRoleDetailFromGSAck(UINT64 qwPeerID)
                :_qwPeerID(qwPeerID)
            {

            };
            ~SGetRoleDetailFromGSAck()
            {
                cout << "call ~SGetRoleDetailFromGSAck()" << endl;
            }
            void SetSessionID(UINT32 dwSessionID) { _dwSessionID = dwSessionID; }
            void operator()(bool bRet) { _bRet = bRet; _hasReturned = true; }
            bool hasReturned() { return _hasReturned; }
            void Step() { SendReturn(false); }
        private:
            void SendReturn(bool isEnd);
            UINT64 _qwPeerID;
            UINT32 _dwSessionID;
            bool _bRet;
            bool _hasReturned;
        };
        void main2()
        {
            shared_func<SGetRoleDetailFromGSAck> pAck(new SGetRoleDetailFromGSAck(100));
            pAck(true);
        }
    }
}

namespace scottmeyer
{
    class Annotation {
    public:
        explicit Annotation(string& text)
            : value(std::move(text)) // "move" text into value; this code
        {
        } // doesn't do what it seems to!
        explicit Annotation(string&& text)
            : value(std::move(text)) // "move" text into value; this code
        {
        }
        void print()
        {
            cout << value << endl;
        }
    private:
        std::string value;
    };

    void process(const Annotation& lvalArg)
    {
        cout << "call lvalue " << endl;
    }// process lvalues
    void process(Annotation&& rvalArg)
    {
        cout << "call rvalue " << endl;
    }// process rvalues

    template<typename T> 
    void logAndProcess(T&& param) //  param, like all function parameters, is an lvalue. 
    {
        auto now = std::chrono::system_clock::now();
        //makeLogEntry("Calling 'process'", now);
        process(param);
        process(std::forward<T>(param));
    }
    void main1()
    {
        string name("claus");
        Annotation ann(name);
        Annotation an2(move(name));
        ann.print();

        logAndProcess(move(ann));
    }
}
template <class T>
class Singleton
{
};

template <class T>
class A : public  Singleton<T>
{
    int a = 200;

};

class A1 : public enable_shared_from_this<A1>
{
    int a1 = 100;
};

void rvalue_main()
{
    A1 a111;
    int val = -1000;
    int fix = val * 10000 / (unsigned int)2000;
    int scale = 10000 ;
    scale += fix;

//     auto pa1 = new A1();
//     shared_ptr<A1> ppa1 = pa1->shared_from_this();

    auto movefirst_ = 12;
    auto a1 = 10;
    int moveto_ = 100;
    moveto_ = ( movefirst_ ++, cout << moveto_ << endl, ++a1/*, 1000 - 1 * (2040 - 200) / 2 */) ;
    cout << moveto_ << endl;

//     scottmeyer::main1();
//     rvalue::move_main();
//     rvalue::ctj::main2();
//     foward_main();
     rvalue::jiangnan::main_();
//     rvalue::mutouyun::main_();
//     foward_main();
//     move_main();
}


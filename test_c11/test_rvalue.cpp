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
    ��Ϊ����Ҫ��һ���������ԣ���ֵ����(rvalue references)
    �����뵽 C++0x�С����ǿ���ͨ����������&&��������һ����ֵ
    ���ã�ԭ����C++��ʹ�á�&���������������������ڱ���Ϊ��ֵ
    ���á�

    ��ֵ���ú���ֵ���õı�����Ϊ����һ�£�����Ψһ�Ĳ�������
    ֵ���ÿ��԰󶨵�һ����ʱ����(��ֵ)�ϣ�����ֵ���ò����ԡ�

    ��ֵ�����������ݣ����纯���ķ���ֵһ��˵��������ֵ��������
    ֵ���в�����ʱ����ֵ��������û�б�Ҫ�����������ĳЩ���
    �¿���ֱ�ӡ��ƶ���֮��ͨ����ֵ���ã����������ȷ�����ֳ���
    ��Ĳ����Ƿ�Ϊ��ֵ���Ӷ������˲���Ҫ�Ŀ����������Ч��Ҳ��
    �õ�����ߡ����ǿ���һ���򵥵����ݽ�����С���򣬴��������
    ��ֵ������������Ч������
    *******************************************/
    template <class T>
    T&& move(T&& a)
    {
        return a;
    }
    /********************************************
    ���������ʵ�ֵĴ����У�����ʹ����һ��move()����������
    ����ĸ�ֵ�������� = ��, move()ֻ�Ǽ򵥵ؽ���һ����ֵ
    ���û�����ֵ������Ϊ������Ȼ��ֱ�ӷ�����Ӧ�������ֵ��
    �á���һ���̲����������(Copy)��������ֻ�ὫԴ������
    ��(Move)��Ŀ�����

    ���ǿ���(Copy)���ƶ�(Move)�Ĳ��ʹ����ֵ���ó�Ϊ
    C++0x��������ĵ�������֮һ����ʵ���ǶȽ������ܹ���
    ���ǽ��C++�г�������Ϊ����ڸ������ʱ�����Ч�����⡣
    �����Ա���������ȫ��C++�е�������������ֵ��ֵ����
    ��ȱ�ݡ��ӿ�����ߵĽǶȽ���������������ִ�����һ��
    �����������ڹ��Ŀ�ʹ���߶��ԣ�����һ��һ����ܹ���
    �á���ѵġ�Ч������
    *********************************************/
    template <class T>
    void _swap(T& a, T& b)
    {
        T tmp(std::move(a)); // ����a���ƶ�������tmp��a����� ??
        a = std::move(b);    // ����b���ƶ�������a��b�����
        b = std::move(tmp);  // ����tmp���ƶ�������b
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
        //�������Ż��������ο���
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

        void processValue(Foo&)         { std::cout << "  ��ֵ" << std::endl; }
        void processValue(const Foo&)   { std::cout << "  ����ֵ" << std::endl; }
        void processValue(Foo&&)        { std::cout << "  ��ֵ" << std::endl; }
        void processValue(const Foo&&)  { std::cout << "  ����ֵ" << std::endl; }

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
        template <typename T> struct type_check<T&>        { type_check() { std::cout << "  ��ֵ   -> "; } };
        template <typename T> struct type_check<const T&>  { type_check() { std::cout << "  ����ֵ -> "; } };
        template <typename T> struct type_check<T&&>       { type_check() { std::cout << "  ��ֵ   -> "; } };
        template <typename T> struct type_check<const T&&> { type_check() { std::cout << "  ����ֵ -> "; } };

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

            // ���ھ�������ֵ������ʵ��һ����ֵ�����T&&���͵��βα������ᶪʧ��ֵ���ԡ�
            // Ϊ���ܹ���ԭ��ԭ�������ԣ�������Ҫ��һ������ת����
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

        /// std::forward��ɽկ�汾
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
            /// ԭ������valʼ����һ����ֵ�����forward�Զ��Ƶ�������T���Ƿ��������ͣ�T&&��ʼ�ն���һ����ֵ����
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

        /// std::forward��ɽկ�汾
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
        // Ϊ�˽�ֹforward���Զ��Ƶ�������д��������
        // ��Ȼ�ˣ���ֻ�Ǽ򵥵�forwardʵ�ַ�ʽ��ʵ���л�Ҫ���Ǻܶ�������������std��forwardʵ�ֻ�����Ҫ���ӡ�
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
//             int* x3 = &x;//�������ʹ�ӡ������
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

            FuncWrapper(test0);  //û�з���ֵ����ӡ1
            FuncWrapper(test1); //����1
            FuncWrapper(test2, 1); //����1
            FuncWrapper(test3, "aa", "bb"); //����"aabb"
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


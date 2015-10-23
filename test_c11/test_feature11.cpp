#include <typeinfo>
#include <iostream>
#include <vector>
#include <map>
#include <initializer_list>

namespace feature_default_delete
{
    class X 
    {
    public:
    //     Defaulted 函数特性仅适用于类的特殊成员函数，且该特殊成员函数没有默认参数。例如：
//         int f() = default;      // 错误 , 函数 f() 非类 X 的特殊成员函数
//         X(int) = default;       // 错误 , 构造函数 X(int, int) 非 X 的特殊成员函数
//         X(int = 1) = default;   // 错误 , 默认构造函数 X(int=1) 含有默认参数

    //    Defaulted 函数既可以在类体里（inline）定义，也可以在类体外（out - of - line）定义。例如：
        X() = default; //Inline defaulted 默认构造函数
        X(const X&);
        X& operator = (const X&);
        ~X() = default;  //Inline defaulted 析构函数

    };
    
    X::X(const X&) = default;  //Out-of-line defaulted 拷贝构造函数
    X& X::operator = (const X&) = default;     //Out-of-line defaulted 拷贝赋值操作符

    class A {
    public:
        virtual ~A() = default; // 编译器自动生成 defaulted 函数定义体
    private:
        int x;
    };

    class B : public A {
    private:
        int y;
    };

    //C++11 标准引入了一个新特性：deleted 函数。程序员只需在函数声明后加上“ = delete; ”，
    //就可将该函数禁用。例如，我们可以将类X的拷贝构造函数以及拷贝赋值操作符声明为 deleted 函数，
    //就可以禁止类X对象之间的拷贝和赋值。
    class X1
    {
    public:
        X1();
        X1(const X1&) = delete;  // 声明拷贝构造函数为 deleted 函数
        X1& operator = (const X1 &) = delete; // 声明拷贝赋值操作符为 deleted 函数
    };

//         X x1;
//         X x2 = x1;   // 错误，拷贝构造函数被禁用
//         X x3;
//         x3 = x1;     // 错误，拷贝赋值操作符被禁用
    
    //Deleted 函数特性还可用于禁用类的某些转换构造函数，从而避免不期望的类型转换。
    //在清单 12 中，假设类X只支持参数为双精度浮点数 double 类型的转换构造函数，
    //而不支持参数为整数 int 类型的转换构造函数，则可以将参数为 int 类型的转换构造
    //函数声明为 deleted 函数。

    //    清单 12
    class X12{
    public:
        X12(double);
        X12(int) = delete;
    };

//     int main(){
//         X12 x1(1.2);
//         X12 x2(2); // 错误，参数为整数 int 类型的转换构造函数被禁用          
//     }

//     Deleted 函数特性还可以用来禁用某些用户自定义的类的new操作符，从而避免在自由存储区创建类的对象。例如：
// 
//         清单 13
// #include <cstddef> 
//         using namespace std;

    class X13{
    public:
        void *operator new(size_t) = delete;
        void *operator new[](size_t) = delete;
    };

//     int main(){
//         X13 *pa = new X13;  // 错误，new 操作符被禁用
//         X13 *pb = new X13[10];  // 错误，new[] 操作符被禁用
//     }

//     虽然 defaulted 函数特性规定了只有类的特殊成员函数才能被声明为 defaulted 函数，但是 deleted 函数特性并没有此限制。非类的成员函数，即普通函数也可以被声明为 deleted 函数。例如：
// 
//         清单 15
        int add(int, int) = delete;
        double add(double a, double b){
            return a + b;
        }
//     int main(){
//         int a, b;
//         add(a, b); // 错误，函数 add(int, int) 被禁用
//         cout << add(1, 3) << endl;    // 错误，调用了 deleted 函数 add(int, int) 
//         cout << add(1.2, 1.3) << endl;
//     }
}
namespace feature_decltype
{
    // typeid(类名).name()可以返回类型的名字，typeid(类名).hash_code()返回类型对应的唯一hash值
    // 该hash值可以比较，如：typeid(类1).hash_code() == typeid(类名2).hash_code()。
    // is_same<类名1, 类名2>::value也可以判断两个类是否相同。

    // decltype不像auto那样从变量声明的初始化表达式获得变量的类型，decltype总是以一个普通的表达式为参数，
    // 返回该表达式的类型。decltype可以获得的类型来定义另外一个变量，与auto相同的是，decltype也是在编译是进行的
    using namespace std;
    void main()
    {
        int i;
        decltype(i) j = 0;
        cout << typeid(j).name() << endl;   // 打印出"i", g++表示integer

        float a;
        double b;
        decltype(a + b) c;
        cout << typeid(c).name() << endl;   // 打印出"d", g++表示double

        //  decltype和using结合
        using size_t = decltype(sizeof(0));//size_t就可以跨平台使用了

        vector<int> vec;
        typedef decltype(vec.begin()) vectype;

        vectype i1;  // 这是auto无法做到的
        for (i1 = vec.begin(); i1 < vec.end(); i1++) {
            // 做一些事情
        }

        for (decltype(vec)::iterator i11 = vec.begin(); i11 < vec.end(); i11++) {
            // 做一些事情
        }
    }

    // decltype可以重用匿名类型
    enum { K1, K2, K3 }anon_e;   // 匿名的强类型枚举

    union {
        decltype(anon_e) key;
        char* name;
    }anon_u;    // 匿名的union

    struct {
        int d;
        decltype(anon_u) id;
    }anon_s[100];   // 匿名的struct数组

    void main1()
    {
        decltype(anon_s) as;//重用匿名类型的类型
        as[0].id.key = decltype(anon_e)::K1;   // 引用匿名强类型枚举中的值
    }//通过匿名的变量名anon_e等加上decltype可以推断这些匿名类型的类型并进行重用，不过这个似乎违背了匿名类型的设计初衷

    // s的类型被声明为decltype(t1 + t2)  
    template<typename T1, typename T2>
    void Sum(T1 & t1, T2 & t2, decltype(t1 + t2) & s) {//可以通过参数引用s返回这个不确定类型的返回值  
        s = t1 + t2;
    }
    // decltype用于实例化模板
    int hash(char*){ return 100; };//一个hash函数
    //map<char*, decltype(hash)> dict_key;    // 无法通过编译，hash是个函数名，hash(nullptr)才是表达式满足decltype
    map<int, decltype(hash(nullptr))> dict_key;

    void main2()
    {
        dict_key.insert(make_pair(1, hash(nullptr)));
    }
    //模板类result_of实现就是基于decltype的，用于推断函数的返回值类型，位于头文件<type_traits>
    typedef double(*func)();
    result_of<func()>::type f;   // 由func()推导其结果类型，double

    //decltype(e)推断的四个规则：
    //    1） 当e是一个没有带括号的标记符表达式(除去关键字、字面量等编译器需要使用的标记之外的程序员自定
    //        义的标记)或者类成员访问表达式，那么decltype(e)就是e所命名的实体的类型。如果e是一个被重
    //        载的函数名，则会编译错误。
    //    2） 否则，假设e类型是T，若e是一个将亡值(如函数非引用返回值)，那么decltype(e)为T &&
    //    3） 否则，若e是一个左值，decltype(e)为T&，这是一个左值规则
    //    4） 否则，decltype(e)为T
    int i = 4;
    int arr[5] = { 0 };
    int *ptr = arr;

    struct S { double d; } s;

    void Overloaded(int);
    void Overloaded(char);      // 重载的函数

    int && RvalRef();

    const bool Func(int);

    // 规则1: 单个标记符表达式以及访问类成员，推导为本类型
    decltype(arr) var1;            // int[5], 标记符表达式
    decltype(ptr) var2;            // int*, 标记符表达式
    decltype(s.d) var4;            // double, 成员访问表达式
//     decltype(Overloaded) var5;     // 无法通过编译，是个重载的函数

    // 规则2: 将亡值，推导为类型的右值引用
    decltype(RvalRef()) var6 = 1;  // int&&

    // 规则3: 左值，推导为类型的引用
    decltype(true ? i : i) var7 = i;    // int&, 三元运算符，这里返回一个i的左值
    decltype((i)) var8 = i;             // int&, 带圆括号的左值 
    decltype(++i) var9 = i;             // int&, ++i返回i的左值
    decltype(arr[3]) var10 = i;         // int& []操作返回左值
    decltype(*ptr)  var11 = i;          // int& *操作返回左值
    decltype("lval") var12 = "lval";    // const char(&)[9], 字符串字面常量为左值

    // 规则4：以上都不是，推导为本类型
    decltype(1) var13;              // int, 除字符串外字面常量为右值
    decltype(i++) var14;            // int, i++返回右值
    decltype((Func(1))) var15;      // const bool, 圆括号可以忽略

    void main3(){
        cout << is_rvalue_reference<decltype(RvalRef())>::value << endl;    // 1  

        cout << is_lvalue_reference<decltype(true ? i : i)>::value << endl; // 1  
        cout << is_lvalue_reference<decltype((i))>::value << endl;          // 1  
        cout << is_lvalue_reference<decltype(++i)>::value << endl;          // 1  
        cout << is_lvalue_reference<decltype(arr[3])>::value << endl;       // 1  
        cout << is_lvalue_reference<decltype(*ptr)>::value << endl;         // 1  
        cout << is_lvalue_reference<decltype("lval")>::value << endl;       // 1  

        cout << is_lvalue_reference<decltype(i++)>::value << endl;          // 0  
        cout << is_rvalue_reference<decltype(i++)>::value << endl;          // 0  

        int j = 3;
        int &k = j;
        const int &n1 = j;
        //decltype(n1) i1; // i1 type const int &
        decltype(j) i2; // i2 type int
        //decltype((j)) i3; // i3 type int &
        decltype(k + 1) i4; // i4 type int
    }

    //decltype可以带来冗余的符号，如decltype(T) & a中decltype本身可能就推断出是个引用，
    //后面由出现一个&则出现冗余的&。auto不能带走初始化表达式的cv限制符，但是decltype却能带
    //走其表达式的cv限制符

    void main4() {
        const int ic = 0;
        volatile int iv;
        struct S { int i; };
        const S a = { 0 };
        volatile S b;
        volatile S* p = &b;     
        
        cout << is_const<decltype(ic)>::value << endl;      // 1  
        cout << is_volatile<decltype(iv)>::value << endl;   // 1  

        cout << is_const<decltype(a)>::value << endl;       // 1  
        cout << is_volatile<decltype(b)>::value << endl;    // 1  

        cout << is_const<decltype(a.i)>::value << endl;     // 0, 成员不是const //成员变量失去了其所属类型的cv属性  
        cout << is_volatile<decltype(p->i)>::value << endl; // 0, 成员不是volatile  
    }

    template<typename T1, typename T2>
    auto Mul(const T1 & t1, const T2 & t2) -> decltype(t1 + t2)
    {//当模板的返回值类型是随着模板实例化才能确定时，通过auto和decltype追踪返回值类型，auto相当于占位符，最后模板实例化从左至右，最后decltype推导出返回值类型填写回auto处即可  
        return t1 + t2;
    }

    // 有的时候，你会发现这是面试题  
    int(*(*pf())())() {//这个看起来真的比较蛋疼  
        return nullptr;
    }

    // auto (*)() -> int(*) () 一个返回函数指针的函数(假设为a函数)  
    // auto pf1() -> auto (*)() -> int (*)() 一个返回a函数的指针的函数  
    auto pf1() -> auto (*)() -> int(*)() {//从右向左解析  
        return nullptr;
    }

    void main5() {
        cout << is_same<decltype(pf), decltype(pf1)>::value << endl;    // 1  
    }

    //追踪返回值类型用于转发函数中
    double foo(int a) {
        return (double)a + 0.1;
    }

    int foo(double b) {
        return (int)b;
    }

    template <class T>
    auto Forward(T t) -> decltype(foo(t)){//转发函数  
        return foo(t);
    }

    void main6(){
        cout << Forward(2) << endl;     // 2.1  
        cout << Forward(0.5) << endl;   // 0  
    }

    //新的for循环方式
    void main7() {
        vector<int> v = { 1, 2, 3, 4, 5 };
        for (auto i = v.begin(); i != v.end(); ++i)
            cout << *i << endl;     // i是迭代器对象  

        for (auto e : v)//冒号分开，前面部分是范围内用于迭代的变量，后半部分是迭代的范围，只能用于明确已知迭代范围  
            cout << e << endl;      // e是解引用后的对象  
    }
}
namespace feature_initlist
{
    using namespace std;
    double sum(std::initializer_list<double> il);
    void main()
    {
        double total = sum({ 2.5, 3.1, 4 }); // 4 converted to 4.0
    }
    double sum(std::initializer_list<double> il)
    {
        double tot = 0;
        for (auto p = il.begin(); p != il.end(); p++)
            tot += *p;
        return tot;
    }
}
namespace feature_enum
{
    enum Old1 { yes, no, maybe }; // traditional form
    enum class New1 { never, sometimes, often, always }; // new form
    enum struct New2 { never, lever, sever }; // new form
}

namespace feature_variadic_template
{
    template<typename ... Args>
    int bar(Args ... arg)
    {
        return 0;
    }
    template<typename ...Args>
    int foo() 
    {
        int(*p)(Args...) = bar<Args...>;
        p(1, 0.5);
        return bar<Args...>(1, 0.5);
    }

    //展开类型参数用来指定基类列表: 该类继承 bar, barz 两个基类
//     template<typename... Args>
//     class C : Args...{
//     };
//     C<bar, barz> f; // 

    //将参数包展开到构造函数的初始化列表
//     template<typename... Args>
//     class C2 : Args...{
//     public:
//         C2(Args...args) :Args(args)...{
//         }
//     };
//     C2<bar, barz> f(bar(), barz());

    //获取模板变长参数的实参长度
//     template<typename ... Args>
//     size_t foo() {
//         return sizeof...(Args);
//     }
//     template<typename ... Args>
//     size_t fooz(Args ...args) {
//         return sizeof...(args);
//     }

    //可变参数模板与模板特化
//     template<typename ... Args>
//     void output(Args ... args) {
//     }
//     template<typename Arg0, typename ... Args>
//     void output(Arg0 arg0, Args ... args) {
//         std::cout << arg0;
//         output<Args...>(args...);
//     }
//     template<>
//     void output() {
//     }

    /*------------------------------*/
//     typedef decltype(std::cout) cout_type;
//     auto endl = std::endl<cout_type::char_type, cout_type::traits_type>;

    void main()
    {
        //foo<int , float>();
//         output(1, 0.3, (void*)nullptr, "string", 'c'/*, endl*/);
    }
}

namespace anonymous_namespace{

    // 在工程实践中，匿名 namespace 有两大不利之处：
    // 其中的函数难以设断点，如果你像我一样使用的是 gdb 这样的文本模式 debugger。
    // 使用某些版本的 g++ 时，同一个文件每次编译出来的二进制文件会变化，
    // 这让某些 build tool 失灵。
    //
    template <char *p>
    struct foo
    {
//         void bar();
    };

    static char a = 'a';

    namespace
    {
        char b = 'b';
        static char c = 'c';

        template <class T> struct xxx {};

        void foobar()
        {
            struct no_linkage {};
            xxx<no_linkage>();  // 如果编译错误，说明no_linkage的linkage没有变化
        }
    }

    int main()
    {
//         foo<&a>().bar();  // 由于a的linkage是internal，因此应该编译错误
//         foo<&b>().bar();  // 如果编译正确，说明b的linkage是external
//         foo<&c>().bar();  // 如果编译错误，说明c的linkage是internal

        foobar();

        return 0;
    }
}

namespace{

    void atexit_handler_1()
    {
        std::cout << "at exit #1\n";
    }

    void atexit_handler_2()
    {
        std::cout << "at exit #2\n";
    }

    int main()
    {
        const int result_1 = std::atexit(atexit_handler_1);
        const int result_2 = std::atexit(atexit_handler_2);

        if ((result_1 != 0) || (result_2 != 0)) {
            std::cerr << "Registration failed\n";
            return EXIT_FAILURE;
        }

        std::cout << "returning from main\n";
        return EXIT_SUCCESS;
    }
}

void feature_main()
{
    main();
    feature_decltype::main5();
    feature_variadic_template::main();
}
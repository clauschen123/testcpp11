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
    //     Defaulted �������Խ���������������Ա�������Ҹ������Ա����û��Ĭ�ϲ��������磺
//         int f() = default;      // ���� , ���� f() ���� X �������Ա����
//         X(int) = default;       // ���� , ���캯�� X(int, int) �� X �������Ա����
//         X(int = 1) = default;   // ���� , Ĭ�Ϲ��캯�� X(int=1) ����Ĭ�ϲ���

    //    Defaulted �����ȿ����������inline�����壬Ҳ�����������⣨out - of - line�����塣���磺
        X() = default; //Inline defaulted Ĭ�Ϲ��캯��
        X(const X&);
        X& operator = (const X&);
        ~X() = default;  //Inline defaulted ��������

    };
    
    X::X(const X&) = default;  //Out-of-line defaulted �������캯��
    X& X::operator = (const X&) = default;     //Out-of-line defaulted ������ֵ������

    class A {
    public:
        virtual ~A() = default; // �������Զ����� defaulted ����������
    private:
        int x;
    };

    class B : public A {
    private:
        int y;
    };

    //C++11 ��׼������һ�������ԣ�deleted ����������Աֻ���ں�����������ϡ� = delete; ����
    //�Ϳɽ��ú������á����磬���ǿ��Խ���X�Ŀ������캯���Լ�������ֵ����������Ϊ deleted ������
    //�Ϳ��Խ�ֹ��X����֮��Ŀ����͸�ֵ��
    class X1
    {
    public:
        X1();
        X1(const X1&) = delete;  // �����������캯��Ϊ deleted ����
        X1& operator = (const X1 &) = delete; // ����������ֵ������Ϊ deleted ����
    };

//         X x1;
//         X x2 = x1;   // ���󣬿������캯��������
//         X x3;
//         x3 = x1;     // ���󣬿�����ֵ������������
    
    //Deleted �������Ի������ڽ������ĳЩת�����캯�����Ӷ����ⲻ����������ת����
    //���嵥 12 �У�������Xֻ֧�ֲ���Ϊ˫���ȸ����� double ���͵�ת�����캯����
    //����֧�ֲ���Ϊ���� int ���͵�ת�����캯��������Խ�����Ϊ int ���͵�ת������
    //��������Ϊ deleted ������

    //    �嵥 12
    class X12{
    public:
        X12(double);
        X12(int) = delete;
    };

//     int main(){
//         X12 x1(1.2);
//         X12 x2(2); // ���󣬲���Ϊ���� int ���͵�ת�����캯��������          
//     }

//     Deleted �������Ի�������������ĳЩ�û��Զ�������new���������Ӷ����������ɴ洢��������Ķ������磺
// 
//         �嵥 13
// #include <cstddef> 
//         using namespace std;

    class X13{
    public:
        void *operator new(size_t) = delete;
        void *operator new[](size_t) = delete;
    };

//     int main(){
//         X13 *pa = new X13;  // ����new ������������
//         X13 *pb = new X13[10];  // ����new[] ������������
//     }

//     ��Ȼ defaulted �������Թ涨��ֻ����������Ա�������ܱ�����Ϊ defaulted ���������� deleted �������Բ�û�д����ơ�����ĳ�Ա����������ͨ����Ҳ���Ա�����Ϊ deleted ���������磺
// 
//         �嵥 15
        int add(int, int) = delete;
        double add(double a, double b){
            return a + b;
        }
//     int main(){
//         int a, b;
//         add(a, b); // ���󣬺��� add(int, int) ������
//         cout << add(1, 3) << endl;    // ���󣬵����� deleted ���� add(int, int) 
//         cout << add(1.2, 1.3) << endl;
//     }
}
namespace feature_decltype
{
    // typeid(����).name()���Է������͵����֣�typeid(����).hash_code()�������Ͷ�Ӧ��Ψһhashֵ
    // ��hashֵ���ԱȽϣ��磺typeid(��1).hash_code() == typeid(����2).hash_code()��
    // is_same<����1, ����2>::valueҲ�����ж��������Ƿ���ͬ��

    // decltype����auto�����ӱ��������ĳ�ʼ�����ʽ��ñ��������ͣ�decltype������һ����ͨ�ı��ʽΪ������
    // ���ظñ��ʽ�����͡�decltype���Ի�õ���������������һ����������auto��ͬ���ǣ�decltypeҲ���ڱ����ǽ��е�
    using namespace std;
    void main()
    {
        int i;
        decltype(i) j = 0;
        cout << typeid(j).name() << endl;   // ��ӡ��"i", g++��ʾinteger

        float a;
        double b;
        decltype(a + b) c;
        cout << typeid(c).name() << endl;   // ��ӡ��"d", g++��ʾdouble

        //  decltype��using���
        using size_t = decltype(sizeof(0));//size_t�Ϳ��Կ�ƽ̨ʹ����

        vector<int> vec;
        typedef decltype(vec.begin()) vectype;

        vectype i1;  // ����auto�޷�������
        for (i1 = vec.begin(); i1 < vec.end(); i1++) {
            // ��һЩ����
        }

        for (decltype(vec)::iterator i11 = vec.begin(); i11 < vec.end(); i11++) {
            // ��һЩ����
        }
    }

    // decltype����������������
    enum { K1, K2, K3 }anon_e;   // ������ǿ����ö��

    union {
        decltype(anon_e) key;
        char* name;
    }anon_u;    // ������union

    struct {
        int d;
        decltype(anon_u) id;
    }anon_s[100];   // ������struct����

    void main1()
    {
        decltype(anon_s) as;//�����������͵�����
        as[0].id.key = decltype(anon_e)::K1;   // ��������ǿ����ö���е�ֵ
    }//ͨ�������ı�����anon_e�ȼ���decltype�����ƶ���Щ�������͵����Ͳ��������ã���������ƺ�Υ�����������͵���Ƴ���

    // s�����ͱ�����Ϊdecltype(t1 + t2)  
    template<typename T1, typename T2>
    void Sum(T1 & t1, T2 & t2, decltype(t1 + t2) & s) {//����ͨ����������s���������ȷ�����͵ķ���ֵ  
        s = t1 + t2;
    }
    // decltype����ʵ����ģ��
    int hash(char*){ return 100; };//һ��hash����
    //map<char*, decltype(hash)> dict_key;    // �޷�ͨ�����룬hash�Ǹ���������hash(nullptr)���Ǳ��ʽ����decltype
    map<int, decltype(hash(nullptr))> dict_key;

    void main2()
    {
        dict_key.insert(make_pair(1, hash(nullptr)));
    }
    //ģ����result_ofʵ�־��ǻ���decltype�ģ������ƶϺ����ķ���ֵ���ͣ�λ��ͷ�ļ�<type_traits>
    typedef double(*func)();
    result_of<func()>::type f;   // ��func()�Ƶ��������ͣ�double

    //decltype(e)�ƶϵ��ĸ�����
    //    1�� ��e��һ��û�д����ŵı�Ƿ����ʽ(��ȥ�ؼ��֡��������ȱ�������Ҫʹ�õı��֮��ĳ���Ա�Զ�
    //        ��ı��)�������Ա���ʱ��ʽ����ôdecltype(e)����e��������ʵ������͡����e��һ������
    //        �صĺ����������������
    //    2�� ���򣬼���e������T����e��һ������ֵ(�纯�������÷���ֵ)����ôdecltype(e)ΪT &&
    //    3�� ������e��һ����ֵ��decltype(e)ΪT&������һ����ֵ����
    //    4�� ����decltype(e)ΪT
    int i = 4;
    int arr[5] = { 0 };
    int *ptr = arr;

    struct S { double d; } s;

    void Overloaded(int);
    void Overloaded(char);      // ���صĺ���

    int && RvalRef();

    const bool Func(int);

    // ����1: ������Ƿ����ʽ�Լ��������Ա���Ƶ�Ϊ������
    decltype(arr) var1;            // int[5], ��Ƿ����ʽ
    decltype(ptr) var2;            // int*, ��Ƿ����ʽ
    decltype(s.d) var4;            // double, ��Ա���ʱ��ʽ
//     decltype(Overloaded) var5;     // �޷�ͨ�����룬�Ǹ����صĺ���

    // ����2: ����ֵ���Ƶ�Ϊ���͵���ֵ����
    decltype(RvalRef()) var6 = 1;  // int&&

    // ����3: ��ֵ���Ƶ�Ϊ���͵�����
    decltype(true ? i : i) var7 = i;    // int&, ��Ԫ����������ﷵ��һ��i����ֵ
    decltype((i)) var8 = i;             // int&, ��Բ���ŵ���ֵ 
    decltype(++i) var9 = i;             // int&, ++i����i����ֵ
    decltype(arr[3]) var10 = i;         // int& []����������ֵ
    decltype(*ptr)  var11 = i;          // int& *����������ֵ
    decltype("lval") var12 = "lval";    // const char(&)[9], �ַ������泣��Ϊ��ֵ

    // ����4�����϶����ǣ��Ƶ�Ϊ������
    decltype(1) var13;              // int, ���ַ��������泣��Ϊ��ֵ
    decltype(i++) var14;            // int, i++������ֵ
    decltype((Func(1))) var15;      // const bool, Բ���ſ��Ժ���

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

    //decltype���Դ�������ķ��ţ���decltype(T) & a��decltype������ܾ��ƶϳ��Ǹ����ã�
    //�����ɳ���һ��&����������&��auto���ܴ��߳�ʼ�����ʽ��cv���Ʒ�������decltypeȴ�ܴ�
    //������ʽ��cv���Ʒ�

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

        cout << is_const<decltype(a.i)>::value << endl;     // 0, ��Ա����const //��Ա����ʧȥ�����������͵�cv����  
        cout << is_volatile<decltype(p->i)>::value << endl; // 0, ��Ա����volatile  
    }

    template<typename T1, typename T2>
    auto Mul(const T1 & t1, const T2 & t2) -> decltype(t1 + t2)
    {//��ģ��ķ���ֵ����������ģ��ʵ��������ȷ��ʱ��ͨ��auto��decltype׷�ٷ���ֵ���ͣ�auto�൱��ռλ�������ģ��ʵ�����������ң����decltype�Ƶ�������ֵ������д��auto������  
        return t1 + t2;
    }

    // �е�ʱ����ᷢ������������  
    int(*(*pf())())() {//�����������ıȽϵ���  
        return nullptr;
    }

    // auto (*)() -> int(*) () һ�����غ���ָ��ĺ���(����Ϊa����)  
    // auto pf1() -> auto (*)() -> int (*)() һ������a������ָ��ĺ���  
    auto pf1() -> auto (*)() -> int(*)() {//�����������  
        return nullptr;
    }

    void main5() {
        cout << is_same<decltype(pf), decltype(pf1)>::value << endl;    // 1  
    }

    //׷�ٷ���ֵ��������ת��������
    double foo(int a) {
        return (double)a + 0.1;
    }

    int foo(double b) {
        return (int)b;
    }

    template <class T>
    auto Forward(T t) -> decltype(foo(t)){//ת������  
        return foo(t);
    }

    void main6(){
        cout << Forward(2) << endl;     // 2.1  
        cout << Forward(0.5) << endl;   // 0  
    }

    //�µ�forѭ����ʽ
    void main7() {
        vector<int> v = { 1, 2, 3, 4, 5 };
        for (auto i = v.begin(); i != v.end(); ++i)
            cout << *i << endl;     // i�ǵ���������  

        for (auto e : v)//ð�ŷֿ���ǰ�沿���Ƿ�Χ�����ڵ����ı�������벿���ǵ����ķ�Χ��ֻ��������ȷ��֪������Χ  
            cout << e << endl;      // e�ǽ����ú�Ķ���  
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

    //չ�����Ͳ�������ָ�������б�: ����̳� bar, barz ��������
//     template<typename... Args>
//     class C : Args...{
//     };
//     C<bar, barz> f; // 

    //��������չ�������캯���ĳ�ʼ���б�
//     template<typename... Args>
//     class C2 : Args...{
//     public:
//         C2(Args...args) :Args(args)...{
//         }
//     };
//     C2<bar, barz> f(bar(), barz());

    //��ȡģ��䳤������ʵ�γ���
//     template<typename ... Args>
//     size_t foo() {
//         return sizeof...(Args);
//     }
//     template<typename ... Args>
//     size_t fooz(Args ...args) {
//         return sizeof...(args);
//     }

    //�ɱ����ģ����ģ���ػ�
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

    // �ڹ���ʵ���У����� namespace ��������֮����
    // ���еĺ���������ϵ㣬���������һ��ʹ�õ��� gdb �������ı�ģʽ debugger��
    // ʹ��ĳЩ�汾�� g++ ʱ��ͬһ���ļ�ÿ�α�������Ķ������ļ���仯��
    // ����ĳЩ build tool ʧ�顣
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
            xxx<no_linkage>();  // ����������˵��no_linkage��linkageû�б仯
        }
    }

    int main()
    {
//         foo<&a>().bar();  // ����a��linkage��internal�����Ӧ�ñ������
//         foo<&b>().bar();  // ���������ȷ��˵��b��linkage��external
//         foo<&c>().bar();  // ����������˵��c��linkage��internal

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
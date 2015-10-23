#if 0
///////////////////////////////////////////////////////////////////////////////
// std::bind\boost::bind的山寨版本, 主要学习用.
#include <stdlib.h>
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

namespace xusd{
    template <int NUM> struct placeholder{ };

    template <typename T> struct is_placeholder;
    template <int NUM> struct is_placeholder<placeholder<NUM> >{ enum{ value = NUM }; };
    template <typename T> struct is_placeholder{ enum{ value = 0 }; };

    template <int ...N> struct seq{ };
    template <unsigned N, unsigned...S> struct gen;
    template <unsigned N, unsigned...S> struct gen : gen<N - 1, N - 1, S...>{ };
    template <unsigned...S> struct gen<0, S...>{ typedef seq<S...> type; };

    template <int N, typename B, typename C>
    typename std::tuple_element<
        N,
        typename std::decay<B>::type
    >::type
    select(std::false_type, B&& b, C&& c){
        return std::get<N>(b);
    }

    template <int N, typename B, typename C>
    typename std::tuple_element <
        is_placeholder<
        typename std::tuple_element<
        N,
        typename std::decay<B>::type
        >::type
        >::value == 0 ? 0 :
        is_placeholder<
        typename std::tuple_element<
        N,
        typename std::decay<B>::type
        >::type
        >::value - 1,
        typename std::decay<C>::type
    > ::type
    select(std::true_type, B&& b, C&& c)
    {
        return std::get<
            is_placeholder<
            typename std::tuple_element<
            N,
            typename std::decay<B>::type
            >::type
            >::value - 1
        >(c);
    }

    template <typename Fun> struct GetResult{
        typedef typename std::enable_if<
            std::is_class<
            typename std::decay<Fun>::type
            >::value,
            typename GetResult<
            decltype(&Fun::operator())
            >::result_type
        >::type result_type;
    };

    template <typename R, typename... Args>
    struct GetResult<R(Args...)>{
        typedef R result_type;
    };
    template <typename C, typename R, typename... Args>
    struct GetResult<R(C::*)(Args...)>{
        typedef R result_type;
    };
    template <typename C, typename R, typename... Args>
    struct GetResult<R(C::*)(Args...)const>{
        typedef R result_type;
    };

    template <typename C, typename R>
    struct GetResult<R(C::*)>{
        typedef decltype(((C*)0)->*((R(C::*))0)) result_type;
    };

    template<typename F, typename... Args>
    class bind_t {
        typedef std::tuple<typename std::decay<Args>::type...> BindArgs;
        typedef typename std::decay<F>::type CallFun;
        enum class BindType { MemberFunction = 0, MemberObject = 1, Other = 2 };

    public:
        typedef typename GetResult<
            typename std::remove_pointer<
            typename std::remove_reference<F>::type
            >::type
        >::result_type result_type;

        bind_t(F fun, Args... args) :_fun(fun), _bindArgs(args...){ }

        template<typename... CArgs>
        result_type operator()(CArgs&&... c){
            std::tuple<CArgs...> cargs(c...);
            return callFunc(
                std::integral_constant<
                int,
                std::is_member_function_pointer<CallFun>::value ? 0 : std::is_member_object_pointer<CallFun>::value ? 1 : 2
                >(),
                cargs,
                typename gen<
                std::tuple_size<BindArgs>::value - std::is_member_function_pointer<CallFun>::value
                >::type()
                );
        }

    private:
        template<typename T, int ...S>
        result_type callFunc(std::integral_constant<int, 2>, T&& t, seq<S...>) {
            return _fun(
                select<S>(
                std::integral_constant<
                bool,
                is_placeholder<
                typename std::tuple_element<S, BindArgs>::type
                >::value != 0
                >(),
                _bindArgs,
                t)...
                );
        }

        template<typename T, int ...S>
        result_type callFunc(std::integral_constant<int, 1>, T&& t, seq<S...>) {
            return select<0>(
                std::integral_constant<
                bool,
                is_placeholder<
                typename std::tuple_element<
                0,
                BindArgs
                >::type
                >::value != 0
                >(),
                _bindArgs,
                t)->*_fun;
        }

        template<typename T, int ...S>
        result_type callFunc(std::integral_constant<int, 0>, T&& t, seq<S...>) {
            return (
                select<0>(
                std::integral_constant<
                bool,
                is_placeholder<
                typename std::tuple_element<
                0,
                BindArgs
                >::type
                >::value != 0
                >(),
                _bindArgs,
                t)->*_fun
                )
                (
                select<S + 1>(
                std::integral_constant<
                bool,
                is_placeholder<
                typename std::tuple_element<
                S + 1,
                BindArgs
                >::type
                >::value != 0
                >(),
                _bindArgs,
                t
                )...
                );
        }

    private:
        CallFun _fun;
        BindArgs _bindArgs;
    };

    template <typename F, typename... Args>
    bind_t<typename std::decay<F>::type, typename std::decay<Args&&>::type...>
        bind(F f, Args&&... args){
            return bind_t<
                typename std::decay<F>::type,
                typename std::decay<Args&&>::type...
            >(f, args...);
    }

    extern placeholder<1> _1;
    extern placeholder<2> _2;
    extern placeholder<3> _3;
    extern placeholder<4> _4;
    extern placeholder<5> _5;
    extern placeholder<6> _6;
    extern placeholder<7> _7;
    extern placeholder<8> _8;
    extern placeholder<9> _9;
    extern placeholder<10> _10;
    extern placeholder<11> _11;
    extern placeholder<12> _12;
    extern placeholder<13> _13;
    extern placeholder<14> _14;
    extern placeholder<15> _15;
    extern placeholder<16> _16;
    extern placeholder<17> _17;
    extern placeholder<18> _18;
    extern placeholder<19> _19;
    extern placeholder<20> _20;
    extern placeholder<21> _21;
    extern placeholder<22> _22;
    extern placeholder<23> _23;
    extern placeholder<24> _24;
}

///////////////////////////////////////////////////////////////////////////////
// 以下开始为测试代码.

#include <cstdlib>
#include <string>
#include <iostream>
#include <gtest/gtest.h>

int add3(int x, int y, int z){
    return x + y + z;
}

std::string to_string(std::string s1, std::string s2, std::string s3){
    return s1 + s2 + s3;
}

int g_test_voidfun = 0;
void voidfun(){
    g_test_voidfun = 1;
}

class MyTest{
public:
    std::string to_string(std::string s1, std::string s2, std::string s3){
        return s1 + s2 + s3;
    }
    int add3(int x, int y, int z){
        return x + y + z;
    }

    int cadd3(int x, int y, int z) const {
        return x + y + z;
    }
    void voidfun(){
        g_test_voidfun = 2;
    }
    void constfun() const {
        g_test_voidfun = 3;
    }

    int memObj = 0;
    std::string memObj2;
};

class TestAddFuncter{
public:
    int operator()(int x, int y){
        return x + y;
    }
};


TEST(TestSeq, Test1){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<gen<0>::type, seq<> >::value));
    EXPECT_TRUE((std::is_same<gen<1>::type, seq<0> >::value));
    EXPECT_TRUE((std::is_same<gen<2>::type, seq<0, 1> >::value));
    EXPECT_TRUE((std::is_same<gen<3>::type, seq<0, 1, 2> >::value));
    EXPECT_TRUE((std::is_same<gen<4>::type, seq<0, 1, 2, 3> >::value));
    EXPECT_TRUE((std::is_same<gen<5>::type, seq<0, 1, 2, 3, 4> >::value));
    EXPECT_TRUE((std::is_same<gen<6>::type, seq<0, 1, 2, 3, 4, 5> >::value));
    EXPECT_TRUE((std::is_same<gen<7>::type, seq<0, 1, 2, 3, 4, 5, 6> >::value));
    EXPECT_TRUE((std::is_same<gen<8>::type, seq<0, 1, 2, 3, 4, 5, 6, 7> >::value));
    EXPECT_TRUE((std::is_same<gen<9>::type, seq<0, 1, 2, 3, 4, 5, 6, 7, 8> >::value));
    EXPECT_TRUE((std::is_same<gen<10>::type, seq<0, 1, 2, 3, 4, 5, 6, 7, 8, 9> >::value));
}

TEST(TestPlaceHolder, Test1){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<decltype(_1), placeholder<1> >::value));
    EXPECT_TRUE((std::is_same<decltype(_2), placeholder<2> >::value));
    EXPECT_TRUE((std::is_same<decltype(_3), placeholder<3> >::value));
    EXPECT_TRUE((std::is_same<decltype(_4), placeholder<4> >::value));
    EXPECT_TRUE((std::is_same<decltype(_5), placeholder<5> >::value));
    EXPECT_TRUE((std::is_same<decltype(_6), placeholder<6> >::value));
    EXPECT_TRUE((std::is_same<decltype(_7), placeholder<7> >::value));
    EXPECT_TRUE((std::is_same<decltype(_8), placeholder<8> >::value));
    EXPECT_TRUE((std::is_same<decltype(_9), placeholder<9> >::value));

    EXPECT_EQ(0, (is_placeholder<int>::value));
    EXPECT_EQ(0, (is_placeholder<class A>::value));
    EXPECT_EQ(1, (is_placeholder<decltype(_1)>::value));
    EXPECT_EQ(2, (is_placeholder<decltype(_2)>::value));
    EXPECT_EQ(3, (is_placeholder<decltype(_3)>::value));
    EXPECT_EQ(4, (is_placeholder<decltype(_4)>::value));
    EXPECT_EQ(5, (is_placeholder<decltype(_5)>::value));
    EXPECT_EQ(6, (is_placeholder<decltype(_6)>::value));
    EXPECT_EQ(7, (is_placeholder<decltype(_7)>::value));
    EXPECT_EQ(8, (is_placeholder<decltype(_8)>::value));
    EXPECT_EQ(9, (is_placeholder<decltype(_9)>::value));
}

TEST(TestSelectArgs, Test1){
    using namespace xusd;
    auto b = std::make_tuple(1, _1, 2, _2, 3, _3, 4, _4);
    auto c = std::make_tuple(11, 22, 33, 44);

    EXPECT_EQ(1, (select<0>(std::false_type(), b, c)));
    EXPECT_EQ(11, (select<1>(std::true_type(), b, c)));
    EXPECT_EQ(2, (select<2>(std::false_type(), b, c)));
    EXPECT_EQ(22, (select<3>(std::true_type(), b, c)));
    EXPECT_EQ(3, (select<4>(std::false_type(), b, c)));
    EXPECT_EQ(33, (select<5>(std::true_type(), b, c)));
    EXPECT_EQ(4, (select<6>(std::false_type(), b, c)));
    EXPECT_EQ(44, (select<7>(std::true_type(), b, c)));
}

TEST(TestGetResult, Test1){
    using namespace xusd;

    class Ret;
    class C;
    class Args;
    class Mem;
    EXPECT_TRUE((std::is_same<void, xusd::GetResult<void()>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, xusd::GetResult<int(int)>::result_type>::value));
    EXPECT_TRUE((std::is_same<const int, xusd::GetResult<const int()>::result_type>::value));
    EXPECT_TRUE((std::is_same<Ret, xusd::GetResult<Ret(Args)>::result_type>::value));
    EXPECT_TRUE((std::is_same<Ret, xusd::GetResult<Ret(C::*)(Args)>::result_type>::value));
    EXPECT_TRUE((std::is_same<Mem&, xusd::GetResult<Mem(C::*)>::result_type>::value));

    const MyTest t1;
    EXPECT_TRUE((std::is_same<int, decltype(t1.memObj)>::value));
}

#define DT(x) decltype(x)
TEST(TestBind_t, Test1){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<void, bind_t<void()>::result_type>::value));
    EXPECT_TRUE((std::is_same<void, bind_t<void(char), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<void, bind_t<void(char, short), DT((_1)), DT((_2))>::result_type>::value));
    EXPECT_TRUE((std::is_same<void, bind_t<void(char, short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
    EXPECT_TRUE((std::is_same<void, bind_t<void(char, short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
    EXPECT_TRUE((std::is_same<void, bind_t<void(char, short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));


    EXPECT_TRUE((std::is_same<int, bind_t<int()>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, bind_t<int(char), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, bind_t<int(char, short), DT((_1)), DT((_2))>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, bind_t<int(char, short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, bind_t<int(char, short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
    EXPECT_TRUE((std::is_same<int, bind_t<int(char, short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));


    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA()>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(char), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(char, short), DT((_1)), DT((_2))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(char, short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(char, short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(char, short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));


    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB, class CCC), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB, class CCC, class DDD), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB, class CCC, class DDD, class EEE), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB, class CCC, class DDD, class EEE, class FFF), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA(class BBB, class CCC, class DDD, class EEE, class FFF, class GGG), DT((_1))>::result_type>::value));


    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1, class Arg2), DT((_2)), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1, class Arg2, class Arg3), DT((_2)), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1, class Arg2, class Arg3, class Arg4), DT((_4)), DT((_1))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1, class Arg2, class Arg3, class Arg4, class Arg5), DT((_5))>::result_type>::value));
    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret(class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6), DT((_6))>::result_type>::value));
}

TEST(TestBind_t, Test2){
    using namespace xusd;
    bind_t<int(int, int, int), DT(_3), DT(_2), DT(_1)> t1(add3, _3, _2, _1);
    EXPECT_EQ((add3(1, 2, 3)), (t1(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (t1(0, 0, 0)));


    bind_t<std::string(std::string, std::string, std::string), DT(_1), DT(_2), DT(_3)> s1(to_string, _1, _2, _3);
    bind_t<std::string(std::string, std::string, std::string), DT(_2), DT(_3), DT(_1)> s2(to_string, _2, _3, _1);
    bind_t<std::string(std::string, std::string, std::string), DT(_3), DT(_1), DT(_2)> s3(to_string, _3, _1, _2);

    EXPECT_EQ("123", (s1("1", "2", "3")));
    EXPECT_EQ("321", (s1("3", "2", "1")));
    EXPECT_EQ("23", (s1("", "2", "3")));
    EXPECT_EQ("2_3", (s1("2", "_", "3")));
    EXPECT_EQ("231", (s2("1", "2", "3")));
    EXPECT_EQ("312", (s3("1", "2", "3")));
}



TEST(TestBind, NotMemberFunction){
    using namespace xusd;
    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(add3, _3, _2, _1)(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(add3, _3, _2, _1)(0, 0, 0)));

    EXPECT_EQ("123", (xusd::bind(to_string, _1, _2, _3)("1", "2", "3")));
    EXPECT_EQ("321", (xusd::bind(to_string, _1, _2, _3)("3", "2", "1")));
    EXPECT_EQ("23", (xusd::bind(to_string, _1, _2, _3)("", "2", "3")));
    EXPECT_EQ("2_3", (xusd::bind(to_string, _1, _2, _3)("2", "_", "3")));
    EXPECT_EQ("231", (xusd::bind(to_string, _2, _3, _1)("1", "2", "3")));
    EXPECT_EQ("312", (xusd::bind(to_string, _3, _1, _2)("1", "2", "3")));


    bind((voidfun))();
    EXPECT_EQ(g_test_voidfun, 1);
}

TEST(TestBind, PassToFunctional){
    using namespace xusd;
    EXPECT_EQ("123", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("1", "2", "3")));
    EXPECT_EQ("321", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("3", "2", "1")));
    EXPECT_EQ("23", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("", "2", "3")));
    EXPECT_EQ("2_3", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("2", "_", "3")));
    EXPECT_EQ("231", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _2, _3, _1))("1", "2", "3")));
    EXPECT_EQ("312", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _3, _1, _2))("1", "2", "3")));
}


TEST(TestBind, TestMumberFunction){
    using namespace xusd;
    MyTest test;
    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(0, 0, 0)));

    EXPECT_EQ("123", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("1", "2", "3")));
    EXPECT_EQ("321", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("3", "2", "1")));
    EXPECT_EQ("23", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("", "2", "3")));
    EXPECT_EQ("2_3", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("2", "_", "3")));
    EXPECT_EQ("231", (xusd::bind(&MyTest::to_string, &test, _2, _3, _1)("1", "2", "3")));
    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, &test, _3, _1, _2)("1", "2", "3")));

    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, _4, _3, _1, _2)("1", "2", "3", &test)));

    xusd::bind(&MyTest::voidfun, &test)();
    EXPECT_EQ(2, g_test_voidfun);

    xusd::bind(&MyTest::constfun, &test)();
    EXPECT_EQ(3, g_test_voidfun);
}

TEST(TestBind, TestFuncter){
    using namespace xusd;
    TestAddFuncter f1;
    EXPECT_EQ(3, (xusd::bind(f1, _1, _2)(2, 1)));
}

TEST(TestBind, TestCFunction){
    using namespace xusd;
    EXPECT_EQ(1, (xusd::bind(abs, _1)(-1)));
}

TEST(TestBind, TestMemberObj){
    MyTest t1;
    EXPECT_EQ(t1.memObj, 0);
    xusd::bind(&MyTest::memObj, &t1)() = 1;
    EXPECT_EQ(t1.memObj, 1);
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

namespace xusd{
    placeholder<1> _1;
    placeholder<2> _2;
    placeholder<3> _3;
    placeholder<4> _4;
    placeholder<5> _5;
    placeholder<6> _6;
    placeholder<7> _7;
    placeholder<8> _8;
    placeholder<9> _9;
    placeholder<10> _10;
    placeholder<11> _11;
    placeholder<12> _12;
    placeholder<13> _13;
    placeholder<14> _14;
    placeholder<15> _15;
    placeholder<16> _16;
    placeholder<17> _17;
    placeholder<18> _18;
    placeholder<19> _19;
    placeholder<20> _20;
    placeholder<21> _21;
    placeholder<22> _22;
    placeholder<23> _23;
    placeholder<24> _24;
}

#endif
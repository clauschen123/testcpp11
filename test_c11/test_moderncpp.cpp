#include <iostream>
#include <vector>
using namespace std;

// template deduction
namespace Item_1{

// Things to Remember
// • During template type deduction, arguments that are references are treated as
//   non - references, i.e., their reference - ness is ignored.
// • When deducing types for universal reference parameters, lvalue arguments get
//   special treatment.
// • When deducing types for by - value parameters, const and / or volatile argu‐
//   ments are treated as non - const and non - volatile.
// • During template type deduction, arguments that are array or function names
//   decay to pointers, unless they’re used to initialize references.

    template<typename T>
    void fcase1_1(T& param){}

    void main1_1()
    {
        int x = 27;             // x is an int
        const int cx = x;       // cx is a const int
        const int& rx = x;      // rx is a reference to x as a const int
        fcase1_1(x);            // T is int, param's type is int&
        fcase1_1(cx);           // T is const int,
                                // param's type is const int&
        fcase1_1(rx);           // T is const int,
                                // param's type is const int&
    }

    template<typename T>
    void fcase1_2(const T& param){} // param is now a ref-to-const
    void main1_2()
    {
        int x = 27;             // as before
        const int cx = x;       // as before
        const int& rx = x;      // as before
        fcase1_2(x);            // T is int, param's type is const int&
        fcase1_2(cx);           // T is int, param's type is const int&
        fcase1_2(rx);           // T is int, param's type is const int&
    }

    template<typename T>
    void fcase1_3(T* param){} // param is now a pointer
    void main1_3()
    {
        int x = 27;         // as before
        const int *px = &x; // px is a ptr to x as a const int
        fcase1_3(&x);       // T is int, param's type is int*
        fcase1_3(px);       // T is const int,
                            // param's type is const int*
    }

    template<typename T>
    void fcase2(T&& param){} // param is now a universal reference
    void main2()
    {
        int x = 27; // as before
        const int cx = x; // as before
        const int& rx = x; // as before

        fcase2(x); // x is lvalue, so T is int&,
        // param's type is also int&
        fcase2(cx); // cx is lvalue, so T is const int&,
        // param's type is also const int&
        fcase2(rx); // rx is lvalue, so T is const int&,
        // param's type is also const int&
        fcase2(27); // 27 is rvalue, so T is int,
        // param's type is therefore int&&
    }

    template<typename T>
    void fcase3(T param){} // param is now passed by value

    void main3()
    {
        int x = 27; // as before
        const int cx = x; // as before
        const int& rx = x; // as before
        fcase3(x); // T's and param's types are both int
        fcase3(cx); // T's and param's types are again both int
        fcase3(rx); // T's and param's types are still both int

        const char* const ptr = // ptr is const pointer to const object
            "Fun with pointers";
        fcase3(ptr);

    }

    template<typename T>
    void fcase4(T& param){} // param is now passed by value
    // return size of an array as a compile-time constant. (The
    // array parameter has no name, because we care only about
    // the number of elements it contains.)
    template<typename T, std::size_t N> 
    /*constexpr*/ std::size_t arraySize(T(&)[N]) 
    {
        return N;
    }
    void main_array()
    {
        const char name[] = "J. P. Briggs"; // name's type is const char[13]
        const char * ptrToName = name; // array decays to pointer
        fcase3(name); // array decay to pointer
        fcase4(name);// pass array to f

        int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 }; // keyVals has 7 elements
        //int mappedVals[arraySize(keyVals)]; // so does mappedVals
        //std::array<int, arraySize(keyVals)> mappedVals;
        int n = arraySize(keyVals); // so does mappedVals
    }

    // function decay to pointer
    void someFunc(int, double)
    {
    } // someFunc is a function;
    // type is void(int, double)
    template<typename T>
    void f1(T param){} // in f1, param passed by value
    template<typename T>
    void f2(T& param){} // in f2, param passed by ref
    void main_function()
    {
        f1(someFunc); // param deduced as ptr-to-func; type is void (*)(int, double)
        f2(someFunc); // param deduced as ref-to-func; type is void (&)(int, double)

        auto f11 = someFunc;
        auto& f12 = someFunc;
    }
}

// auto deduction
namespace Item_2{
    // Things to Remember
    // • auto type deduction is usually the same as template type deduction, but auto
    //   type deduction assumes that a braced initializer represents a std::initializer_list,
    //   and template type deduction doesn’t.
    // • auto in a function return type or a lambda parameter implies template type
    //   deduction, not auto type deduction.

    // C++14 permits
    //         auto to indicate that a function’s return type should be deduced(see Item 3), and
    //         C++14 lambdas may use auto in parameter declarations.However, these uses of
    //         auto employ template type deduction, not auto type deduction.So a function with an
    //         auto return type that returns a braced initializer won’t compile

    template<typename T> // template with parameter
    void f(T param) // declaration equivalent to
    {}

    template<typename T>
    void f(std::initializer_list<T> initList)
    {}

    void main()
    {
        auto x1 = 27; // type is int, value is 27
        auto x2(27); // ditto
        auto x3 = { 27 }; // type is std::initializer_list<int>,
        // value is { 27 }
        auto x4{ 27 }; // ditto

        auto x = { 11, 23, 9 }; // x's type is
        // std::initializer_list<int>
        // x's declaration
        // f({ 11, 23, 9 }); // error! can't deduce type for T

        f({ 11, 23, 9 });   // T deduced as int, and initList's
                            // type is std::initializer_list<int>

        int a = 3;
        const int& b = a;
        auto c = b;
    }
}

// decltype deduction
namespace Item_3{

    template<typename Container, typename Index> // works, but
    auto authAndAccess(Container& c, Index i) // requires
        -> decltype(c[i]) // refinement
    {
        authenticateUser();
        return c[i];
    }

    void main()
    {
        vector<int> v(10,2);
        decltype(v[0])  a=v[0];     // a is int&

        vector<bool> v1;
        decltype(v1[0]) b = v1[0];  // b is bool
    }
}
void morderncpp_main()
{
    Item_2::main();
}
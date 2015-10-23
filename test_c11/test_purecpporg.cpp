#include <vector>
#include <iostream>
using namespace std;

//可变模板参数类作为友元类
template<typename... Args> class A;
class B{
private:
    int b = 1;
    template<typename... Args> friend class A;
};

template<typename... Args>
class A{
public:
    void fun(B b){ cout << b.b << endl; }
};

int vardic_main() {
    // your code goes here
    A<int> a;
    B b;
    a.fun(b);
    return 0;
}

//C++多返回值
template <typename T, typename ... Others>
struct _mr : _mr < Others... >
{
    T& value;
    typedef _mr<Others...> base_type;
    _mr(T& t, base_type& father)
        : value(t),
        base_type(father.value, static_cast<typename base_type::base_type&>(father))
    {
    }

    template <typename Arg>
    _mr<Arg, T, Others...> operator ,(Arg& arg) {
        return _mr<Arg, T, Others...>(arg, *this);
    }

    template <typename Container>
    void operator=(Container && c) {
        auto it = c.begin();
        std::advance(it, sizeof...(Others));
        set(it);
    }

    template <typename Iterator>
    void set(Iterator& it) {
        value = *it--;
        base_type::set(it);
    }
};

template <typename T>
struct _mr < T >
{
    T& value;
    typedef _mr<T> base_type;
    _mr(T& t) : value(t) {}

    _mr(T& t, base_type&) : value(t) {}

    template <typename Arg>
    _mr<Arg, T> operator ,(Arg& arg) {
        return _mr<Arg, T>(arg, *this);
    }

    template <typename Container>
    void operator=(Container && c) {
        auto it = c.begin();
        set(it);
    }

    template <typename Iterator>
    void set(Iterator& it) {
        value = *it;
    }
};

struct _mr0
{
    template <typename T>
    _mr<T> operator ,(T& t) {
        return _mr<T>(t);
    }
};

static _mr0 MR;

std::vector<int> foo() {
    return{ 3, 4, 5 };
}

int pure_main()
{
    //C++多返回值
    int a = 1, b = 2, c = 3;
    (MR, a, b, c) = foo();
    cout << a << b << c << endl;

    return 0;
}
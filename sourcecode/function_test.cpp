#include <iostream>
using std::cout;
using std::endl;

/*{{{*/
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <string>
#include <cstdlib>

#include <cxxabi.h>
/*}}}*/

template <class T>
std::string type_name(void)
/*{{{*/
{
    typedef typename std::remove_reference<T>::type TR;

    std::unique_ptr<char, void(*)(void*)> own
        (
        abi::__cxa_demangle(typeid(TR).name(), nullptr,
        nullptr, nullptr),
        std::free
        );

    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";

    return r;
}
/*}}}*/

#define PRINT_TYPE(a, b) cout << #a << ": "<< type_name<b>() << endl;

void f(void){}
void g(void){}

template <typename T>
void f(void)
{
    PRINT_TYPE(f, T);
}

template <typename T>
void ff(void)
{
    PRINT_TYPE(f, T);
}

/* template <typename T> */
// void ff(T t)
// {
    // PRINT_TYPE(f_mark3, T);
/* } */

template <typename R, typename F, typename P>
void f(void)
{
}

template <void T(void)>
void f(void)
{
    decltype(T) a;
    PRINT_TYPE(f_mark1, decltype(T));
}

template <typename T = void (void)>
void f1(void)
{
    PRINT_TYPE(f_mark2, T);
}

template <typename T>
void f1_1(void)
{
    PRINT_TYPE(f_mark1_1, T);
}

template <typename T = void (*)(void)>
void f2(void)
{
}

// template <typename T = void ()(void)>
// void f22(void)
// {
// }

template <typename T = void (&)(void)>
void f3(void)
{
}

template <typename T = void (&&)(void)>
void f4(void)
{
}

template <void T(void) = f>
void f5(void)
{
}

template <void T(void) = &f>
void f6(void)
{
}

template <void (void) = f>
void f7(void)
{
}

template <void (void)>
void f8(void)
{
}

template <void (void) = &f>
void f9(void)
{
}

template <void (*)(void) = &f>
void f10(void)
{
}

template <void (*T)(void) = &f>
void f101(void)
{
}

template <void (&)(void) = f>
void f10(void)
{
}

template <void (&T)(void) = f>
void f101(void)
{
}

template <void (&T)(void) = &f>
void f102(void)
{
}

template <void (&&T)(void) = &f>
void f102(void)
{
}

template <void (&&T)(void) = f>
void f103(void)
{
}

// template <void (T&)(void) = f>
// void f101(void)
// {
// }

template <void (*)(void) = f>
void f11(void)
{
}

template <void (&)(void) = &f>
void f11(void)
{
}

template <void (&)(void) = &f>
void f12(void)
{
}

template <void (&&)(void) = &f>
void f12(void)
{
}

template <void (&&)(void) = f>
void f13(void)
{
}

int main(int argc, char* argv[])
{
{
    void   (*pf)     (void);
    // void   ()        (void);
    void   (f)       (void);
    void   (*)       (void);
    void   (&)       (void);
    void   (&&)      (void);
}
{
    void   (*pf)     (void)    =   f;
    void   (*)       (void)    =   f;

    void   (&flr)    (void)    =   f;
    void   (&&frr)   (void)    =   f;
}
// {
    // void   (flr&)    (void)    =   f;
    // void   (frr&&)   (void)    =   f;
// }
// {
    // void   (&flr)    (void);
    // void   (&&frr)   (void);
// }
{
    void   (&)       (void)    =   f;
    void   (&&)      (void)    =   f;

    void   (*pf)     (void)    =   f;
    void   (*)       (void)    =   f;
}
{
    void   (*pf)     (void)    =   &f;
    void   (*)       (void)    =   &f;
    // void   (&flr)    (void)    =   &f;
    // void   (&&frr)   (void)    =   &f;
    void   (&)       (void)    =   &f;
    void   (&&)      (void)    =   &f;
}

    f<f>();
    // f1<>();
    f1();
    f1_1<decltype(g)>();
    // f1<f>();
    // ff<f>();
    // ff<&f>();
    // ff(f);
    // ff(&g);
    ff<decltype(g)>();

    PRINT_TYPE("1", decltype(g));
    PRINT_TYPE("2", decltype(&g));
    decltype(g)& p = g;
    PRINT_TYPE("3", decltype(p));
    PRINT_TYPE("4", void(&)(void));

    decltype(g)&& p2 = g;
    PRINT_TYPE("5", decltype(p2));
    // void(void)& p1 = g;

    return 0;
}

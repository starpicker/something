// #define   TEST_OB
// #define   TEST_OO
// #define   TEST_CRTP
// #define   TEST_POLICY
// #define   TEST_VARIADIC
#define   TEST_DELEGATE
// #define   TEST_VARIADIC_WRAP
// #define   TEST_VARIADIC_WRAP_SPECIAL
// #define   TEST_VARIADIC_WRAP_SPECIAL_RETURN

#include "crtp.h"
#include "ob.h"
#include "oo.h"
#include "policy.h"
#include "variadic.h"
#include "delegate.h"
#include "variadic_wrap.h"
#include "variadic_wrap_special.h"
#include "variadic_wrap_special_return.h"

int main(int argc, char* argv[])
{
#ifdef TEST_OB
{
        cout << "test ob" << endl;
        using namespace ml_ob;
        {
            A1 a1 {1};
            a1.set(1);
            a1.process(nullptr);
        }
        {
            A2 a2 {2, 2};
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
        }
        cout << "************" << endl;
}
#endif // TEST_OB

#ifdef TEST_OO
{
        cout << "test oo" << endl;
        using namespace ml_oo;
        {
            A* a = new A1;
            A1::A1_P1 ap11 {1};
            A1::A1_P1 ap12 {1};
            A1::A1_P2 ap13 {nullptr};
            a->init(&ap11);
            a->set(&ap12);
            a->process(&ap13);
            delete a;
        }
        {
            A* a = new A2;
            A2::A2_P1 ap21 {2, 2};
            A2::A2_P1 ap22 {2, 2};
            A2::A2_P2 ap23 {nullptr, nullptr};
            a->init(&ap21);
            a->set(&ap22);
            a->process(&ap23);
            delete a;
        }

        cout << "************" << endl;
}
#endif // TEST_OO

#ifdef TEST_CRTP
{
        cout << "test crtp" << endl;
        using namespace ml_crtp;
        {
            A<A1>* a = new A1;
            A1::A1_P1 ap11 {1};
            A1::A1_P1 ap12 {1};
            A1::A1_P2 ap13 {nullptr};
            a->init(&ap11);
            a->set(&ap12);
            a->process(&ap13);
            delete a;
        }

        {
            A<A2>* a = new A2;
            A2::A2_P1 ap21 {2, 2};
            A2::A2_P1 ap22 {2, 2};
            A2::A2_P2 ap23 {nullptr, nullptr};
            a->init(&ap21);
            a->set(&ap22);
            a->process(&ap23);
            delete a;
        }

        cout << "************" << endl;
}
#endif // TEST_CRTP

#ifdef TEST_POLICY
{
        cout << "test policy" << endl;
        using namespace ml_policy;
        {
            A<A1> a1;
            A1::A1_P1 ap11 {1};
            A1::A1_P1 ap12 {1};
            A1::A1_P2 ap13 {nullptr};
            a1.init(&ap11);
            a1.set(&ap12);
            a1.process(&ap13);
        }
        {
            A<A2> a2;
            A2::A2_P1 ap21 {2, 2};
            A2::A2_P1 ap22 {2, 2};
            A2::A2_P2 ap23 {nullptr, nullptr};
            a2.init(&ap21);
            a2.set(&ap22);
            a2.process(&ap23);
        }

        cout << "************" << endl;
}
#endif // TEST_POLICY

#ifdef TEST_VARIADIC
{
        cout << "test variadic" << endl;
        using namespace ml_variadic;
        {
            A<A1> a1;
            a1.init(1);
            a1.set(1);
            a1.process(nullptr);
            a1.unit();
        }
        {
            A<A2> a2;
            a2.init(2, 2);
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
            a2.unit();
        }

        cout << "************" << endl;
}
#endif // TEST_VARIADIC

#ifdef TEST_DELEGATE
{
        cout << "test delegate" << endl;
        using namespace ml_delegate;
        {
            A<A1> a1;
            a1.init(1);
            a1.set(1);
            a1.process(nullptr);
            a1.unit();
        }
        {
            A<A2> a2;
            a2.init(2, 2);
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
            a2.unit();
        }

        cout << "************" << endl;
}
#endif // TEST_DELEGATE

#ifdef TEST_VARIADIC_WRAP
{
        cout << "test variadic_wrap" << endl;
        using namespace ml_variadic_wrap;
        {
            A<1> a1;
            a1.init(1);
            a1.set(1);
            a1.process(nullptr);
            a1.unit();
        }
        {
            A<2> a2;
            a2.init(2, 2);
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
            a2.unit();
        }

        cout << "************" << endl;
}
#endif // TEST_VARIADIC_WRAP

#ifdef TEST_VARIADIC_WRAP_SPECIAL
{
        cout << "test variadic_wrap_special" << endl;
        using namespace ml_variadic_wrap_special;
        {
            A<1> a1;
            a1.init(1);
            a1.set(1);
            a1.process(nullptr);
            a1.unit();
            // a1.special(3);
        }
        {
            A<2> a2;
            a2.init(2, 2);
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
            a2.unit();
        }
        {
            A<3> a3;
            // a3.init(1);
            a3.special(3);
        }

        cout << "************" << endl;
}
#endif // TEST_VARIADIC_WRAP_SPECIAL

#ifdef TEST_VARIADIC_WRAP_SPECIAL_RETURN
{
        cout << "test variadic_wrap_special_return" << endl;
        using namespace ml_variadic_wrap_special_return;
        {
            A<1> a1;
            a1.init(1);
            a1.set(1);
            a1.process(nullptr);
            a1.unit();
            // a1.special(3);
        }
        {
            A<2> a2;
            a2.init(2, 2);
            a2.set(2, 2);
            a2.process(nullptr, nullptr);
            a2.unit();
        }
        {
            A<3> a3;
            // a3.init(1);
            /*Data* data;
            data = */a3.special(3);
        }
        {
            A<4> a4;
            Data* data;
            data = a4.special(4);
        }

        cout << "************" << endl;
}
#endif // TEST_VARIADIC_WRAP_SPECIAL_RETURN

    return 0;
}

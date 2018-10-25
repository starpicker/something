#define   TEST_OB
#define   TEST_OO
#define   TEST_CRTP
#define   TEST_POLICY

#include "crtp.h"
#include "ob.h"
#include "oo.h"
#include "policy.h"

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

    return 0;
}

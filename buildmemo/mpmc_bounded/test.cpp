#include "mpmc_bounded_q.h"

#include <thread>
#include <iostream>
#include <atomic>

using namespace std;

using mpmcQ = mpmc_bounded_queue<char*>;
        
mpmcQ q(1024);

std::atomic<bool>   empty;
std::atomic<bool>   full;
        
void m_work(void)
{
    char* p = new char[rand()%(1024*5)];
    //char* p = new char[1024*1024*4];
    if(!q.enqueue(move(p)))
    {
        cout << "Q full, size " << q.approx_size() << endl;
        delete[] p;
        //full = true;
        //empty = false;
    }
    
    
    if(q.approx_size() > (q.size()*2/3))
    {
        full = true;
        empty = false;
    }
}

void f_work(void)
{
    char* p;
    if(q.dequeue(p))
    {
        //cout << (void*)p << endl;
        delete[] p;
    }
    else
    {
        cout << "Q empty, size " << q.approx_size() << endl;
        //empty = true;
        //full = false;
    }
    
    if(q.approx_size() < (q.size()/3))
    {
        empty = true;
        full = false;
    }
}

using fn = void (*)(void);

static atomic<fn> g_work[19];

void work(int id)
{
    start:
    
    if(empty)
        g_work[19] = m_work;
    else
        g_work[19] = f_work;
    
    if(full)
        g_work[0] = f_work;
    else
        g_work[0] = m_work;
        
    
    fn f = g_work[id];
    
    f();
    
    this_thread::sleep_for(chrono::milliseconds(5));
    goto start;
}

int main()
{
    srand((unsigned)time(NULL));
    
    for(auto i = 0; i < 10; i++)
        g_work[i] = m_work;
    
    for(auto i = 10; i < 19; i++)
        g_work[i] = f_work;
    
    for(auto i = 0; i < 19; i++)
        thread(work, i).detach();
    
    thread([]{
        start:
        this_thread::sleep_for(chrono::seconds(5));
        cout << "Q size " << q.approx_size() << endl;
        goto start;
    }).detach();
    
    char p;
    cin >> p;
    
    return 0;
}
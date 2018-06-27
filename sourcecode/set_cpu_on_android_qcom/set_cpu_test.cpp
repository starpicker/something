#include "set_cpu.h"

#include <thread>
using std::thread;

int ffttest(int n);

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("usage: %s set_cpu[0/1]\n", argv[0]);
        
        return -1;
    }
    
    int setcpu = atoi(argv[1]);
    int count = 1024*8*3;
    int fftvalue = 1024;
    struct timeval tv1;
    struct timeval tv2;
    int64_t ts1;
    int64_t ts2;
    set_cpu* sc = nullptr;
#if 0// single thread no meaning
    if(setcpu)
        sc = new set_cpu(1000);

    gettimeofday(&tv1, NULL);
    ts1 = (int64_t)tv1.tv_sec*1000 + tv1.tv_usec/1000;
    
    for(int i = 0; i < count; i++)
        ffttest(fftvalue);

    gettimeofday(&tv2, NULL);
    ts2 = (int64_t)tv2.tv_sec*1000 + tv2.tv_usec/1000;
    
    printf("ffttest no thread setcpu is %d, cost time is: %lld\n", setcpu, ts2-ts1);
    
    delete sc;
#endif    
    if(setcpu)
        sc = new set_cpu(1000*2);

    gettimeofday(&tv1, NULL);
    ts1 = (int64_t)tv1.tv_sec*1000 + tv1.tv_usec/1000;
    
    int threadCnt = thread::hardware_concurrency();
    thread t[threadCnt];
    int perWorkCnt = count / threadCnt;
    
    for(auto i  = 0; i < threadCnt; i++)
    {
        t[i] = thread([=](int tid){
            
            for(int i = tid; i < tid + perWorkCnt; i++)
                ffttest(fftvalue);
        }, i);
    }
    
    for(auto& i:t)i.join();

    gettimeofday(&tv2, NULL);
    ts2 = (int64_t)tv2.tv_sec*1000 + tv2.tv_usec/1000;
    
    printf("ffttest with 8 threads setcpu is %d, cost time is: %lld\n", setcpu, ts2-ts1);
    
    delete sc;
    
    return 0;
}


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//精度0.0001弧度

typedef struct//复数类型
{
    float real;       //实部
    float imag;       //虚部
}complex;

#define PI 3.1415926


///////////////////////////////////////////
void conjugate_complex(int n,complex in[],complex out[]);
void c_plus(complex a,complex b,complex *c);//复数加
void c_mul(complex a,complex b,complex *c) ;//复数乘
void c_sub(complex a,complex b,complex *c); //复数减法
void c_div(complex a,complex b,complex *c); //复数除法
void fft(int N,complex f[]);//傅立叶变换 输出也存在数组f中
void c_abs(complex f[],float out[],int n);//复数数组取模

void conjugate_complex(int n,complex in[],complex out[])
{
    int i = 0;
    for(i=0;i<n;i++)
    {
        out[i].imag = -in[i].imag;
        out[i].real = in[i].real;
    }
}

void c_abs(complex f[],float out[],int n)
{
    int i = 0;
    float t;
    for(i=0;i<n;i++)
    {
        t = f[i].real * f[i].real + f[i].imag * f[i].imag;
        out[i] = sqrt(t);
    }
}


void c_plus(complex a,complex b,complex *c)
{
    c->real = a.real + b.real;
    c->imag = a.imag + b.imag;
}

void c_sub(complex a,complex b,complex *c)
{
    c->real = a.real - b.real;
    c->imag = a.imag - b.imag;
}

void c_mul(complex a,complex b,complex *c)
{
    c->real = a.real * b.real - a.imag * b.imag;
    c->imag = a.real * b.imag + a.imag * b.real;
}

void c_div(complex a,complex b,complex *c)
{
    c->real = (a.real * b.real + a.imag * b.imag)/(b.real * b.real +b.imag * b.imag);
    c->imag = (a.imag * b.real - a.real * b.imag)/(b.real * b.real +b.imag * b.imag);
}

#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

void Wn_i(int n,int i,complex *Wn,char flag)
{
    Wn->real = cos(2*PI*i/n);
    if(flag == 1)
        Wn->imag = -sin(2*PI*i/n);
    else if(flag == 0)
        Wn->imag = -sin(2*PI*i/n);
}

//傅里叶变化
void fft(int N,complex f[])
{
    complex t,wn;//中间变量
    int i,j,k,m,n,l,r,M;
    int la,lb,lc;
    /*----计算分解的级数M=log2(N)----*/
    for(i=N,M=1;(i=i/2)!=1;M++);
    /*----按照倒位序重新排列原信号----*/
    for(i=1,j=N/2;i<=N-2;i++)
    {
        if(i<j)
        {
            t=f[j];
            f[j]=f[i];
            f[i]=t;
        }
        k=N/2;
        while(k<=j)
        {
            j=j-k;
            k=k/2;
        }
        j=j+k;
    }

    /*----FFT算法----*/
    for(m=1;m<=M;m++)
    {
        la=pow(2,m); //la=2^m代表第m级每个分组所含节点数
        lb=la/2;    //lb代表第m级每个分组所含碟形单元数
        //同时它也表示每个碟形单元上下节点之间的距离
        /*----碟形运算----*/
        for(l=1;l<=lb;l++)
        {
            r=(l-1)*pow(2,M-m);
            for(n=l-1;n<N-1;n=n+la) //遍历每个分组，分组总数为N/la
            {
                lc=n+lb;  //n,lc分别代表一个碟形单元的上、下节点编号
                Wn_i(N,r,&wn,1);//wn=Wnr
                c_mul(f[lc],wn,&t);//t = f[lc] * wn复数运算
                c_sub(f[n],t,&(f[lc]));//f[lc] = f[n] - f[lc] * Wnr
                c_plus(f[n],t,&(f[n]));//f[n] = f[n] + f[lc] * Wnr
            }
        }
    }
}


int ffttest(int n)
{
    int N,i,M;
    N = n;
    
    if(N < 2)
    {
        printf("too small\n");
        return 0;
    }
    unsigned short amp[N];
    int fs = 44100;

    complex input[N];
    for(i=0;i<N;i++)
    {
        input[i].real = 7000*cos(1000*2*(double)PI*((double)i/44100) + (double)3/5*PI)
                + 200*cos(600*2*(double)PI*((double)i/44100) + (double)1/2*PI);
        input[i].imag = 0;
    }

    fft(N,input);

    //printf("得到的频谱值为:\n");
    for(i=0;i<N/2;i++)
    {
        double originamp = sqrt(input[i].real*input[i].real + input[i].imag*input[i].imag);
        if(i == 0)
        {
            amp[i] = originamp/N;
        }
        else
        {
            amp[i] = originamp/N*2;
        }
        if(amp[i] > 20)
        {
            //printf("f:%.2lf amp:%d\n",(double)i*fs/N,amp[i]);
        }
    }
    return 0;
}
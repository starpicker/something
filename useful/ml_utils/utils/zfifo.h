#ifndef ML_ZFIFO__H_
#define ML_ZFIFO__H_
// for one thread read one thread write queue

struct kfifo_lite;

class zfifo
{
    private:
        kfifo_lite* _kfifo_lite;
    public:
        zfifo();
        ~zfifo();
        bool enqueue(const unsigned char* data, unsigned int len);
        bool dequeue(unsigned char* data, unsigned int len);
        unsigned int sizeright(void);
        unsigned int sizeleft(void);
        void clear(void);
        bool get(unsigned char* data, unsigned int offset, unsigned int len);
        unsigned int  count();
};

#endif // ML_ZFIFO__H_

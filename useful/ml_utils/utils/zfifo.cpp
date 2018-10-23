#include "zfifo.h"
#include "kfifo_lite.h"

const int ZFIFO_SIZE = 0x200000*8;

zfifo::zfifo()
{
    _kfifo_lite = kfifo_lite_alloc(ZFIFO_SIZE);
}

zfifo::~zfifo()
{
    if(NULL != _kfifo_lite)
      kfifo_lite_free(_kfifo_lite);
}

bool zfifo::enqueue(const unsigned char* data, unsigned int len)
{
    int ret = 0;
    ret = __kfifo_lite_put(_kfifo_lite, data, len);
    if(ret == 0)
        return true;
    else
        return false;
}

bool zfifo::dequeue(unsigned char* data, unsigned int len)
{
    int ret = __kfifo_lite_get(_kfifo_lite, data, len);
    if(ret == 0)
        return true;
    else
        return false;
}

unsigned int zfifo::sizeright(void)
{
    return __kfifo_lite_len(_kfifo_lite);
}

unsigned int zfifo::sizeleft(void)
{
    return __kfifo_lite_len_left(_kfifo_lite);
}

void zfifo::clear(void)
{
    __kfifo_lite_reset(_kfifo_lite);
}

//only get data from kfifo, not change its out flag.
bool zfifo::get(unsigned char* data, unsigned int offset, unsigned int len)
{
    int ret = __kfifo_get(_kfifo_lite, data, offset, len);
    if(ret == 0)
        return true;
    else
        return false;
}

unsigned int zfifo::count()
{
    if(sizeright())
        return  __kfifo_lite_count(_kfifo_lite);
    else
        return 0;
}

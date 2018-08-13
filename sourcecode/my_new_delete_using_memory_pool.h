#error only work on 32bit system, 64bit please modified the uchar2Int and Int2uchar

#include <ext/mt_allocator.h>

typedef unsigned char value_type;
typedef __gnu_cxx::__mt_alloc<value_type> allocator_type;

extern allocator_type g_alloc;

inline int uchar2Int(unsigned char* b, int len)
{
    int value = 0;
    int i;

    for(i = 0; i < len; i++)
    {
        value += (*(b + i) & 0xFF) << (8 * (3 - i));
    }

    return value;
}

inline void Int2uchar(int value, unsigned char dst[4])
{
    dst[3] = (unsigned char)((value & 0x000000ff));
    dst[2] = (unsigned char)((value & 0x0000ff00) >> 8);
    dst[1] = (unsigned char)((value & 0x00ff0000) >> 16);
    dst[0] = (unsigned char)((value & 0xff000000) >> 24);
}

inline void* ml_malloc(int size)
{
    size += sizeof(int);
    allocator_type::pointer p1 = g_alloc.allocate(size);
    Int2uchar(size, (unsigned char*)p1);

    return (void*)(p1 + sizeof(int));
}

inline void ml_free(void* pointer)
{
    if(pointer == 0)
    {
        return;
    }

    allocator_type::pointer p2 = (unsigned char*)pointer - sizeof(int);
    g_alloc.deallocate(p2, uchar2Int(p2, sizeof(int)));
}

#define malloc ml_malloc
#define free   ml_free
#define MLINCLASSNEW    public: \
                                \
                                void* operator new (size_t size) \
                                { \
                                    return ml_malloc(size); \
                                } \
                                \
                                void* operator new [](size_t size) \
                                { \
                                    return operator new(size);\
                                } \
                                \
                                void operator delete (void* pointer) \
                                { \
                                    ml_free(pointer); \
                                } \
                                \
                                void operator delete[](void* pointer) \
                                { \
                                    operator delete(pointer); \
                                }

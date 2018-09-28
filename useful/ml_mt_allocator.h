#include <ext/mt_allocator.h>

typedef unsigned char value_type;
typedef __gnu_cxx::__mt_alloc<value_type> allocator_type;
typedef __gnu_cxx::__pool_base::_Tune tune_type;

inline allocator_type& get_mt_allocator(void)
{
    struct mt_allocator_helper
    {
        mt_allocator_helper()
        {
            tune_type t_opt(8, 10240, 8, 102400, 20, 10, false);

            alloc._M_set_options(t_opt);
        }

        allocator_type alloc;
    };

    static mt_allocator_helper mtah;

    return mtah.alloc;
}

template <typename CharT>
inline decltype(auto) get_mt_allocator(void)
{
    typedef CharT value_type;
    typedef __gnu_cxx::__mt_alloc<value_type> allocator_type;

    struct mt_allocator_helper
    {
        mt_allocator_helper()
        {
            tune_type t_opt(8, 10240, 8, 102400, 20, 10, false);

            alloc._M_set_options(t_opt);
        }

        allocator_type alloc;
    };

    static mt_allocator_helper mtah;

    return (mtah.alloc);
}

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

// bigger than 4G fail
inline void* ml_malloc(int size)
{
    size += sizeof(int);
    allocator_type::pointer p1 = get_mt_allocator().allocate(size);
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
    get_mt_allocator().deallocate(p2, uchar2Int(p2, sizeof(int)));
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

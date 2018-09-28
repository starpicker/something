#include <string>

#include "ml_mt_allocator.h"

template <typename T>
class mlallocator
{
public:
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T*             pointer;
    typedef const T*       const_pointer;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef T              value_type;

    mlallocator()noexcept{}
    mlallocator(const mlallocator&)noexcept{}
    template <typename U>
    mlallocator(const mlallocator<U>&)noexcept{}
    ~mlallocator()noexcept{}

    // bigger than 4G fail
    T* allocate(std::size_t num, const void* hint = 0)
    {
        return get_mt_allocator<T>().allocate(num*sizeof(T));
    }

    void deallocate(T* p, std::size_t num)
    {
        get_mt_allocator<T>().deallocate(p, num);
    }

    T* address(T& value)const
    {
        return &value;
    }

    const T* address(const T& value)const
    {
        return &value;
    }

    std::size_t max_size()const noexcept
    {
       return std::numeric_limits<std::size_t>::max()/sizeof(T);
    }

    void construct(T* p, const T& value)
    {
        ::new((void*)p)T(value);
    }

    void destroy(T* p)
    {
        p->~T();
    }

    template <typename U>
    struct rebind
    {
        typedef mlallocator<U> other;
    };
};

template <typename T1, typename T2>
bool operator == (const mlallocator<T1>&, const mlallocator<T2>&)noexcept
{
    return true;
}

template <typename T1, typename T2>
bool operator != (const mlallocator<T1>&, const mlallocator<T2>&)noexcept
{
    return false;
}

typedef std::basic_string<char, std::char_traits<char>, mlallocator<char>> mlstring;

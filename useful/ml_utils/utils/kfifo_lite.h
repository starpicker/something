#ifndef ML_KFIFO_LITE__H_
#define ML_KFIFO_LITE__H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <atomic>
#include <algorithm>

#define __u32 unsigned long
#define __u64 unsigned long long

// #define max(a,b)            (((a) > (b)) ? (a) : (b))
// #define min(a,b)            (((a) < (b)) ? (a) : (b))
#define barrier()           __asm__ __volatile__("": : :"memory")


#define MAX_KFIFO_SIZE 0x300000

struct kfifo_lite {
    unsigned char *buffer; /* the buffer holding the data */
    unsigned int size; /* the size of the allocated buffer */
    unsigned int in; /* data is added at offset (in % size) */
    unsigned int out; /* data is extracted from off. (out % size) */
    std::atomic_uint count; /* the count of the added item*/
};

/**
 * fls - find last bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs:
 * - return 32..1 to indicate bit 31..0 most significant bit set
 * - return 0 to indicate no bits set
 */
static inline int fls(int x)
{
    int position;
    if(0 != x)
    {
        int i;
        for(i = (x >> 1), position = 0; i != 0; ++position)
            i >>= 1;
    }
    else
    {
        position = -1;
    }
    return position+1;
}

/**
 * fls64 - find last bit set in a 64-bit value
 * @n: the value to search
 *
 * This is defined the same way as ffs:
 * - return 64..1 to indicate bit 63..0 most significant bit set
 * - return 0 to indicate no bits set
 */
static inline int fls64(__u64 x)
{
    __u32 h = x >> 32;
    if(h)
        return fls(h) + 32;
    return fls(x);
}

static inline unsigned fls_long(unsigned long l)
{
    if(sizeof(l) == 4)
        return fls(l);
    return fls64(l);
}

static inline unsigned long roundup_pow_of_two(unsigned long x)
{
    return 1UL << fls_long(x - 1);
}

/**
 * * kfifo_lite_alloc - allocates a new FIFO and its internal buffer
 * * @size: the size of the internal buffer to be allocated.
 * * @gfp_mask: get_free_pages mask, passed to kmalloc()
 * * @lock: the lock to be used to protect the fifo buffer
 * *
 * * The size will be rounded-up to a power of 2.
 * */
inline struct kfifo_lite *kfifo_lite_alloc(unsigned int size)
{
    unsigned char *buffer;
    struct kfifo_lite *fifo;

    /*
     *       * round up to the next power of 2, since our 'let the indices
     *            * wrap' tachnique works only in this case.
     *                 */
    if(size & (size - 1))
    {
        if(size > 0x80000000)
            return NULL;
        size = roundup_pow_of_two(size);
    }

    buffer = (unsigned char *)malloc(size);
    if(!buffer)
        return NULL;

    fifo = (struct kfifo_lite*)malloc(sizeof(struct kfifo_lite));

    if(!fifo)
    {
        free(buffer);
        return NULL;
    }

    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = fifo->count = 0;

    return fifo;
}

/**
 * * kfifo_lite_free - frees the FIFO
 * * @fifo: the fifo to be freed.
 * */
inline void kfifo_lite_free(struct kfifo_lite* fifo)
{
    free(fifo->buffer);
    free(fifo);
}

/**
* __kfifo_lite_put - puts some data into the FIFO, no locking version
* @fifo: the fifo to be used.
* @buffer: the data to be added.
* @len: the length of the data to be added.
*
* This function copies at @len bytes from the @buffer into
* the FIFO depending on the free space, and return 0 when sucessed,
* else return -1.
*
* Note that with only one concurrent reader and one concurrent
* writer, you don't need extra locking to use these functions.
*/
inline unsigned int __kfifo_lite_put(struct kfifo_lite* fifo,
                        const unsigned char* buffer, unsigned int len)
{
    unsigned int l;

    if((fifo->size - fifo->in + fifo->out) < len) return -1;

    /*
    * Ensure that we sample the fifo->out index -before- we
    * start putting bytes into the kfifo.
    */

    barrier();

    /* first put the data starting from fifo->in to buffer end */
    l = std::min(len, fifo->size - (fifo->in & (fifo->size - 1)));
    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);

    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(fifo->buffer, buffer + l, len - l);

    /*
    * Ensure that we add the bytes to the kfifo -before-
    * we update the fifo->in index.
    */

    barrier();

    fifo->in += len;
    fifo->count++;

    return 0;
}

/**
* __kfifo_lite_get - gets some data from the FIFO, no locking version
* @fifo: the fifo to be used.
* @buffer: where the data must be copied.
* @len: the size of the destination buffer.
*
* This function copies at @len bytes from the FIFO into the
* @buffer and return 0 when sucessed, else return -1.
*
* Note that with only one concurrent reader and one concurrent
* writer, you don't need extra locking to use these functions.
*/
inline unsigned int __kfifo_lite_get(struct kfifo_lite* fifo,
                         unsigned char* buffer, unsigned int len)
{
    unsigned int l;

    if((fifo->in - fifo->out) < len) return -1;

    /*
    * Ensure that we sample the fifo->in index -before- we
    * start removing bytes from the kfifo.
    */

    barrier();

    /* first get the data from fifo->out until the end of the buffer */
    l = std::min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy( buffer + l, fifo->buffer, len - l);

    /*
    * Ensure that we remove the bytes from the kfifo -before-
    * we update the fifo->out index.
    */

    barrier();

    fifo->out += len;
    fifo->count--;

    return 0;
}

/**
* __kfifo_lite_reset - removes the entire FIFO contents, no locking version
* @fifo: the fifo to be emptied.
*/
static inline void __kfifo_lite_reset(struct kfifo_lite* fifo)
{
    fifo->in = fifo->out = fifo->count = 0;
}

/**
* __kfifo_lite_len - returns the number of bytes available in the FIFO, no locking version
* @fifo: the fifo to be used.
*/
static inline unsigned int __kfifo_lite_len(struct kfifo_lite* fifo)
{
    return fifo->in - fifo->out;
}

static inline bool __kfifo_reach_wrap(struct kfifo_lite* fifo)
{
    return (fifo->in < fifo->out);
}

/**
* __kfifo_lite_len_left - returns the number of empty bytes space available in the FIFO, no locking version
* @fifo: the fifo to be used.
*/
static inline unsigned int __kfifo_lite_len_left(struct kfifo_lite* fifo)
{
    return fifo->size - fifo->in + fifo->out;
}

//Only get data, not change the out pointer.
inline unsigned int __kfifo_get(struct kfifo_lite* fifo,
                         unsigned char* buffer, unsigned int offset, unsigned int len)
{
    unsigned int l, tmpOut;
    tmpOut = fifo->out + offset;
    if((fifo->in - tmpOut) < len) return -1;

    /*
    * Ensure that we sample the fifo->in index -before- we
    * start removing bytes from the kfifo.
    */

    barrier();

    /* first get the data from fifo->out until the end of the buffer */
    l = std::min(len, fifo->size  - (tmpOut & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer  + (tmpOut & (fifo->size - 1)), l);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, fifo->buffer, len - l);

    /*
    * Ensure that we remove the bytes from the kfifo -before-
    * we update the fifo->out index.
    */

    barrier();

    return 0;
}

inline unsigned int __kfifo_lite_count(struct kfifo_lite* fifo)
{
    return fifo->count/2;
}
#endif // ML_KFIFO_LITE__H_

/*
 * malloc - Small malloc/realloc/free with heap checking
 *  Ported to ELKS from V7 malloc by Greg Haerr 20 Apr 2020
 *
 * Enhancements:
 * Minimum BLOCK allocate from kernel sbrk, > BLOCK allocates requested size
 * Much improved size and heap overflow handling with errno returns
 * Full heap integrity checking and reporting with debug options
 * Use near heap pointers to work with OpenWatcom large model
 * Combine free areas at heap start before allocating from free area at end of heap
 */
#include <unistd.h>
#include <errno.h>

#define DEBUG   0

/*  C storage allocator
 *  circular first-fit strategy
 *  works with noncontiguous, but monotonically linked, arena
 *  each block is preceded by a ptr to the (pointer of) 
 *  the next following block
 *  blocks are exact number of words long 
 *  aligned to the data type requirements of ALIGN
 *  pointers to blocks must have BUSY bit 0
 *  bit in ptr is 1 for busy, 0 for idle
 *  gaps in arena are merely noted as busy blocks
 *  last block of arena (pointed to by alloct) is empty and
 *  has a pointer to first
 *  idle blocks are coalesced during space search
 *
 *  a different implementation may need to redefine
 *  ALIGN, NALIGN, BLOCK, BUSY, INT
 *  where INT is integer type to which a pointer can be cast
 */
#define INT int
#define ALIGN int
#define NALIGN 1
#define BUSY 1
#define BLOCK       34              /* min+WORD amount to sbrk (was 514) */
#define MINALLOC    14              /* minimum actual malloc size */
#define GRANULE     0               /* sbrk granularity */
#define NULL 0

union store {
    union store *ptr;
    ALIGN dummy[NALIGN];
};
typedef union store *   NPTR;
#define WORD            sizeof(union store)

#define testbusy(p)           ((INT)(p)&BUSY)
#define setbusy(p)      (NPTR)((INT)(p)|BUSY)
#define clearbusy(p)    (NPTR)((INT)(p)&~BUSY)
#define next(p)         ((p)->ptr)

#define SIZE     2
static  union store allocs[SIZE];
static  NPTR allocp;   /*search ptr*/
static  NPTR alloct;   /*arena top*/
static  NPTR allocx;   /*for benefit of realloc*/

#if DEBUG
#define ASSERT(p)   if(!(p))malloc_assert_fail(#p,__LINE__);else {}
static void malloc_assert_fail(char *s, int);
#else
#define ASSERT(p)
#endif

void *
malloc(unsigned nbytes)
{
    NPTR p, q;
    unsigned int nw, temp;

    if (allocs[0].ptr == 0) {  /*first time*/
        allocs[0].ptr = setbusy(&allocs[1]);
        allocs[SIZE-1].ptr = setbusy(&allocs[0]);
        alloct = (NPTR)&allocs[SIZE-1];
        allocp = (NPTR)&allocs[0];
    }

    errno = 0;
    if (nbytes == 0) {
        return NULL;        /* ANSI std, no error */
    }
    if (nbytes < MINALLOC)
        nbytes = MINALLOC;

    /* check INT overflow beyond 32762 (nbytes/WORD+2*WORD+(WORD-1) > 0xFFFF/WORD/WORD) */
    if (nbytes > ((unsigned)-1)/WORD-2*WORD-(WORD-1)) {
        errno = ENOMEM;
        return(NULL);
    }
    nw = (nbytes+WORD+WORD-1)/WORD;          /* extra word for link ptr/size*/

    ASSERT(allocp>=allocs && allocp<=alloct);
    ASSERT(malloc_check_heap());
    /* combine free areas at heap start before allocating from free area past allocp */
    allocp = (NPTR)allocs;
    for(p=allocp; ; ) {
        for(temp=0; ; ) {
            if(!testbusy(next(p))) {
                while(!testbusy(next(q = next(p)))) {
                    ASSERT(q>p);
                    ASSERT(q<alloct);
                    next(p) = next(q);
                }
                if(q>=p+nw && p+nw>=p)
                    goto found;
            }
            q = p;
            p = clearbusy(next(p));
            if(p>q) {
                ASSERT(p<=alloct);
            } else if(q!=alloct || p!=(NPTR)allocs) {
                ASSERT(q==alloct&&p==(NPTR)allocs);
                errno = ENOMEM;
                return(NULL);
            } else if(++temp>1)
                break;
        }

        /* extend break at least BLOCK bytes at a time, plus a word for top link */
        if (nw < BLOCK/WORD)
            temp = BLOCK/WORD + 1;
        else
            temp = nw + 1; /* NOTE always allocates full req w/o looking at free at top */

#if 0   /* not required and slow, initial break always even */
        q = (NPTR)sbrk(0);
        if((INT)q & (sizeof(union store) - 1))
            sbrk(4 - ((INT)q & (sizeof(union store) - 1)));

        /* check possible address wrap - performed in kernel */
        if(q+temp+GRANULE < q) {
            errno = ENOMEM;
            return(NULL);
        }
#endif
        q = (NPTR)sbrk((int)(temp*WORD));
        if((INT)q == -1) {
            errno = ENOMEM;
            return(NULL);
        }
        ASSERT(!((INT)q & 1));
        ASSERT(q>alloct);
        next(alloct) = q;
        if(q!=alloct+1)         /* mark any gap as permanently allocated*/
            next(alloct) = setbusy(next(alloct));
        alloct = next(q) = q+temp-1;
        next(alloct) = setbusy(allocs);
    }
found:
    allocp = p + nw;
    ASSERT(allocp<=alloct);
    if(q>allocp) {
        allocx = next(allocp);   /* save contents in case of realloc data overwrite*/
        next(allocp) = next(p);
    }
    next(p) = setbusy(allocp);
    return((void *)(p+1));
}

/*  freeing strategy tuned for LIFO allocation
 */
void
free(void *ptr)
{
    NPTR p = (NPTR)ptr;

    if (ptr == NULL)
        return;
    ASSERT(p>clearbusy(allocs[SIZE-1].ptr)&&p<=alloct);
    ASSERT(malloc_check_heap());
    allocp = --p;
    ASSERT(testbusy(next(p)));
    next(p) = clearbusy(next(p));
    ASSERT(next(p) > allocp && next(p) <= alloct);
}

#if 0
unsigned _dmalloc_usable_size(void *ptr)
{
    NPTR p = (NPTR)ptr;

    if (p == NULL)
        return 0;
    ASSERT(p>clearbusy(allocs[SIZE-1].ptr)&&p<=alloct);
    --p;
    ASSERT(testbusy(next(p)));
    return (clearbusy(next(p)) - clearbusy(p)) * sizeof(union store);
}

/*  realloc(p, nbytes) reallocates a block obtained from malloc()
 *  and freed since last call of malloc()
 *  to have new size nbytes, and old content
 *  returns new location, or 0 on failure
 */
void *
realloc(void *ptr, unsigned nbytes)
{
    NPTR p = (NPTR)ptr;
    NPTR q;
    NPTR s, t;
    unsigned int nw, onw;

    if (p == 0)
        return _dmalloc(nbytes);

    ASSERT(testbusy(next(p-1)));
    if(testbusy(next(p-1)))
        _dfree(p);
    onw = next(p-1) - p;
    q = (NPTR)_dmalloc(nbytes);   // FIXME and also use memcpy
    if(q==NULL || q==p)
        return((void *)q);

    /* copy old data into new allocation*/
    s = p;
    t = q;
    nw = (nbytes+WORD-1)/WORD;
    if(nw<onw)
        onw = nw;
    while(onw--!=0)
        *t++ = *s++;

    /* restore old data for special case of malloc link overwrite*/
    if(q<p && q+nw>=p) {
        next(q+(q+nw-p)) = allocx;
    }
    return((void *)q);
}

#if DEBUG
static void malloc_assert_fail(char *s, int line)
{
    __dprintf("malloc assert fail: %s (line %d)\n", s, line);
    abort();
}
#endif
#endif

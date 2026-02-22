/* sys/types.h */

#ifndef NULL
#define NULL            ((void *) 0)
#endif

/*
 * We define various special typedefs here rather than
 * including the compiler vendor's headers, which may
 * drag in other conflicting definitions.
 */

/* <stddef.h> */
typedef unsigned int    size_t;
#define offsetof(__typ,__id) ((size_t)((char *)&(((__typ*)0)->__id) - (char *)0))

/* <sys/types.h> */
typedef signed int      ssize_t;

/* <stdint.h> */
typedef int             intptr_t;
typedef unsigned int    uintptr_t;

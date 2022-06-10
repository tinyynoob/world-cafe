#ifndef __FFS_H
#define __FFS_H

#include <stdint.h>
#include "ifs.h"

#define ffs(x)                 \
    _Generic((x), uint64_t     \
             : ffs64, int64_t  \
             : ffs64, uint32_t \
             : ffs32, int32_t  \
             : ffs32, default  \
             : ffs64)(x)

/* debruijn sequence = 0x07C4ACDDu */
static const int debruijn_ffs32_table[32] = {
    1,  2,  11, 3,  12, 15, 23, 4,  31, 13, 16, 18, 20, 24, 27, 5,
    32, 10, 14, 22, 30, 17, 19, 26, 9,  21, 29, 25, 8,  28, 7,  6};


/* most significant = 32
 * least significant = 1
 * 0 = 0
 */
int ffs32(uint32_t x)
{
    return (0 - !!x) & debruijn_ffs32_table[(ifs(x) * 0x07C4ACDDu) >> 27 & 31];
}

/* debruijn sequence = 0x07EDD5E59A4E28C2u */
static const int debruijn_ffs64_table[64] = {
    64, 1,  59, 2,  60, 48, 54, 3,  61, 40, 49, 28, 55, 34, 43, 4,
    62, 52, 38, 41, 50, 19, 29, 21, 56, 31, 35, 12, 44, 15, 23, 5,
    63, 58, 47, 53, 39, 27, 33, 42, 51, 37, 18, 20, 30, 11, 14, 22,
    57, 46, 26, 32, 36, 17, 10, 13, 45, 25, 16, 9,  24, 8,  7,  6};

/* most significant = 64
 * least significant = 1
 * 0 = 0
 */
int ffs64(uint64_t x)
{
    return (0 - !!x) &
           debruijn_ffs64_table[(ifs(x) * 0x07EDD5E59A4E28C2u) >> 58 & 63];
}
#endif
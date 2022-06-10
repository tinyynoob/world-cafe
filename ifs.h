#ifndef __IFS_H
#define __IFS_H

#include <stdint.h>

#define ifs(x)                 \
    _Generic((x), uint32_t     \
             : ifs32, int32_t  \
             : ifs32, uint64_t \
             : ifs64, int64_t  \
             : ifs64, default  \
             : ifs64)(x)

// isolate first set
uint32_t ifs32(uint32_t n)
{
    return n & (-n);
}

// isolate first set
uint64_t ifs64(uint64_t n)
{
    return n & (-n);
}
#endif
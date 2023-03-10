#ifndef HELPERS_H
#define HELPERS_H
/* a=target variable, b=bit number to act upon 0-n */
#include <cstddef>
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

namespace helpers{
    template <typename T, std::size_t N>
    std::size_t size(T (&)[N])
    {
        return N;
    }
}

#endif // HELPERS_H

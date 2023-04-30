/**
 * \file helpers.h asdojosdj
 */
#ifndef HELPERS_H
#define HELPERS_H
/* a=target variable, b=bit number to act upon 0-n */
#include <cstddef>
/**
 * BIT_SET is a macro used to set a bit in an integer variable
 * \param a Variable
 * \param b Bit number (LSF)
 */
#define BIT_SET(a, b)   ((a) |= (1ULL << (b)))
/**
 * BIT_CLEAR is a macro used to clear a bit in an integer variable
 * \param a Variable
 * \param b Bit number (LSF)
 */
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
/**
 * BIT_FLIP is a macro used to flip a bit in an integer variable
 * \param a Variable
 * \param b Bit number (LSF)
 */
#define BIT_FLIP(a, b)  ((a) ^= (1ULL << (b)))
/**
 * BIT_CHECK is a macro used to get a bit in an integer variable
 * \param a Variable
 * \param b Bit number (LSF)
 * \returns Bit that was retrieved
 */
#define BIT_CHECK(a, b)                                                        \
    (!!((a) & (1ULL << (b)))) // '!!' to make sure this returns 0 or 1

namespace helpers {
    /**
     * \brief Function used to determine the size of a variable
     * \return Size of a variable in bytes i think
     */
    template <typename T, std::size_t N> std::size_t size(T (&)[N]) {
        return N;
    }
} // namespace helpers

#endif // HELPERS_H

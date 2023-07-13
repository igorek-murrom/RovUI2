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
        /**
     * @brief Reads sizeof(data) bytes from msg to data
     *
     * @tparam T Type
     * @param msg Input buffer
     * @param i Reference to "counter" variable
     * @param data Reference to the output variable
     */
    template <typename T>
    void read_bytes(const char *msg, size_t &i, T &data) {
        memcpy(&data, msg + i, sizeof(data));
        i += sizeof(data);
    }

    /**
     * @brief Writes sizeof(data) bytes from data to msg
     *
     * @tparam T Type
     * @param msg Output buffer
     * @param i Reference to "counter" variable
     * @param data Reference to the input variable
     */
    template <typename T> void write_bytes(size_t *msg, size_t &i, T &data) {
        memcpy(msg + i, &data, sizeof(data));
        i += sizeof(data);
    }
} // namespace helpers

#endif // HELPERS_H

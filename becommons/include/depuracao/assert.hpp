/** @copyright Copyright (c) 2025 Daniel Oliveira */
/**
 * @file assert.hpp
 *
 * @author Daniel O. dos Santos
 * @date 2025-04-08
 * @version 1.0
 *
 * @licence MIT License
 *
 */

#include <stdexcept>
#include <cmath>
#include <string>

#define ASSERT(condition) do { \
    if (!(condition)) { \
        throw std::runtime_error( \
            "ASSERT falhou: " #condition ", arquivo " __FILE__ ", linha " + std::to_string(__LINE__) \
        ); \
    } \
} while (0)

#define ASSERT_EQUAL(actual, expected) \
if ((actual) != (expected)) { \
    throw std::runtime_error("ASSERT_EQUAL falhou: " #actual " != " #expected); \
}

#define ASSERT_TRUE(expression) \
if (!(expression)) { \
    throw std::runtime_error("ASSERT_TRUE falhou: " #expression " nao é verdadeiro" ); \
}

#define ASSERT_APPROX_EQUAL(a, b, epsilon) \
    if (std::fabs((a) - (b)) > (epsilon)) { \
        throw std::runtime_error( \
            std::string("ASSERT_APPROX_EQUAL falhou: ") + std::to_string(a) + " != " + std::to_string(b) + \
            " (diferença maior que " + std::to_string(epsilon) + ")" + \
            " em " + __FILE__ + ":" + std::to_string(__LINE__) \
        ); \
    }

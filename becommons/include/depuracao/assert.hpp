/** @copyright 
MIT LicenseCopyright (c) 2025 Daniel Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
*/
/**
 * @file assert.hpp
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

#ifdef _DEBUG
#define ASSERT(condition) do { \
    if (!(condition)) { \
        throw std::runtime_error("ASSERT falhou: " #condition ", aqruivo " << __FILE__ << ", linha " << __LINE__ << ); \
    } \
} while (0)
#define ASSERT_EQUAL(actual, expected) \
if ((actual) != (expected)) { \
    throw std::runtime_error("ASSERT_EQUAL falhou: " #actual " != " #expected); \
}
#define ASSERT_TRUE(expression) \
if (!#expression) { \
    throw std::runtime_error("ASSERT_TRUE falhou: " #expression " nao é verdadeiro" ); \
}
#define ASSERT_APPROX_EQUAL(a, b, epsilon) \
    if (std::fabs((a) - (b)) > (epsilon)) { \
        throw std::runtime_error( \
            "ASSERT_APPROX_EQUAL falhou: " + std::to_string(a) + " != " + std::to_string(b) + \
            " (diferença maior que " + std::to_string(epsilon) + ")" + \
            " em " + __FILE__ + ":" + std::to_string(__LINE__) \
        ); \
    }
#else
#define ASSERT(condition) do { } while (0)
#define ASSERT_TRUE(expression) do { } while (0)
#define ASSERT_EQUAL(actual, expected) do { } while (0)
#define ASSERT_APPROX_EQUAL(a, b, epsilon) do { } while (0)
#endif // DEBUG
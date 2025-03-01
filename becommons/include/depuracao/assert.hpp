#ifdef _DEBUG
#define ASSERT(condition) do { \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " #condition ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
        std::abort(); \
    } \
} while (0)
#define ASSERT_EQUAL(actual, expected) \
    if ((actual) != (expected)) { \
        std::cerr << "Assertion failed: " << #actual << " == " << #expected \
                  << ", actual value: " << (actual) << ", expected value: " << (expected) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::abort(); \
    }
#else
#define ASSERT(condition) do { } while (0)
#define ASSERT_EQUAL(actual, expected) do { } while (0)
#endif // DEBUG
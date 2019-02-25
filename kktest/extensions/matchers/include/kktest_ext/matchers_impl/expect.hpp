#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_EXPECT_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_EXPECT_HPP_

#include <kktest_ext/matchers_impl/matcher.hpp>

namespace kktest {
namespace matchers {

template<
        class T,
        class M,
        class = typename std::enable_if<
                    std::is_base_of<matchers::Matcher, M>::value
                >
        >
void expect(const T& object, M matcher) {
    if (matcher.matches(object)) {
        return;
    }
    matchers::Description description;
    description << "Expected ";
    matcher.describe(&description);
    description << "\n\tGot      '";
    description << object;
    description << "'\n";
    matcher.describeMismatch(&description, object);
    fail("Expectation failed:\n\t" + description.toString());
}

}
}

#endif
#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_FUNCTIONAL_MATCHERS_HPP_

#include <functional>

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>
#include <kktest_ext/matchers_impl/detail/std_invoke_polyfill.hpp>

namespace kktest {
namespace matchers {

KKTEST_EXPORT extern detail::ThrowsAnythingMatcher throws;

template<class E>
detail::ThrowsSpecificMatcher<E> throwsA() {
    return detail::ThrowsSpecificMatcher<E>();
}

template<class F, class... Args>
std::function<void()> wrapFunc(const F& func, const Args... args) {
    return [&]() { detail::invokePolyfill(func, args...); };
}

namespace detail {

class KKTEST_EXPORT ThrowsAnythingMatcher: public Matcher {
 public:
    bool matches(Executable func);

    void describe(Description* description);

    void describeMismatch(Description* description, Executable func);
};

template<class E>
class ThrowsSpecificMatcher: public Matcher {
 public:
    bool matches(Executable func) {
        try {
            func();
            failureType = 1;
            return false;
        } catch(const E& exception) {
            failureType = 0;
            return true;
        } catch(...) {
            failureType = 2;
            return false;
        }
    }

    void describe(Description* description) {
        (*description) << "a function that throws " << typeid(E).name();
    }

    void describeMismatch(Description* description, Executable func) {
        if (failureType == 1) {
            (*description) << "a function that did not throw";
        }
        if (failureType == 2) {
            (*description) << "a function that throws something else";
        }
    }

 private:
    int failureType = -1;
};

}
}
}

#endif
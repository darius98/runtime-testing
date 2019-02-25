#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_NUMERIC_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_NUMERIC_MATCHERS_HPP_

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

KKTEST_EXPORT extern detail::IsPositiveMatcher isPositive;

KKTEST_EXPORT extern detail::IsNegativeMatcher isNegative;

KKTEST_EXPORT extern detail::IsEvenMatcher isEven;

KKTEST_EXPORT extern detail::IsOddMatcher isOdd;

KKTEST_EXPORT extern detail::IsZeroMatcher isZero;

template<class T>
detail::IsAlmostEqualMatcher<T> isAlmostEqualTo(const T& target,
                                                const double& eps) {
    return detail::IsAlmostEqualMatcher<T>(target, eps);
}

namespace detail {

class KKTEST_EXPORT IsPositiveMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object > 0;
    }

    void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "non-positive";
    }
};

class KKTEST_EXPORT IsNegativeMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object < 0;
    }

    void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "non-negative";
    }
};

class KKTEST_EXPORT IsEvenMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 0;
    }

    void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "odd";
    }
};

class KKTEST_EXPORT IsOddMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object % 2 == 1 || object % 2 == -1;
    }

    void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "even";
    }
};

class KKTEST_EXPORT IsZeroMatcher: public Matcher {
 public:
    template<class T>
    bool matches(const T& object) {
        return object == 0;
    }

    void describe(Description* description);

    template<class T>
    void describeMismatch(Description* description, const T&) {
        (*description) << "non-zero";
    }
};

template<class T>
class IsAlmostEqualMatcher: public Matcher {
 public:
    explicit IsAlmostEqualMatcher(const T& target, const double& eps):
            target(target), eps(eps) {}

    bool matches(const T& object) {
        T delta = object - target;
        return -eps < delta && delta < eps;
    }

    void describe(Description* description) {
        (*description) << "a number within " << eps << " of " << target;
    }

    void describeMismatch(Description* description, const T&) {
        (*description) << "a number not within " << eps << " of " << target;
    }

 private:
    T target;
    double eps;
};

}
}
}

#endif
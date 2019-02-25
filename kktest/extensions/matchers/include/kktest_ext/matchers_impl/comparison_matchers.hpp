#ifndef KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_COMPARISON_MATCHERS_HPP_
#define KKTEST_EXTENSIONS_MATCHERS_INCLUDE_KKTEST_EXT_MATCHERS_IMPL_COMPARISON_MATCHERS_HPP_

#include <functional>
#include <string>

#include <kktest_ext/matchers_impl/matcher.hpp>
#include <kktest_ext/matchers_impl/detail/decl.hpp>

namespace kktest {
namespace matchers {

template<class T>
detail::EqualityMatcher<T> isEqualTo(const T& object) {
    return detail::EqualityMatcher<T>(object);
}

template<class T>
detail::NonEqualityMatcher<T> isNotEqualTo(const T& object) {
    return detail::NonEqualityMatcher<T>(object);
}

template<class T>
detail::IsLessThanMatcher<T> isLessThan(const T& object) {
    return detail::IsLessThanMatcher<T>(object);
}

template<class T>
detail::IsLessThanEqualMatcher<T> isLessThanEqual(const T& object) {
    return detail::IsLessThanEqualMatcher<T>(object);
}

template<class T>
detail::IsGreaterThanMatcher<T> isGreaterThan(const T& object) {
    return detail::IsGreaterThanMatcher<T>(object);
}

template<class T>
detail::IsGreaterThanEqualMatcher<T> isGreaterThanEqual(const T& object) {
    return detail::IsGreaterThanEqualMatcher<T>(object);
}

template<class T>
detail::IdentityMatcher<T> isIdenticalTo(const T& object) {
    return detail::IdentityMatcher<T>(object);
}

namespace detail {

template<class T>
class EqualityMatcher: public Matcher {
 public:
    explicit EqualityMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& obj) {
        return obj == target;
    }

    void describe(Description* description) {
        (*description) << "'" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "not '" << target << "'";
    }

 private:
    T target;
};

template<class T>
class NonEqualityMatcher: public Matcher {
 public:
    explicit NonEqualityMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& obj) {
        return obj != target;
    }

    void describe(Description* description) {
        (*description) << "not '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "'" << target << "'";
    }

 private:
    T target;
};

template<class T>
class IsLessThanMatcher: public Matcher {
 public:
    explicit IsLessThanMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) {
        return object < target;
    }

    void describe(Description* description) {
        (*description) << "< '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << ">= '" << target << "'";
    }

 private:
    T target;
};

template<class T>
class IsLessThanEqualMatcher: public Matcher {
 public:
    explicit IsLessThanEqualMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) {
        return object <= target;
    }

    void describe(Description* description) {
        (*description) << "<= '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "> '" << target << "'";
    }

 private:
    T target;
};

template<class T>
class IsGreaterThanMatcher: public Matcher {
 public:
    explicit IsGreaterThanMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) {
        return object > target;
    }

    void describe(Description* description) {
        (*description) << "> '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "<= '" << target << "'";
    }

 private:
    T target;
};

template<class T>
class IsGreaterThanEqualMatcher: public Matcher {
 public:
    explicit IsGreaterThanEqualMatcher(const T& target): target(target) {}

    template<class O>
    bool matches(const O& object) {
        return object >= target;
    }

    void describe(Description* description) {
        (*description) << ">= '" << target << "'";
    }

    template<class O>
    void describeMismatch(Description* description, const O&) {
        (*description) << "< '" << target << "'";
    }

 private:
    T target;
};

template<class T>
class IdentityMatcher: public Matcher {
 public:
    explicit IdentityMatcher(const T& target):
            address(static_cast<const void*>(&target)) {}
    IdentityMatcher(const IdentityMatcher& other) = default;

    template<class S>
    bool matches(const S& object) {
        return &object == address;
    }

    void describe(Description* description) {
        (*description) << "variable at address '" << address << "'";
    }

    template<class S>
    void describeMismatch(Description* description, const S& object) {
        (*description) << "variable at address '" << &object << "'";
    }

 private:
    const void* address;
};

template<>
class KKTEST_EXPORT EqualityMatcher<std::string>: public Matcher {
 public:
    explicit EqualityMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator EqualityMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class EqualityMatcher<char[n]>: public EqualityMatcher<std::string> {
 public:
    using EqualityMatcher<std::string>::EqualityMatcher;
};

template<int n>
EqualityMatcher<std::string>::operator EqualityMatcher<char[n]>() const {
    return EqualityMatcher<char[n]>(target);
}

template<>
class KKTEST_EXPORT NonEqualityMatcher<std::string>: public Matcher {
 public:
    explicit NonEqualityMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator NonEqualityMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class NonEqualityMatcher<char[n]>: public NonEqualityMatcher<std::string> {
 public:
    using NonEqualityMatcher<std::string>::NonEqualityMatcher;
};

template<int n>
NonEqualityMatcher<std::string>::operator NonEqualityMatcher<char[n]>() const {
    return EqualityMatcher<char[n]>(target);
}

template<>
class KKTEST_EXPORT IsLessThanMatcher<std::string>: public Matcher {
 public:
    explicit IsLessThanMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsLessThanMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class IsLessThanMatcher<char[n]>: public IsLessThanMatcher<std::string> {
 public:
    using IsLessThanMatcher<std::string>::IsLessThanMatcher;
};

template<int n>
IsLessThanMatcher<std::string>::operator IsLessThanMatcher<char[n]>() const {
    return IsLessThanMatcher<char[n]>(target);
}

template<>
class KKTEST_EXPORT IsLessThanEqualMatcher<std::string>: public Matcher {
 public:
    explicit IsLessThanEqualMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsLessThanEqualMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class IsLessThanEqualMatcher<char[n]>:
        public IsLessThanEqualMatcher<std::string> {
 public:
    using IsLessThanEqualMatcher<std::string>::IsLessThanEqualMatcher;
};

template<int n>
IsLessThanEqualMatcher<std::string>::
        operator IsLessThanEqualMatcher<char[n]>() const {
    return IsLessThanEqualMatcher<char[n]>(target);
}

template<>
class KKTEST_EXPORT IsGreaterThanMatcher<std::string>: public Matcher {
 public:
    explicit IsGreaterThanMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsGreaterThanMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class IsGreaterThanMatcher<char[n]>: public IsGreaterThanMatcher<std::string> {
 public:
    using IsGreaterThanMatcher<std::string>::IsGreaterThanMatcher;
};

template<int n>
IsGreaterThanMatcher<std::string>::
        operator IsGreaterThanMatcher<char[n]>() const {
    return IsGreaterThanMatcher<char[n]>(target);
}

template<>
class KKTEST_EXPORT IsGreaterThanEqualMatcher<std::string>: public Matcher {
 public:
    explicit IsGreaterThanEqualMatcher(std::string target);

    bool matches(const std::string& obj);

    bool matches(const char* obj);

    void describe(Description* description);

    void describeMismatch(Description* description, const std::string& obj);

    template<int n>
    operator IsGreaterThanEqualMatcher<char[n]>() const;

 protected:
    std::string target;
};

template<int n>
class IsGreaterThanEqualMatcher<char[n]>:
        public IsGreaterThanEqualMatcher<std::string> {
 public:
    using IsGreaterThanEqualMatcher<std::string>::IsGreaterThanEqualMatcher;
};

template<int n>
IsGreaterThanEqualMatcher<std::string>::
        operator IsGreaterThanEqualMatcher<char[n]>() const {
    return IsGreaterThanEqualMatcher<char[n]>(target);
}

}

}
}

#endif
#include "functional_matchers.hpp"

using namespace std;

namespace matcher {

bool ThrowsAnythingMatcher::matches(const function<void()>& func) {
    try {
        func();
        return false;
    } catch(...) {
        return true;
    }
}

void ThrowsAnythingMatcher::describeExpectation(Description* description) {
    description->append("a function that throws");
}

void ThrowsAnythingMatcher::describeFailure(matcher::Description *description) {
    description->append("a function that did not throw");
}

void ThrowsAnythingMatcher::describeSuccess(matcher::Description *description) {
    description->append("a function that throws");
}

}

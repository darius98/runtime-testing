#include "expect.hpp"

using namespace std;
using namespace matcher;

namespace runtime_testing {

ExpectationFailed::ExpectationFailed(const string& str):
        runtime_error(str) {}

ExpectationFailed::ExpectationFailed(const ExpectationFailed& other) noexcept:
        ExpectationFailed(other.getMessage()) {}

string ExpectationFailed::getMessage() const {
    return what();
}

void checkDuringTest(const char* file, int line) {
    if (!isDuringTest()) {
        throw runtime_error(
            string(file) + ":" + to_string(line) + ": "
            "'expect' can only be called inside tests!"
        );
    }
}

void throwExpectationFailed(Description* description) {
    string stringDescription = description->toString();
    delete description;
    throw ExpectationFailed(stringDescription);
}


void __expect(const bool& exprResult,
              const char* file,
              int line,
              const string& expr) {
    checkDuringTest(file, line);
    if (exprResult) {
        return;
    }
    auto description = matcher::Description::createForExpect(file, line, expr);
    throwExpectationFailed(description);
}

}

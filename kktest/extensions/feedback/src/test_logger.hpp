#ifndef KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_
#define KKTEST_EXTENSIONS_FEEDBACK_SRC_TEST_LOGGER_HPP_

#include <map>
#include <ostream>
#include <set>
#include <utility>

#include "kktest/core/src/executed_test.hpp"

namespace kktest {
namespace feedback {

class TestLogger {
 public:
    explicit TestLogger(std::ostream& stream);

    void addTest(const ExecutedTest& test);

    void logFatalError(const std::string& errorMessage);

    void logFinalInformation();

 private:
    std::string getRecursiveGroupDescription(GroupPtr group);

    void printTestMessage(const ExecutedTest& test);

    std::ostream& stream;

    double totalTimeTicks = 0;
    int passedTests = 0;
    int failedTests = 0;
    int failedOptionalTests = 0;
    int loggedTests = 0;
};

}
}

#endif

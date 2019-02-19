#include "extensions/feedback/src/test_logger.hpp"

#include <termcolor/termcolor.hpp>

using namespace std;

namespace kktest {
namespace feedback {

TestLogger::TestLogger(ostream& _stream): stream(_stream) {}

void TestLogger::logTest(const ExecutedTest& test) {
    passedTests += test.isPassed();
    failedTests += !test.isPassed();

    failedOptionalTests += (!test.isPassed() && test.isOptional());
    printTestMessage(test);
    testsLogged += 1;
}

void TestLogger::logFinalInformation() {
    stream << "\nTests passed: "
           << termcolor::green
           << passedTests
           << termcolor::reset
           << "\nTests failed: "
           << (failedTests == failedOptionalTests
                    ? (failedTests == 0 ? termcolor::green : termcolor::yellow)
                    : termcolor::red)
           << failedTests
           << termcolor::reset;
    if (failedOptionalTests != 0) {
        stream << " ("
               << termcolor::yellow << failedOptionalTests << termcolor::reset
               << " "
               << (failedOptionalTests == 1 ? "was" : "were")
               << " optional)";
    }
    stream << "\n";
}

void TestLogger::logFatalError(const string& errorMessage) {
    stream << "\nA fatal "
           << termcolor::red
           << "error"
           << termcolor::reset
           << " occurred during execution: "
           << errorMessage
           << "\n";
}

string TestLogger::getRecursiveGroupDescription(GroupPtr group) {
    if (group == nullptr) {
        return "";
    }
    string recursive = getRecursiveGroupDescription(group->getParentGroup());
    if (group->getIndex() == 0 && group->getDescription().empty()) {
        return recursive;
    }
    return recursive + group->getDescription() + "::";
}

void TestLogger::printTestMessage(const ExecutedTest& test) {
    stream << "[";
    if (test.isPassed()) {
        stream << termcolor::green << "P" << termcolor::reset;
    } else {
        stream << termcolor::red << "F" << termcolor::reset;
    }
    stream << "] ";
    string groupDescription = getRecursiveGroupDescription(test.getGroup());
    stream << termcolor::grey
           << groupDescription
           << termcolor::reset
           << test.getDescription();
    if (!test.isPassed()) {
        stream << "\n\t";
        // TODO(darius98): This should be somewhere else (in utils maybe?)
        size_t pos = 0;
        string failure = test.getFailure();
        while ((pos = failure.find('\n', pos)) != string::npos) {
            failure.replace(pos, 1, "\n\t");
            pos += 2;
        }
        stream << termcolor::red << failure << termcolor::reset;
    }
    stream << "\n";
}

}
}

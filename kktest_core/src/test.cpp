#include "errors.hpp"
#include "group.hpp"

using namespace std;

namespace kktest {

Test::Test(const TestConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

void Test::setExecuted(double _executionTimeTicks, bool _passed, string _failureMessage) {
    if (isExecuted()) {
        throw KKTestLibraryImplementationError("Test::setExecuted called twice on the same test.");
    }
    executed = true;
    executionTimeTicks = _executionTimeTicks;
    if (executionTimeTicks > getConfig().timeTicksLimit) {
        _passed = false;
        _failureMessage = "Execution timed out.";
    }
    passed = _passed;
    failureMessage = _failureMessage;
}

const TestConfig& Test::getConfig() const {
    return config;
}

bool Test::isExecuted() const {
    return executed;
}

bool Test::isFailed() const {
    return isExecuted() && !isPassed();
}

bool Test::isPassed() const {
    return isExecuted() && passed;
}

string Test::getFailureMessage() const {
    return isFailed() ? failureMessage : "";
}

double Test::getExecutionTimeTicks() const {
    return executionTimeTicks;
}

Group* Test::getGroup() const {
    return parentGroup;
}

TestInfo Test::getTestInfo() const {
    return TestInfo{
        parentGroup->getIndex(),
        index,
        config.line,
        config.file,
        config.optional,
        config.description,
        isPassed(),
        getFailureMessage()
    };
}

}

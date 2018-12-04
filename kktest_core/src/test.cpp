#include "errors.hpp"
#include "group.hpp"
#include "test.hpp"

using kktest::interproc::Message;
using kktest::interproc::MessageReader;

namespace kktest {

interproc::Message TestExecutionInfo::toMessage() const {
    return Message::build(executionTimeTicks, passed, failureMessage);
}

TestExecutionInfo TestExecutionInfo::fromMessage(const Message& message) {
    TestExecutionInfo info{};
    MessageReader reader(message);
    reader << info.executionTimeTicks << info.passed << info.failureMessage;
    return info;
}

Test::Test(const TestConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

void Test::setExecuted(const TestExecutionInfo& _executionInfo) {
    if (isExecuted()) {
        throw KKTestLibraryImplementationError("Test::setExecuted called twice on the same test.");
    }
    executed = true;
    executionInfo = _executionInfo;
    if (executionInfo.executionTimeTicks > config.timeTicksLimit) {
        executionInfo.passed = false;
        executionInfo.failureMessage = "Execution timed out.";
    }
}

const TestConfig& Test::getConfig() const {
    return config;
}

bool Test::isExecuted() const {
    return executed;
}

bool Test::isFailed() const {
    return isExecuted() && !executionInfo.passed;
}

Group* Test::getGroup() const {
    return parentGroup;
}

TestInfo Test::getTestInfo() const {
    return TestInfo{
        parentGroup->getIndex(),
        index,
        config.optional,
        config.description,
        executionInfo.passed,
        executionInfo.failureMessage
    };
}

}  // namespace kktest

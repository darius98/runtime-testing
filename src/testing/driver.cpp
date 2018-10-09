#include <iostream>

#include <EasyFlags.hpp>

#include <matcher/matcher.hpp>

#include "driver.hpp"

using namespace easyflags;
using namespace std;

AddArgument(int, flagEnableLogging)
    .Name("enable-logging")
    .ArgumentType("bool")
    .Description("Enable STDOUT logging for this test run")
    .DefaultValue(1).ImplicitValue(1);


namespace kktest {

TestingDriver* TestingDriver::globalTestingDriver = nullptr;

TestingDriver* TestingDriver::getGlobalDriver() {
    if (globalTestingDriver == nullptr) {
        cout << "Global testing driver not defined. Cannot continue.\n";
        exit(1);
    }
    return globalTestingDriver;
}

void TestingDriver::initGlobal(const string& executorName) {
    if (globalTestingDriver != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    globalTestingDriver = new TestingDriver(executorName);
}

void TestingDriver::generateTestReport(ostream& report) {
    report << getGlobalDriver()->groupStack[0]->generateReport().stringify(0);
}

int TestingDriver::destroyGlobal() {
    TestingDriver* driver = getGlobalDriver();
    int status = driver->getNumFailedTests();
    delete driver;
    globalTestingDriver = nullptr;
    return status;
}

TestingDriver::TestingDriver(const string& executorName):
        shouldLog(flagEnableLogging != 0),
        executor(new Executor(executorName)),
        globalScope(new Group("", "", 0, nullptr)),
        groupStack({globalScope}) {}

TestingDriver::~TestingDriver() {
    delete globalScope;
    delete executor;
}

bool TestingDriver::isDuringTest() {
    return globalTestingDriver != nullptr &&
            globalTestingDriver->executor->isDuringTest();
}

void TestingDriver::addGroup(string description,
                             string file,
                             int line,
                             Executable func) {
    executor->checkIsInactive("group");
    Group* group = groupStack.back()->addSubGroup(move(description), move(file), line);
    groupStack.push_back(group);
    executor->executeGroup(group, func);
    groupStack.pop_back();
}

void TestingDriver::addTest(string description,
                            string file,
                            int line,
                            Executable func) {
    executor->checkIsInactive("test");
    Test* test = groupStack.back()->addTest(move(description), move(file), line);
    log(test->getFullDescription(), ": ");
    executor->execute(groupStack, test, func);
    if (test->isFailed()) {
        log("FAILED\n\t", test->getFailureMessage(), "\n");
    } else {
        log("PASSED\n");
    }
    for (Group* group: groupStack) {
        group->numTests += 1;
        group->numFailedTests += test->isFailed();
    }
    Matcher::cleanupMatchersCreatedDuringTests();
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

int TestingDriver::getNumFailedTests() {
    return globalScope->numFailedTests;
}

}

#include <fstream>
#include <iostream>

#include <EasyFlags.hpp>

#include "modules/box_module/box_executor.hpp"
#include "driver.hpp"
#include "smooth_executor.hpp"

using namespace autojson;
using namespace easyflags;
using namespace std;


namespace kktest {

string TestingDriver::getBinaryPath() {
    return getInstance()->binaryPath;
}

bool TestingDriver::isDuringTest() {
    return instance != nullptr && instance->executor->isDuringTest();
}

void TestingDriver::setExecutor(Executor* executor) {
    executor->copyHooks(getInstance()->executor);
    delete getInstance()->executor;
    getInstance()->executor = executor;
}

void TestingDriver::addBeforeTestHook(Executor::TestHook hook) {
    getInstance()->executor->addBeforeTestHook(move(hook));
}

void TestingDriver::addAfterTestHook(Executor::TestHook hook) {
    getInstance()->executor->addAfterTestHook(move(hook));
}

void TestingDriver::addBeforeGroupHook(Executor::GroupHook hook) {
    getInstance()->executor->addBeforeGroupHook(move(hook));
}

void TestingDriver::addAfterGroupHook(Executor::GroupHook hook) {
    getInstance()->executor->addAfterGroupHook(move(hook));
}

void TestingDriver::addAfterInitHook(CopyableExecutable hook) {
    getInstance()->afterInitHooks.push_back(hook);
}

void TestingDriver::addBeforeDestroyHook(CopyableExecutable hook) {
    getInstance()->beforeDestroyHooks.push_back(hook);
}

JSON TestingDriver::toJSON() {
    return getInstance()->globalScope->toJSON();
}

TestingDriver* TestingDriver::instance = nullptr;

TestingDriver* TestingDriver::getInstance() {
    if (instance == nullptr) {
        throw runtime_error("TestingDriver::getInstance() called "
                            "before TestingDriver::init.");
    }
    return instance;
}

TestingDriver* TestingDriver::init(const string &binaryPath) {
    if (instance != nullptr) {
        throw runtime_error("TestingDriver::init called a second time!");
    }
    return instance = new TestingDriver(binaryPath);
}

int TestingDriver::destroy() {
    TestingDriver* driver = getInstance();
    driver->executor->finalize();
    for (Executable hook: driver->beforeDestroyHooks) {
        hook();
    }
    int status = driver->globalScope->getNumFailedTests();
    driver->uninstallModules();
    delete driver;
    return status;
}

TestingDriver::TestingDriver(const string& _binaryPath):
        binaryPath(_binaryPath),
        globalScope(new Group("", "", 0, nullptr)),
        groupStack({globalScope}),
        executor(new SmoothExecutor()) {}

TestingDriver::~TestingDriver() {
    delete globalScope;
    delete executor;
}

void TestingDriver::addGroup(string description,
                             string file,
                             int line,
                             Executable func) {
    executor->checkIsInactive("group");
    Group* group = groupStack.back()->addSubGroup(
        move(description), move(file), line
    );
    groupStack.push_back(group);

    executor->beforeGroup(group);
    try {
        func();
    } catch(const exception& e) {
        throw runtime_error("An exception was thrown inside group '" +
            group->getFullDescription() +
            "': " +
            e.what());
    } catch(...) {
        throw runtime_error(
            "A non-exception object was thrown inside group'" +
            group->getFullDescription() + "'");
    }
    executor->afterGroup(group);

    groupStack.pop_back();
}

void TestingDriver::addTest(string description,
                            string file,
                            int line,
                            Executable func) {
    executor->checkIsInactive("test");
    Test* test = groupStack.back()->addTest(
        move(description), move(file), line
    );
    executor->executeTest(test, func);
}

void TestingDriver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(func);
}

void TestingDriver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(func);
}

}

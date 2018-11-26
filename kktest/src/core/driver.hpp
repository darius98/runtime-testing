#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <functional>
#include <map>
#include <set>
#include <vector>

#include <kktest_impl/plugin_api.hpp>
#include <core/errors.hpp>
#include <core/executor.hpp>
#include <core/group.hpp>

namespace kktest {

class TestingDriver {
private:
    static TestingDriver* getInstance();
    static TestingDriver* instance;

    static void init();
    static int destroy();
    static void forceDestroy(const ConfigurationError& error);

    static void beforeTestCase();
    static void afterTestCase();

    TestingDriver();

    ~TestingDriver();

    void installPlugins();

    void uninstallPlugins();

    template<class H>
    void addHook(H h, std::vector<H>& hs) {
        if (!allowRegisterHooks) {
            throw KKTestLibraryImplementationError("Hook added outside Plugin::install()!");
        }
        hs.emplace_back(std::move(h));
    }

    void addGroup(const GroupConfig& config, Executable func);

    void addTest(const TestConfig& config, Executable func);

    void addSetUp(Executable func, const std::string& file, int line);

    void addTearDown(Executable func, const std::string& file, int line);

    void afterTest(Test* test);
    void beforeTest(Test* test);
    void beforeGroup(Group* group);
    void afterGroup(Group* group);

    void markTestFinished(Group* group);
    void markTestStarted(Group* group);
    void markAllTestsStarted(Group* group);

    // Hooks
    std::vector<AfterInitHook> afterInitHooks;
    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;
    std::vector<BeforeDestroyHook> beforeDestroyHooks;
    std::vector<BeforeForceDestroyHook> beforeForceDestroyHooks;

    std::map<Group*, int> testsInExecutionPerGroup;
    std::set<Group*> groupsWithAllTestsStarted;

    Executor* executor = nullptr;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;
    bool allowRegisterHooks = false;

    bool failedAnyNonOptionalTest = false;

friend int main();
friend class TestDefiner;
friend class GroupDefiner;
friend class SetUpDefiner;
friend class TearDownDefiner;
friend class ExpectDefiner;
friend void addBeforeTestHook(const TestHook& hook);
friend void addAfterTestHook(const TestHook& hook);
friend void addBeforeGroupHook(const GroupHook& hook);
friend void addAfterGroupHook(const GroupHook& hook);
friend void addAfterInitHook(const AfterInitHook& hook);
friend void addBeforeDestroyHook(const BeforeDestroyHook& hook);
friend void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook);
};

}

#endif

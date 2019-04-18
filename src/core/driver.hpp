#pragma once

#include <thread>

#include "disallow_copy_and_move.hpp"
#include "executor.hpp"
#include "hooks_manager.hpp"

namespace mcga::test {

class Driver {
  private:
    static inline Driver* instance = nullptr;

  public:
    static Driver* Instance();

    static void Init(Driver* driver);

    static void Clean();

    Driver() = default;

    Driver(HooksManager* hooks, Executor* executor);

    MCGA_DISALLOW_COPY_AND_MOVE(Driver);

    ~Driver() = default;

    Executor::Type getExecutorType() const;

    void addGroup(GroupConfig config, const Executable& body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    void addFailure(const std::string& failure);

    void emitWarning(const std::string& message);

  private:
    bool checkMainThreadAndInactive(const std::string& method);

    void onWarning(const Warning& warning);
    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(const GroupPtr& group);
    void afterGroup(const GroupPtr& group);

    HooksManager* hooks = nullptr;
    Executor* executor = nullptr;

    std::size_t testingThreadId
      = std::hash<std::thread::id>()(std::this_thread::get_id());
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}  // namespace mcga::test

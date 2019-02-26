#ifndef KKTEST_CORE_SRC_TEST_HPP_
#define KKTEST_CORE_SRC_TEST_HPP_

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/src/group.hpp"

namespace kktest {

/** Immutable class holding the internal representation of a test.
 *
 * This class should only be instantiated once for every call to test() from
 * the API of the library, as an upgrade to the TestConfig instance passed to
 * the test() function. Although the copy constructor is no longer explicitly
 * deleted, this should never need to be copied throughout the lifecycle of a
 * test. After execution, this is again upgraded to an ExecutedTest.
 *
 * This class privately inherits TestConfig because TestConfig exposes public
 * mutability(TestConfig#setDescription(), TestConfig#setOptional() etc.), and
 * the desired behaviour for a Test is to be immutable once instantiated. */
class Test : private TestConfig {
 public:
    /** Default constructor from a TestConfig and the metadata received from the
     * testing Driver. */
    Test(TestConfig&& config, GroupPtr group, int index);

    /** See TestConfig#description. */
    const std::string& getDescription() const;

    /** See TestConfig#optional. */
    bool isOptional() const;

    /** See TestConfig#timeTicksLimit. */
    double getTimeTicksLimit() const;

    /** See TestConfig#attempts. */
    int getNumAttempts() const;

    /** See TestConfig#passedAttemptsRequired */
    int getNumRequiredPassedAttempts() const;

    /** The group that contains this test. */
    GroupPtr getGroup() const;

    /** Globally unique, incremental index of the test. */
    int getIndex() const;

 private:
    GroupPtr group;

    int index;
};

/** Immutable class representing a Test that has finished its execution, and the
 * Executor has provided it with the execution information.
 *
 * This class should always be instantiated from a rvalue Test instance, since
 * a Test is basically _upgraded_ to an ExecutedTest, and should not be used
 * after this. */
class ExecutedTest : public Test {
 public:
    /** Simple helper structure that holds the execution information for a
     * test. */
    struct Info {
        /** Number of time ticks the execution took.
         *
         * Is `-1` by default, signaling that the number of time ticks was not
         * computable for this execution. */
        double timeTicks = -1.0;

        /** Whether the test passed or not. */
        bool passed = true;

        /** Human-readable failure message in case the test failed, or empty
         * string if the test passed. */
        std::string failure = "";

        /** Default constructor. */
        Info() = default;

        /** Shorthand constructor for a failed execution where no other
         * information except a failure message could be computed (e.g. KBS). */
        explicit Info(std::string&& failure);

        /** Shorthand method for marking the test execution as failed and saving
         * a failure message.
         *
         * Note: if this method is called multiple times, subsequent calls do
         * NOT overwrite the first failure message. */
        void fail(const std::string& failure);
    };

    /** Upgrade a Test to an ExecutedTest with the use of the execution Info
     * result for all executions.
     *
     * Since this process only happens once for a test, and since the Test
     * instance should no longer be needed after obtaining an ExecutedTest
     * instance, this constructor moves the Test. */
    ExecutedTest(Test&& test, std::vector<Info>&& executions);

    /** Check whether the execution Info marks the test as passed or failed. */
    bool isPassed() const;

    /** The number of attempts passed. */
    int getNumPassedAttempts() const;

    /** The average number of time ticks per execution (only counting executions
     * where the number of time ticks is computable). */
    double getAvgTimeTicksForExecution() const;

    /** The total number of time ticks used (only counting executions where the
     * number of time ticks is computable). */
    double getTotalTimeTicks() const;

    /** Get the error message for the last failed execution, or empty string if
     * all executions passed. */
    std::string getLastFailure() const;

    /** Get the array of executions. */
    const std::vector<Info>& getExecutions() const;

 private:
    std::vector<Info> executions;

    // Cached values.
    mutable int numPassedExecutions = -1;
    mutable double totalTimeTicks = -2; // -1=un-computable. -2=not calculated
    mutable double avgTimeTicks = -2; // -1=un-computable. -2=not calculated
};

}

#endif

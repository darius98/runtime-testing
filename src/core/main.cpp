#include "main.hpp"

#include <iostream>

#include "box_executor.hpp"
#include "driver.hpp"
#include "scan_executor.hpp"

using mcga::cli::ChoiceArgumentSpec;
using mcga::cli::NumericArgumentSpec;
using mcga::cli::Parser;
using std::cout;
using std::exit;
using std::max;
using std::size_t;
using std::vector;

namespace mcga::test {

void runTestsOnExecutor(Executor* executor, const vector<TestCase*>& tests) {
    Driver driver(executor);
    Driver::Init(&driver);
    for (TestCase* testCase: tests) {
        driver.addGroup(testCase->name, testCase->body);
    }
    Driver::Clean();
}

void runTests(int argc,
              char** argv,
              const vector<TestCase*>& tests,
              const vector<Extension*>& extensions) {
    Parser parser("Test binary. Generated by The MCGA::Test Framework.");

    parser.addHelpFlag();
    auto executorTypeArg = parser.addChoiceArgument(
      ChoiceArgumentSpec<Executor::Type>("executor")
        .setDescription("Choose the type of executor to use. A smooth "
                        "executor runs all tests in the same process, "
                        "while a boxed executor runs each test in a "
                        "separate process. Using a smooth executor "
                        "means a test killed by signal will not be "
                        "detected and will kill the whole test suite.")
        .setOptions({{"smooth", Executor::SMOOTH}, {"boxed", Executor::BOXED}})
        .setDefaultValue("boxed"));
    auto maxParallelTestsArg = parser.addNumericArgument(
      NumericArgumentSpec<size_t>("max-parallel-tests")
        .setDescription("Maximum number of tests to execute in parallel"
                        " (processes to spawn). Ignored if `executor` "
                        "type is 'smooth'.")
        .setDefaultValue(1u)
        .setImplicitValue(1u));

    for (Extension* extension: extensions) {
        extension->registerCommandLineArgs(&parser);
    }

    parser.parse(argc, argv);

    HooksManager api;
    for (Extension* extension: extensions) {
        extension->init(&api);
    }

    ScanExecutor scanner(&api);
    runTestsOnExecutor(&scanner, tests);

    switch (executorTypeArg->getValue()) {
        case Executor::SMOOTH: {
            Executor executor(&api);
            runTestsOnExecutor(&executor, tests);
            break;
        }
        case Executor::BOXED: {
            BoxExecutor executor(&api,
                                 max(maxParallelTestsArg->getValue(), 1ul));
            runTestsOnExecutor(&executor, tests);
            break;
        }
        default: {
            cout << "Unknown executor type. Aborting.";
            exit(1);
        }
    }
    api.runHooks<HooksManager::BEFORE_DESTROY>();
    for (Extension* extension: extensions) {
        extension->destroy();
    }
}

}  // namespace mcga::test

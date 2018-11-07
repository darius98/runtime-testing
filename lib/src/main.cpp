#include <vector>

#include <core/driver.hpp>
#include <core/testcase.hpp>
#include <plugins/plugins.hpp>

using namespace std;

namespace kktest {

int main(int argc, char** argv) {
    TestingDriver::init(argc, argv, getAllPlugins());
    testCase();
    return TestingDriver::destroy();
}

}

// TODO: Move this somewhere else.
int main(int argc, char** argv) {
    return kktest::main(argc, argv);
}

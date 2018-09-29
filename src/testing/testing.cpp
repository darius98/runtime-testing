#include "testing.hpp"

#include <fstream>
#include <utility>

#include "driver.hpp"
#include "expect.hpp"

using namespace std;


namespace runtime_testing {

void initializeTestingDriver(std::ostream& log) {
    TestingDriver::init(log);
}

void destroyTestingDriver() {
    TestingDriver::destroy();
}

bool isDuringTest() {
    return TestingDriver::isDuringTest();
}

void setUp(const function<void()>& func) {
    return TestingDriver::getDriver()->addSetUp(func);
}

void tearDown(const function<void()>& func) {
    return TestingDriver::getDriver()->addTearDown(func);
}

int numFailedTests() {
    return TestingDriver::getDriver()->getNumFailedTests();
}

int writeTestSuiteReport(ostream &report) {
    return TestingDriver::getDriver()->generateTestReport(report);
}

int finalizeTesting(const string& reportFileName) {
    ofstream reportFileStream(reportFileName);
    writeTestSuiteReport(reportFileStream);
    reportFileStream.close();
    int status = numFailedTests();
    destroyTestingDriver();
    return status;
}

} // namespace runtime_testing

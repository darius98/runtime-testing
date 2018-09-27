#include "group.hpp"

using namespace std;

namespace runtime_testing {

Group::Group(const std::string &description): description(description) {}

Group::~Group() {
    for (Test* test: tests) {
        delete test;
    }
    for (Group* group: subGroups) {
        delete group;
    }
}

int Group::generateTestReport(ostream& report, size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    if (!description.empty()) {
        report << prefix
               << R"("description": ")"
               << description
               << "\",\n";
    }
    report << prefix << "\"numTests\": " << numTests << ",\n";
    report << prefix << "\"numFailedTests\": " << numFailedTests << ",\n";
    report << prefix << "\"tests\": [";
    for (size_t i = 0; i < tests.size(); ++ i) {
        if (i == 0) {
            report << "\n" << prefix;
        }
        report << "  ";
        tests[i]->generateTestReport(report, spaces + 4);
        if (i + 1 != tests.size()) {
            report << ",";
        }
        report << "\n" << prefix;
    }
    report << "],\n";
    report << prefix << "\"subGroups\": [";
    for (size_t i = 0; i < subGroups.size(); ++ i) {
        if (i == 0) {
            report << "\n" << prefix;
        }
        report << "  ";
        subGroups[i]->generateTestReport(report, spaces + 4);
        if (i + 1 != subGroups.size()) {
            report << ",";
        }
        report << "\n" << prefix;
    }
    report << "]\n";
    report << string(spaces, ' ') << "}";
    return numFailedTests;
}

}

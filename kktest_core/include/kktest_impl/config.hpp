#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_H_

#include <string>


namespace kktest {

struct TestConfig {
    std::string file = "";
    int line = -1;
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;
};

struct GroupConfig {
    std::string file = "";
    int line = -1;
    std::string description = "-";
};

}

#define kkTestConfig(...) ([&]{kktest::TestConfig _;__VA_ARGS__; return _;}())
#define kkGroupConfig(...) ([&]{kktest::GroupConfig _;__VA_ARGS__; return _;}())

#endif
#ifndef KKTEST_COMMON_UTILS_SRC_TIME_HPP_
#define KKTEST_COMMON_UTILS_SRC_TIME_HPP_

#include <ctime>

namespace kktest {
namespace utils {

class ProcessStopwatch {
 public:
    explicit ProcessStopwatch(double _ms);

    bool isElapsed() const;

 private:
    timespec startTime;
    double ms;
};

class ProcessTimer {
 public:
    ProcessTimer();

    double getMsElapsed() const;

 private:
    timespec startTime;
};

void sleepForMs(int ms);

void spinForMs(double ms);

}
}

#endif
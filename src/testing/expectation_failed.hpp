#ifndef KKTEST_TESTING_EXPECTATION_FAILED_H_
#define KKTEST_TESTING_EXPECTATION_FAILED_H_

#include <stdexcept>
#include <string>


namespace kktest {

class ExpectationFailed: std::runtime_error {
public:
    explicit ExpectationFailed(const std::string& str);
    ExpectationFailed(const ExpectationFailed& other) noexcept;

    std::string getMessage() const;
};

} // namespace kktest

#endif

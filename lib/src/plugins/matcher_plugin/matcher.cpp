#include <core/driver.hpp>
#include <core/matcher.hpp>

using namespace std;


namespace kktest {

string Description::toString() const {
    return stream.str();
}

void* Matcher::operator new(size_t size) noexcept {
    void* p = malloc(size);
    if (duringTest) {
        matchersAllocatedDuringTests.insert(p);
    }
    return p;
}

void Matcher::operator delete(void* obj) noexcept {
    matchersAllocatedDuringTests.erase(obj);
    free(obj);
}

void Matcher::setDuringTest(bool _duringTest) {
    duringTest = _duringTest;
}

void Matcher::cleanupMatchersCreatedDuringTests() {
    for (void* obj: matchersAllocatedDuringTests) {
        free(obj);
    }
    matchersAllocatedDuringTests.clear();
}

set<void*> Matcher::matchersAllocatedDuringTests;
bool Matcher::duringTest;

}
#include "matcher.hpp"
#include "../testing/testing.hpp"

using namespace runtime_testing;
using namespace std;


namespace matcher {

void* BaseMatcher::operator new(size_t size) noexcept {
    void* p = malloc(size);
    if (isDuringTest()) {
        matchersAllocatedDuringTests.insert(p);
    }
    return p;
}

void BaseMatcher::operator delete(void* obj) noexcept {
    matchersAllocatedDuringTests.erase(obj);
    free(obj);
}

void BaseMatcher::cleanupMatchersCreatedDuringTests() {
    for (void* obj: matchersAllocatedDuringTests) {
        free(obj);
    }
    matchersAllocatedDuringTests.clear();
}

set<void*> BaseMatcher::matchersAllocatedDuringTests;

}
#include <kktest.hpp>

#include "kktest/extensions/matchers/include/kktest_ext/matchers_impl/comparison_matchers.hpp"

using namespace kktest;
using namespace kktest::matchers;
using namespace std;

void comparisonMatchersTest() {
    group("Equality", [&] {
        test("isEqualTo matches equal values", [&] {
            expect(isEqualTo(3).matches(3));
            expect(isEqualTo(vector<int>{1, 2}).matches(vector<int>{1, 2}));
            expect(isEqualTo(string("abc")).matches("abc"));
            expect(isEqualTo("abc").matches(string("abc")));
            expect(isEqualTo("abc").matches("abc"));
        });

        test("isEqualTo fails for different values", [&] {
            expect(!isEqualTo(3).matches(2));
            expect(!isEqualTo(2).matches(3));
            expect(!isEqualTo(vector<int>{1, 3}).matches(vector<int>{2, 3}));
        });

        test("isNotEqualTo does not match equal values", [&] {
            expect(!isNotEqualTo(3).matches(3));
            expect(!isNotEqualTo(vector<int>{1, 2}).matches(vector<int>{1, 2}));
            expect(!isNotEqualTo(string("abc")).matches("abc"));
            expect(!isNotEqualTo("abc").matches(string("abc")));
            expect(!isNotEqualTo("abc").matches("abc"));
        });

        test("isNotEqualTo matches for different values", [&] {
            expect(isNotEqualTo(3).matches(2));
            expect(isNotEqualTo(2).matches(3));
            expect(isNotEqualTo(vector<int>{1, 3}).matches(vector<int>{2, 3}));
        });
    });

    group("Identity", [&] {
        test("isIdenticalTo matches identical variables", [&] {
            int x = 4;
            expect(isIdenticalTo(x).matches(x));
        });

        test("isIdenticalTo fails for equal values", [&] {
            int x = 3, y = 3;
            expect(!isIdenticalTo(x).matches(y));
            expect(!isIdenticalTo(y).matches(x));
        });

        test("isIdenticalTo fails for different values", [&] {
            int x = 3, y = 5;
            expect(!isIdenticalTo(x).matches(y));
            expect(!isIdenticalTo(y).matches(x));
        });
    });

    group("Less than", [&] {
        test("isLessThan matches smaller values", [&] {
            expect(isLessThan(5).matches(3));
            expect(isLessThan(17).matches(-20));
            expect(isLessThan('c').matches('a'));
            expect(isLessThan(string("cadabra")).matches("abra"));
            expect(isLessThan("cadabra").matches("abra"));
            expect(isLessThan("cadabra").matches(string("abra")));
            expect(isLessThan(vector<int>{2, 1, 3})
                       .matches(vector<int>{1, 2, 3}));
        });

        test("isLessThan does not match equal values", [&] {
            expect(!isLessThan(3).matches(3));
            expect(!isLessThan(-17).matches(-17));
            expect(!isLessThan('c').matches('c'));
            expect(!isLessThan(string("cadabra")).matches("cadabra"));
            expect(!isLessThan(vector<int>{2, 1, 3})
                       .matches(vector<int>{2, 1, 3}));
        });

        test("isLessThan does not match greater values", [&] {
            expect(!isLessThan(3).matches(5));
            expect(!isLessThan(-20).matches(17));
            expect(!isLessThan('a').matches('c'));
            expect(!isLessThan(string("abra")).matches("cadabra"));
            expect(!isLessThan(vector<int>{1, 2, 3})
                       .matches(vector<int>{2, 1, 3}));
        });
    });

    group("Less than or equal to", [&] {
        test("isLessThanEqual matches smaller values", [&] {
            expect(isLessThanEqual(5).matches(3));
            expect(isLessThanEqual(17).matches(-20));
            expect(isLessThanEqual('c').matches('a'));
            expect(isLessThanEqual(string("cada")).matches("abra"));
            expect(isLessThanEqual("cada").matches("abra"));
            expect(isLessThanEqual("cada").matches(string("abra")));
            expect(isLessThanEqual(vector<int>{2, 1, 3})
                       .matches(vector<int>{1, 2, 3}));
        });

        test("isLessThanEqual matches equal values", [&] {
            expect(isLessThanEqual(3).matches(3));
            expect(isLessThanEqual(-17).matches(-17));
            expect(isLessThanEqual('c').matches('c'));
            expect(isLessThanEqual(string("cada")).matches("cada"));
            expect(isLessThanEqual(vector<int>{2, 1, 3})
                       .matches(vector<int>{2, 1, 3}));
        });

        test("isLessThanEqual does not match greater values", [&] {
            expect(!isLessThanEqual(3).matches(5));
            expect(!isLessThanEqual(-20).matches(17));
            expect(!isLessThanEqual('a').matches('c'));
            expect(!isLessThanEqual(string("abra")).matches("cada"));
            expect(!isLessThanEqual(vector<int>{1, 2, 3})
                       .matches(vector<int>{2, 1, 3}));
        });
    });

    group("Greater than", [&] {
        test("isGreaterThan does not match smaller values", [&] {
            expect(!isGreaterThan(5).matches(3));
            expect(!isGreaterThan(17).matches(-20));
            expect(!isGreaterThan('c').matches('a'));
            expect(!isGreaterThan(string("cadabra")).matches("abra"));
            expect(!isGreaterThan("cadabra").matches(string("abra")));
            expect(!isGreaterThan("cadabra").matches("abra"));
            expect(!isGreaterThan(vector<int>{2, 1, 3})
                       .matches(vector<int>{1, 2, 3}));
        });

        test("isGreaterThan does not match equal values", [&] {
            expect(!isGreaterThan(3).matches(3));
            expect(!isGreaterThan(-17).matches(-17));
            expect(!isGreaterThan('c').matches('c'));
            expect(!isGreaterThan(string("cadabra")).matches("cadabra"));
            expect(!isGreaterThan(vector<int>{2, 1, 3})
                       .matches(vector<int>{2, 1, 3}));
        });

        test("isGreaterThan matches greater values", [&] {
            expect(isGreaterThan(3).matches(5));
            expect(isGreaterThan(-20).matches(17));
            expect(isGreaterThan('a').matches('c'));
            expect(isGreaterThan(string("abra")).matches("cadabra"));
            expect(isGreaterThan(vector<int>{1, 2, 3})
                       .matches(vector<int>{2, 1, 3}));
        });
    });

    group("Greater than or equal to", [&] {
        test("isGreaterThanEqual does not match smaller values", [&] {
            expect(!isGreaterThanEqual(5).matches(3));
            expect(!isGreaterThanEqual(17).matches(-20));
            expect(!isGreaterThanEqual('c').matches('a'));
            expect(!isGreaterThanEqual(string("cada")).matches("abra"));
            expect(!isGreaterThanEqual("cada").matches(string("abra")));
            expect(!isGreaterThanEqual("cada").matches("abra"));
            expect(!isGreaterThanEqual(vector<int>{2, 1, 3})
                       .matches(vector<int>{1, 2, 3}));
        });

        test("isGreaterThanEqual matches equal values", [&] {
            expect(isGreaterThanEqual(3).matches(3));
            expect(isGreaterThanEqual(-17).matches(-17));
            expect(isGreaterThanEqual('c').matches('c'));
            expect(isGreaterThanEqual(string("cada")).matches("cada"));
            expect(isGreaterThanEqual(vector<int>{2, 1, 3})
                       .matches(vector<int>{2, 1, 3}));
        });

        test("isGreaterThanEqual matches greater values", [&] {
            expect(isGreaterThanEqual(3).matches(5));
            expect(isGreaterThanEqual(-20).matches(17));
            expect(isGreaterThanEqual('a').matches('c'));
            expect(isGreaterThanEqual(string("abra")).matches("cada"));
            expect(isGreaterThanEqual(vector<int>{1, 2, 3})
                       .matches(vector<int>{2, 1, 3}));
        });
    });
}
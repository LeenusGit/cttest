#include <cttest.h>

extern cttest::Suite suite;

struct Test1 {
    constexpr auto operator()() {
        return cttest::expectTrue(true);
    }
};

static auto test1 = suite.addTest(Test1{});

static auto func = [] {
    return cttest::expectTrue(true);
};

static auto test2 = suite.addTest(func);

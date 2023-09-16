#pragma once
#include <cttest.h>


auto combined42 = [] (auto a, auto b) { return a + b == 42; };

template <>
struct cttest::Msg<decltype(combined42)> {
    auto operator()(auto a, auto b) {
        return fmt::format("expected {} + {} = 42", a, b);
    }
};

constexpr auto ut_suite = [] {
    cttest::Suite res{"Suite2"};
    // res.addTest([]{
    //     return cttest::expect(1, cttest::GreaterThan{}, 0);
    // });
    res.add(
        cttest::expect(41, combined42, 1)
    );
    res.add(
        cttest::equal_to(42, 42)
    );
    return res;
};

static constexpr auto a = cttest::Expect<std::greater_equal<>>{};

// static auto suite2Tmp = []{
//     const auto suite = suite2();
//     suite.summary();
//     return suite;
// }();

// static_assert(suite2().passed());

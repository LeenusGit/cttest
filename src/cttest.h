#pragma once

#include "testresult.h"
#include "attributes.h"
#include "concepts.h"
#include "suite.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/color.h>

#include <utility>
#include <array>
#include <string>
#include <source_location>
#include <type_traits>
#include <string_view>
#include <algorithm>
#include <vector>

namespace cttest
{


template <typename T, typename Func>
constexpr TestResult expectUnaryOp(const T& a, Func op,
    const std::source_location location = std::source_location::current()) {

    TestResult res{op(a), location};

    if (std::is_constant_evaluated()) { return res; }
    
    // Create debug message at run time

    if constexpr (fmt::is_formattable<T>()) {

        if constexpr (hasUnaryOpMsg<Func, T>) {
            res.optMsg = op.msg(a, res);
        }
    }
    else if constexpr (hasUnaryOpMsg<Func, std::string>) {
            res.optMsg = op.msg("$a", res);
    }

    return res;
}

template <typename T, typename Func, typename U>
constexpr TestResult expectBinOp(const T& a, Func op, const U& b,
    const std::source_location location = std::source_location::current()) {

    TestResult res{op(a, b), location};

    if (std::is_constant_evaluated()) { return res; }

    // Create debug message at run time

    if constexpr (fmt::is_formattable<T>()) {

        if constexpr (hasBinOpMsg<Func, T>) {
            res.optMsg = op.msg(a, b, res);
        }
    }
    else if constexpr (hasBinOpMsg<Func, std::string>) {
            res.optMsg = op.msg("$a", "$b", res);
    }
    return res;
}

/// Helpers ///

// Unary checks

template <typename Op>
[[nodiscard]] constexpr TestResult expect(const auto& a, Op op,
    const std::source_location location = std::source_location::current()) {

    return expectUnaryOp(a, op, location);
}

template <typename T>
constexpr TestResult expectTrue(const T& a,
    const std::source_location location = std::source_location::current()) {

    return expectUnaryOp(a, True{}, location);
}
template <typename T>
constexpr TestResult expectFalse(const T& a,
    const std::source_location location = std::source_location::current()) {

    return expectUnaryOp(a, False{}, location);
}

// Binary checks

template <typename Op>
constexpr TestResult expect(const auto& a, Op op, const auto& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, op, b, location);
}

template <typename T>
constexpr TestResult expectEq(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, Equality{}, b, location);
}

template <typename T>
constexpr TestResult expectNe(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, InEquality{}, b, location);
}

template <typename T>
constexpr TestResult expectLt(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, LessThan{}, b, location);
}

template <typename T>
constexpr TestResult expectLe(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, LessThanOrEqual{}, b, location);
}

template <typename T>
constexpr TestResult expectGt(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, GreaterThan{}, b, location);
}

template <typename T>
constexpr TestResult expectGe(const T& a, const T& b,
    const std::source_location location = std::source_location::current()) {

    return expectBinOp(a, GreaterThanOrEqual{}, b, location);
}

constexpr bool testAtCompileTime(auto func) {

    constexpr TestResult compileTimeResult = func();
    return compileTimeResult.passed();
}

bool testAtRunTime(TestFunc auto func) {

    const TestResult runTimeResult = func();
    fmt::print("{}\n", cttest::format(runTimeResult));
    return runTimeResult.passed();
}

bool test(TestFunc auto func) {
    static constexpr bool ctResult = testAtCompileTime(func);
    return testAtRunTime(func) && ctResult;
}

} // namespace cttest

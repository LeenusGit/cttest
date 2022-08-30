#pragma once

#include "testresult.h"
#include "attributes.h"
#include "concepts.h"

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

constexpr void writeMsg(const std::string& msg, auto iter) {
    const auto limit = TestResult::MAX_MSG_LEN;
    if (msg.length() <= limit) {
        fmt::format_to(iter, "{}", msg);
    }
    else {
        fmt::format_to(iter,"Msg length ({}) is too long (limit is {})", msg.length(), limit);
    }
}

template <typename T, typename Func>
constexpr TestResult expectUnaryOp(const T& a, Func op,
    const std::source_location location = std::source_location::current()) {

    TestResult res{op(a), location};

    if (std::is_constant_evaluated()) { return res; }
    
    // Create debug message at run time

    if constexpr (fmt::is_formattable<T>()) {

        if constexpr (hasUnaryOpMsg<Func, T>) {
            std::string msg = op.msg(a, res);
            writeMsg(msg, res.msg.begin());
        }
        else {
            res.msg = {"No message defined"};
        }
    }
    else if constexpr (hasUnaryOpMsg<Func, std::string>) {
            std::string msg = op.msg("$a", res);
            writeMsg(msg, res.msg.begin());
    }
    else {
        res.msg = {"No message defined"};
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
            std::string msg = op.msg(a, b, res);
            writeMsg(msg, res.msg.begin());
        }
        else {
            res.msg = {"No message defined"};
        }
    }
    else if constexpr (hasBinOpMsg<Func, std::string>) {
            std::string msg = op.msg("$a", "$b", res);
            writeMsg(msg, res.msg.begin());
    }
    else {
        res.msg = {"No message defined"};
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

bool testAtRunTime(auto func) {

    const TestResult runTimeResult = func();
    print(runTimeResult);
    return runTimeResult.passed();
}

bool test(auto func) {
    static constexpr bool ctResult = testAtCompileTime(func);
    return testAtRunTime(func) && ctResult;
}


} // namespace cttest

#include "suite.h"
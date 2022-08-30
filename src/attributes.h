#pragma once

#include "testresult.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>

namespace cttest
{

struct True {
    constexpr bool operator()(bool a) { return a == true; }
    auto msg(const auto& a, const TestResult& res) {
        return fmt::format("expected {{true}}, got {{{}}}", a);
    }
};

struct False {
    constexpr bool operator()(bool a) { return !True{}(a); }
    auto msg(const auto& a, const TestResult& res) { 
        return fmt::format("expected {{false}}, got {{{}}}", a); 
    }
};

struct Equality {
    constexpr bool operator()(auto a, auto b) { return a == b; }
    auto msg(const auto& a, const auto& b, cttest::TestResult& res) {
        return fmt::format("expected equality between {{{}}} and {{{}}}", a, b);
    }
};

struct InEquality {
    constexpr bool operator()(const auto& a, const auto& b) { return !Equality{}(a, b); }
    auto msg(const auto& a, const auto& b, const cttest::TestResult& res) {
        return fmt::format("expected inequality between {{{}}} and {{{}}}", a, b);
    }
};


struct LessThan {
    constexpr bool operator()(const auto& a, const auto& b) { return a < b; }
    auto msg(const auto& a, const auto& b, cttest::TestResult& res) {
        return fmt::format("expected {{{}}} to be less than {{{}}}", a, b);
    }
};

struct GreaterThanOrEqual {
    constexpr bool operator()(const auto& a, const auto& b) { return !LessThan{}(a, b); }
    auto msg(const auto& a, const auto& b, cttest::TestResult& res) {
        return fmt::format("expected {{{}}} >= {{{}}}", a, b);
    }
};


struct GreaterThan {
    constexpr bool operator()(const auto& a, const auto& b) { return a > b; }
    auto msg(const auto& a, const auto& b, cttest::TestResult& res) {
        return fmt::format("expected {{{}}} > {{{}}}", a, b);
    }
};


struct LessThanOrEqual {
    constexpr bool operator()(const auto& a, const auto& b) { return !GreaterThan{}(a, b); }
    auto msg(const auto& a, const auto& b, cttest::TestResult& res) {
        return fmt::format("expected {{{}}} <= {{{}}}", a, b);
    }
};

// template <typename T>
// struct Contains {
//     constexpr Contains(T val) : mVal{val} {}
//     constexpr bool operator()(const auto& a) {
//         const auto res = std::ranges::find(a, mVal);
//         return res != a.end();
//     }
//     auto msg(const auto& a, cttest::TestResult& res) {
//         return fmt::format("expected {{{}}} to contain {{{}}}", a, mVal);
//     }
//     T mVal;
// };

}
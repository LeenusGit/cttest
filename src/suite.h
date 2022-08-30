#pragma once

#include "testresult.h"
#include "concepts.h"

#include <fmt/core.h>
#include <vector>
#include <algorithm>

namespace cttest
{

struct Suite {

    constexpr Suite() = default;
    constexpr Suite(const std::string& name) : name{name} {};

    constexpr void addTest(TestFunc auto func) {
        constexpr TestResult ctResult = func();
        results.push_back(ctResult);

        TestResult runTimeResult = func();
        results.back().msg = runTimeResult.msg;
    }

    void summary() const {

        fmt::print("{}:\n", name);

        if (results.empty()) {
            fmt::print("No tests defined.\n");
            return;
        }
        for (const auto & res : results) {
            print(res);
        }
    }
    constexpr bool passed() const {
        auto pred = [](const TestResult& res) { return res.passed(); };
        return std::ranges::all_of(results, pred);
    }
    constexpr bool failed() const { return !passed(); }

    std::vector<TestResult> results;
    std::string name{"Anonymous"};
};

}
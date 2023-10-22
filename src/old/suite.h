#pragma once

#include "testresult.h"
#include "concepts.h"

#include <fmt/core.h>
#include <vector>
#include <algorithm>
#include <functional>

namespace cttest
{

struct Suite {

    constexpr Suite() = default;
    constexpr Suite(const std::string& name) : name{name} {};

    constexpr auto addTest(TestFunc auto func) {
        constexpr TestResult ctResult = func();
        results.push_back(ctResult);

        if (std::is_constant_evaluated() == false) {
            TestResult runTimeResult = func();
            messages.push_back(cttest::format(runTimeResult));
            // fmt::print("{}\n", cttest::format(runTimeResult));
        }
        return ctResult;
    }

    constexpr auto add(const TestResult& res) {
        results.push_back(res);

        if (std::is_constant_evaluated() == false) {
            messages.push_back(cttest::format(res));
        }
        return res;
    }

    void print() const {

        fmt::print("{}:\n", name);

        if (results.empty()) {
            fmt::print("No tests defined.\n");
            return;
        }

        for (const auto& res: messages) {
            fmt::print("{}\n", res);
        }
    }

    constexpr bool run(bool verbose = true) const {
        if (not std::is_constant_evaluated() && verbose) {
            print();
        }
        auto pred = [&](const TestResult& res) { 
            return res.passed(); 
        };
        return std::ranges::all_of(results, pred);
    }
    constexpr bool failed() const { return !run(); }

    std::vector<TestResult> results{};
    std::vector<std::string> messages{};
    std::string name{"Anonymous"};
};

} // ns

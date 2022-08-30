#pragma once

#include <fmt/core.h>
#include <fmt/color.h>

#include <source_location>
#include <array>
#include <cstddef>

namespace cttest
{

struct TestResult {
    static constexpr size_t MAX_MSG_LEN = 200;

    constexpr TestResult () = default;
    constexpr TestResult (bool passed, std::source_location location) :
        msg{"N/A"},
        _location{location},
        _ok{passed}
    {
    }
    constexpr bool passed() const { return _ok; }
    constexpr bool failed() const { return !passed(); }

    std::array<char, MAX_MSG_LEN> msg;
    std::source_location _location;
    bool _ok = false;
};

void print(const TestResult& res) {

    const auto passedStyle = fmt::fg(fmt::color::sea_green) | fmt::emphasis::bold;
    const auto failedStyle = fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold;
    const auto locationStyle = fmt::fg(fmt::color::gold);
    const auto& location = res._location;
    const auto& msg = res.msg;

    const auto locationStr = fmt::format("{}:{}:{}", location.file_name(), location.line(), location.column());

    constexpr auto formatStr = "{} - {} in file: {} because: ({})\n";

    if (res.passed()) {

        fmt::print(formatStr, fmt::styled("PASSED", passedStyle),
            location.function_name(),
            fmt::styled(locationStr, locationStyle),
            fmt::styled(msg.data(), passedStyle)
        );
    }
    else {
        fmt::print(formatStr, fmt::styled("FAILED", failedStyle),
            location.function_name(),
            fmt::styled(locationStr, locationStyle),
            fmt::styled(msg.data(), failedStyle)
        );
    }
}


}
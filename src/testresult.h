#pragma once

#include <fmt/core.h>
#include <fmt/color.h>

#include <source_location>
#include <array>
#include <cstddef>
#include <optional>

namespace cttest
{

struct TestResult {

    constexpr TestResult () = default;
    constexpr TestResult (bool passed, std::source_location location) :
        _location{location},
        _ok{passed}
    {
    }
    constexpr bool passed() const { return _ok; }
    constexpr bool failed() const { return !passed(); }

    std::optional<std::string> optMsg;
    std::source_location _location;
    bool _ok = false;
};

static std::string format(const TestResult& res) {

    const auto passedStyle = fmt::fg(fmt::color::sea_green) | fmt::emphasis::bold;
    const auto failedStyle = fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold;
    const auto locationStyle = fmt::fg(fmt::color::gold);
    const auto& location = res._location;

    const auto locationStr = fmt::format("{}:{}:{}", location.file_name(), location.line(), location.column());
    constexpr auto formatStr = "{} - {} in file: {} because: ({})";
    const auto msg = res.optMsg.value_or("N/A");

    if (res.passed()) {
        return fmt::format(
            formatStr,
            fmt::styled("PASSED", passedStyle),
            location.function_name(),
            fmt::styled(locationStr, locationStyle),
            fmt::styled(msg, passedStyle)
        );
    }
    else {
        return fmt::format(formatStr,
            fmt::styled("FAILED", failedStyle),
            location.function_name(),
            fmt::styled(locationStr, locationStyle),
            fmt::styled(msg, failedStyle)
        );
    }
}

}
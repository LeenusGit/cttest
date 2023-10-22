#include <source_location>
#include <array>
#include <ranges>
#include <cstddef>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/compile.h>

namespace ctt {

struct result {

    using iterator = std::array<char, 64>::iterator;

    constexpr result () = default;
    constexpr result (bool passed, std::source_location location, std::string_view reason) :
        location{location},
        ok{passed}
    {
        auto end = fmt::format_to(
            buf.begin(),
            FMT_COMPILE("Failed in file: {}, line: {} because: {}"),
            location.file_name(),
            location.line(),
            reason
        );
        mSize = std::distance(buf.begin(), end);
    }

    constexpr bool passed() const { return ok; }
    constexpr bool failed() const { return !passed(); }

    std::source_location location;
    bool ok = false;
    std::array<char, 128> buf{};
    std::size_t mSize;

    constexpr auto data() const { return buf.data(); }
    constexpr auto size() const { return mSize; }
};

static constexpr auto passed = result{true, std::source_location::current(), "Passed"};

template <typename Op>
auto format_string = [] {
    if constexpr (std::same_as<Op, std::ranges::equal_to>)  { return "expected: {} == {}"; }
    if constexpr (std::same_as<Op, std::ranges::less>)      { return "expected: {} < {}"; }
    return "N/A";
};

template <typename Op>
auto msg_to = [] (auto output, auto a, auto b) {
    return fmt::format_to(
        output,
        FMT_COMPILE(format_string<Op>()),
        a, b
    );
};

auto expect = [] (auto a, auto op, auto b, const std::source_location location = std::source_location::current()) {

    std::array<char, 64> data;
    auto end = msg_to<decltype(op)>(data.begin(), a, b);
    std::string_view reason{data.begin(), end};

    bool const passed = op(a, b);
    return result{passed, location, reason};
};

[[maybe_unused]] auto expect_eq = [] (auto a, auto b, const std::source_location location = std::source_location::current()) {
    return expect(a, std::ranges::equal_to{}, b, location);
};

[[maybe_unused]] auto expect_less = [] (auto a, auto b, const std::source_location location = std::source_location::current()) {
    return expect(a, std::ranges::less{}, b, location);
};

// Parameter pack?
auto test = [] (auto func) {

#ifdef DEBUG
    auto const result = func();
    if (result.passed()) {
        return 0;
    }
    std::string_view message{result.data(), result.size()};
    fmt::println("{}", message);
    return 1;
#else
    static constexpr auto result = func();
    static_assert(result.passed(), result);
    return 0;
#endif

};

} // NS


int main() {

    constexpr auto test = [] {

        int ans = 1;
        ctt::result result{};
        
        result = ctt::expect_less(ans, 100);
        if (result.failed()) { return result; }

        result = ctt::expect_less(ans, 10);
        if (result.failed()) { return result; }
        
        return ctt::passed;
    };

    return ctt::test(test);
}

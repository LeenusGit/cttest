#include <source_location>
#include <array>
#include <ranges>
#include <cstddef>
#include <optional>
#include <functional>

#include <fmt/core.h>
#include <fmt/compile.h>
// #include <fmt/color.h>

#define CTT_CHECK(x) if (auto err = (x); err) { return err; }

namespace ctt {

struct true_t {
    constexpr bool operator()(bool in) noexcept { return in == true; }
};
struct false_t {
    constexpr bool operator()(bool in) noexcept { return in == false; }
};

template <typename Op>
auto format_string = [] {
    if constexpr (std::same_as<Op, std::ranges::equal_to>)  { return "expected: {{{}}} == {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::less>)      { return "expected: {{{}}} < {{{}}}"; }
    if constexpr (std::same_as<Op, true_t>)                 { return "expected: {{true}}, got: {{{}}}"; }
    if constexpr (std::same_as<Op, false_t>)                { return "expected: {{{false}}}, got: {{{}}}"; }
    return "N/A";
};

using location = std::source_location;

struct failure {

    constexpr failure (
        std::string_view reason,
        location location = location::current()
    ) {
        auto end = fmt::format_to(
            buf.begin(),
            FMT_COMPILE("\n\tin file:({}:{}), reason:\n\t\t{}"),
            location.file_name(),
            location.line(),
            reason
        );
        mSize = std::distance(buf.begin(), end);
    }
    std::array<char, 128> buf{};
    std::size_t mSize{};

    constexpr auto data() const { return buf.data(); }
    constexpr auto size() const { return mSize; }
};

using result = std::optional<failure>;

static constexpr auto passed = result{};

template <typename Op>
static constexpr auto msg_to(auto output, auto a, auto b) {
    return fmt::format_to(
        output,
        FMT_COMPILE(format_string<Op>()),
        a, b
    );
}

template <typename Op>
static constexpr auto msg_to(auto output, auto a) {
    return fmt::format_to(
        output,
        FMT_COMPILE(format_string<Op>()),
        a
    );
}

static constexpr auto expect(
    auto const& a,
    auto op,
    auto const& b,
    const location location = location::current()
    ) -> result {

    std::array<char, 64> data;
    auto end = msg_to<decltype(op)>(data.begin(), a, b);
    std::string_view reason{data.begin(), end};

    return op(a, b) ? std::nullopt : result{failure{reason, location}};
}

static constexpr auto expect(
    auto const& a,
    auto op,
    const location location = location::current()
    ) -> result {

    std::array<char, 64> data;
    auto end = msg_to<decltype(op)>(data.begin(), a);
    std::string_view reason{data.begin(), end};

    return op(a) ? std::nullopt : result{failure{reason, location}};
}

static constexpr auto expect_eq(auto a, auto b, const location location = location::current()) {
    return expect(a, std::ranges::equal_to{}, b, location);
}

static constexpr auto expect_less(auto a, auto b, const location location = location::current()) {
    return expect(a, std::ranges::less{}, b, location);
}

static constexpr auto expect_true(auto a,const location location = location::current()) {
    return expect(a, ctt::true_t{}, location);
}

static constexpr bool test (auto func) {

#ifdef CTT_DEBUG
    auto const err = func();
    if (not err) {
        return true;
    }
    std::string_view message{err->data(), err->size()};
    fmt::println("{}", message);
    return false;
#else
    static constexpr auto err = func();

    // static_assert(not err, err.value()); // fails because err.value() is always evaluated
    if constexpr (err) {
        static_assert(false, err.value());
    }

    return true;
#endif
}

// Run all tests
template <typename... Fs>
static constexpr int test (Fs... funcs) {

    int const test_count = sizeof...(funcs);
    int const passed = (... + test(funcs));
    return passed == test_count ? 0 : 1;
}


// Run tests until first failure, only works at runtime
template <typename... Fs>
static constexpr int test_lazy (Fs... funcs) {

    bool const all_ok = (... && test(funcs));
    return all_ok ? 0 : 1;
}

} // NS

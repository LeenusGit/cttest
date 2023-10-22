#include <source_location>
#include <array>
#include <ranges>
#include <cstddef>
#include <optional>
#include <functional>

#include <fmt/core.h>
#include <fmt/compile.h>

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
    if constexpr (std::same_as<Op, std::ranges::equal_to>)      { return "expected {{{}}} == {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::not_equal_to>)  { return "expected {{{}}} != {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::greater>)       { return "expected {{{}}} > {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::less>)          { return "expected {{{}}} < {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::greater_equal>) { return "expected {{{}}} >= {{{}}}"; }
    if constexpr (std::same_as<Op, std::ranges::less_equal>)    { return "expected {{{}}} <= {{{}}}"; }
    if constexpr (std::same_as<Op, true_t>)                     { return "expected {{true}}, got: {{{}}}"; }
    if constexpr (std::same_as<Op, false_t>)                    { return "expected {{false}}, got: {{{}}}"; }
    return "N/A";
};

using location = std::source_location;

struct failure {

    static constexpr auto max_msg_len = 128;

    constexpr failure (
        std::string_view reason,
        location location = location::current()
    ) {
        auto end = fmt::format_to(
            buf.begin(),
            FMT_COMPILE("\n\tin file:({}:{})\n\t\t{}"),
            location.file_name(),
            location.line(),
            reason
        );
        msg_len = std::distance(buf.begin(), end);
    }
    std::array<char, max_msg_len> buf{};
    std::size_t msg_len{};

    constexpr auto data() const { return buf.data(); }
    constexpr auto size() const { return msg_len; }
};

using result = std::optional<failure>;

template <typename F>
concept test_func = requires (F func) {
    { func() } -> std::convertible_to<result>;
};

static constexpr auto passed = result{};

static constexpr auto expect(
    auto const& a,
    auto op,
    auto const& b,
    location location = location::current()
    ) {

    std::vector<char> data{};
    fmt::format_to(
        std::back_inserter(data),
        FMT_COMPILE(format_string<decltype(op)>()),
        a, b
    );
    std::string_view const reason{data};
    return op(a, b) ? std::nullopt : result{failure{reason, location}};
}

static constexpr result expect(
    auto const& a,
    auto op,
    location location = location::current()
    ) {

    std::vector<char> data{};
    fmt::format_to(
        std::back_inserter(data),
        FMT_COMPILE(format_string<decltype(op)>()),
        a
    );
    std::string_view const reason{data};

    return op(a) ? std::nullopt : result{failure{reason, location}};
}

static constexpr result expect_true(auto const& a, location location = location::current()) {
    return expect(a, ctt::true_t{}, location);
}

static constexpr result expect_false(auto const& a, location location = location::current()) {
    return expect(a, ctt::false_t{}, location);
}

static constexpr result expect_eq(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::equal_to{}, b, location);
}

static constexpr result expect_ne(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::not_equal_to{}, b, location);
}

static constexpr result expect_greater(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::greater{}, b, location);
}

static constexpr result expect_less(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::less{}, b, location);
}

static constexpr result expect_greater_equal(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::greater_equal{}, b, location);
}

static constexpr result expect_less_equal(auto const& a, auto const& b, location location = location::current()) {
    return expect(a, std::ranges::less_equal{}, b, location);
}


static constexpr bool test (test_func auto func) {

#ifdef CTT_DEBUG
    static constexpr result ct_err = func();
    result const err = func();
    if (not err and not ct_err) {
        return true;
    }
    std::string_view message{err->data(), err->size()};
    fmt::println("{}", message);
    return false;
#else
    static constexpr auto err = func();

    // Can not use `static_assert(not err, err.value());` because err.value() is always evaluated
    if constexpr (err) {

#if __cpp_static_assert >= 202306L
        static_assert(not err, err.value());
#else
        static_assert(not err);
#endif
    }
    return true;
#endif
}

// Run all tests
template <typename... Fs>
static constexpr int test_all (Fs... funcs) {

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

#include <cttest.h>
#include <array>

using namespace cttest;

struct Nonformattable {
    int tmp;
    constexpr auto operator==(const Nonformattable& other) const -> bool = default;
};

struct NoMsg {
    constexpr bool operator()(Nonformattable a) { return true; }
    constexpr bool operator()(const Nonformattable& a, const Nonformattable& b) { return true; }
    constexpr bool operator()(bool a) { return true; }
    constexpr bool operator()(bool a, bool b) { return true; }
};

struct MsgNonFormattable {
    constexpr bool operator()(Nonformattable a) { return true; }
    auto msg(const auto& a, TestResult& res) {
        return fmt::format("expected {{true}}, got {{{}}}", a);
    }
};

template TestResult cttest::expectUnaryOp(const bool&, cttest::True, std::source_location);
template TestResult cttest::expectUnaryOp(const bool&, NoMsg, std::source_location);
template TestResult cttest::expectUnaryOp(const Nonformattable&, MsgNonFormattable, std::source_location);
template TestResult cttest::expectUnaryOp(const Nonformattable&, NoMsg, std::source_location);

template TestResult cttest::expectBinOp(const bool&, cttest::Equality, const bool&, std::source_location);
template TestResult cttest::expectBinOp(const bool&, NoMsg, const bool&, std::source_location);
template TestResult cttest::expectBinOp(const Nonformattable&, cttest::Equality, const Nonformattable&, std::source_location);
template TestResult cttest::expectBinOp(const Nonformattable&, NoMsg, const Nonformattable&, std::source_location);

auto func = [] { return TestResult{}; };
template bool cttest::testAtCompileTime(decltype(func));
template bool cttest::testAtRunTime(decltype(func));
template bool cttest::test(decltype(func));

template auto cttest::Suite::addTest(decltype(func));

template TestResult cttest::expect(const bool&, cttest::True, std::source_location);
template TestResult cttest::expectTrue(const bool&, std::source_location);

template TestResult cttest::expect(const bool&, cttest::Equality, const bool&, std::source_location);
template TestResult cttest::expectFalse(const bool&, std::source_location);
template TestResult cttest::expectEq(const bool&, const bool&, std::source_location);
template TestResult cttest::expectNe(const bool&, const bool&, std::source_location);
template TestResult cttest::expectLt(const bool&, const bool&, std::source_location);
template TestResult cttest::expectLe(const bool&, const bool&, std::source_location);
template TestResult cttest::expectGt(const bool&, const bool&, std::source_location);
template TestResult cttest::expectGe(const bool&, const bool&, std::source_location);

constexpr auto createEmptySuite = []{
    Suite suite{};
    return suite;
};

struct TestType {
    int value;
    constexpr bool operator==(const TestType& other) const = default;
    constexpr bool operator<(const TestType& other) const { return value < other.value;}
    constexpr bool operator>(const TestType& other) const { return value > other.value; }
};

template<>
struct fmt::formatter<TestType> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }
    template <typename FormatContext>
    auto format(const TestType& tmp, FormatContext& ctx) const {
        auto res = ctx.out();
        res = fmt::format_to(ctx.out(), "TestType({})", tmp.value);
        return res;
    }
};

constexpr auto passingTests = []{
    Suite suite{"passingTests"};
    suite.addTest([]{
        TestType a{1};
        return expectTrue(a.value == 1);
    });
    suite.addTest([]{
        TestType a{1};
        TestType b{1};
        return expectEq(a, b);
    });
    suite.addTest([]{
        TestType a{1};
        TestType b{2};
        return expectNe(a, b);
    });
    suite.addTest([]{
        TestType a{42};
        return expectEq(a, TestType{42});
    });
    return suite;
};

constexpr auto failingTests = []{
    Suite suite{"Failing Tests"};
    suite.addTest([]{
        TestType a{1};
        return expectFalse(a.value == 1);
    });
    suite.addTest([]{
        TestType a{1};
        return expectTrue(a.value == 0);
    });
    return suite;
};

struct TrueNoMsg {
    constexpr auto operator()(const auto& a) { return true; }
    constexpr auto operator()(const auto& a, const auto& b) { return true; }
};

struct TrueWithMsg {
    constexpr auto operator()(const Nonformattable& a) { return a.tmp == 42; }
    auto msg(const auto& a, const TestResult& res) -> std::string { 
        return fmt::format("expected {} to be 42", a);
    }
};

constexpr auto unaryFormatTest = []{
    Suite suite{"unaryFormatTest"};
    suite.addTest([]{
        return expect(TestType{1}, TrueNoMsg{});
    });
    suite.addTest([]{
        return expect(Nonformattable{42}, TrueWithMsg{});
    });
    suite.addTest([]{
        return expect(Nonformattable{42}, NoMsg{});
    });
    return suite;
};

constexpr auto binaryFormatTests = []{
    Suite suite{"binaryFormatTests"};
    suite.addTest([]{
        return expectEq(TestType{1}, TestType{1});
    });
    suite.addTest([]{
        return expect(TestType{1}, TrueNoMsg{}, TestType{1});
    });
    suite.addTest([]{
        return expect(Nonformattable{1}, Equality{}, Nonformattable{1});
    });
    suite.addTest([]{
        return expect(Nonformattable{1}, TrueNoMsg{}, Nonformattable{1});
    });
    return suite;
};

constexpr auto helperFunctionTests = []{
    Suite suite{"helperFunctionTests"};
    suite.addTest([]{
        return expectLt(TestType{1}, TestType{2});
    });
    suite.addTest([]{
        return expectLe(TestType{2}, TestType{2});
    });
    suite.addTest([]{
        return expectGt(TestType{2}, TestType{1});
    });
    suite.addTest([]{
        return expectGe(TestType{2}, TestType{2});
    });
    return suite;
};

int main() {

    createEmptySuite().summary();
    fmt::print("---\n");
    passingTests().summary();
    fmt::print("---\n");
    failingTests().summary();
    fmt::print("---\n");
    unaryFormatTest().summary();
    fmt::print("---\n");
    binaryFormatTests().summary();
    fmt::print("---\n");
    helperFunctionTests().summary();
    fmt::print("---\n");
    
    constexpr std::array results {
        createEmptySuite().passed(),
        passingTests().passed(),
        failingTests().failed(),
        unaryFormatTest().passed(),
        binaryFormatTests().passed(),
        helperFunctionTests().passed(),
    };
    constexpr bool cxPassed = std::ranges::find(results, false) == results.end();
    static_assert(cxPassed);

    std::vector<bool> runtimeResults {
        passingTests().passed(),
        failingTests().failed(),
        testAtCompileTime([]{ return expectEq(TestType{1}, TestType{1}); }),
        test([]{ return expectEq(TestType{1}, TestType{1}); }),
        []{ return expectEq(TestType{1}, TestType{2}); }().failed(),
    };

    runtimeResults.push_back(testAtCompileTime([]{ return expectEq(TestType{1}, TestType{1}); }));
    runtimeResults.push_back(TestResult{}.failed());
    bool rtPassed = std::ranges::find(runtimeResults, false) == runtimeResults.end();

    bool passed = cxPassed && rtPassed;

    return passed? 0 : 1;
}

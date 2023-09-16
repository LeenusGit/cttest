#include "staticmap.h"

#include <cttest.h>

#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>

template <typename key_t, typename val_t, size_t staticSize>
struct fmt::formatter<StaticMap<key_t, val_t, staticSize>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }
    template <typename FormatContext>
    auto format(const StaticMap<key_t, val_t, staticSize>& map, FormatContext& ctx) const -> decltype(ctx.out()) {

        auto res = ctx.out();
        for (size_t i = 0; i < map.data.size(); i++) {
            if (map.flags.isSet(i)) {
                res = fmt::format_to(ctx.out(), "\"{}\":[{}]", i, map.data.at(i));
            }
        }
        return res;
    }
};

struct Key {
    enum id {
        A,
        B,
        C,
    };
    static constexpr auto Count = static_cast<uint32_t>(3);
};


template struct StaticMap<Key::id, int, Key::Count>;
auto tmp = [](int& val) {};
template void StaticMap<int, int, 1>::visit(decltype(tmp));
template class bitset<10>;

struct MapFixture {
    StaticMap<Key::id, int, Key::Count> map{};
    constexpr MapFixture() {
        map.add(Key::B, 123);
    }
};

auto test1 = []{

    auto map = MapFixture{}.map;
    cttest::TestResult res;

    res = cttest::expect(map.exists(Key::A), cttest::False{});
    if (res.failed()) { return res; }
    res = cttest::expect(map.exists(Key::B), cttest::True{});
    if (res.failed()) { return res; }
    res = cttest::expect(map.exists(Key::C), cttest::False{});
    if (res.failed()) { return res; }
    
    map.add(Key::A, 42);
    res = cttest::expect(map.exists(Key::A), cttest::True{});
    if (res.failed()) { return res; }

    return res;
};

auto getSuite = [] {
    cttest::Suite suite;
    suite.addTest(test1);
    suite.addTest([]{
        auto map = MapFixture{}.map;
        map.add(Key::A, 42);
        auto map2 = MapFixture{}.map;
        map2.add(Key::A, 42);
        return cttest::expectEq(map, map2);
    });

    suite.addTest([]{
        auto map = MapFixture{}.map;
        return cttest::expectEq(map.at(Key::B), 123);
    });

    suite.addTest([]{
        auto map = MapFixture{}.map;
        map.visit([] (int& val) { val++; });
        return cttest::expectEq(map.at(Key::B), 124);
    });

    return suite;
};


int main() {

    getSuite().print();
    return (getSuite().run()) ? 0 : 1;
}
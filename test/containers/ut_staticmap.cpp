#include "staticmap.h"

#include <cttest.h>

#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>

#include <fmt/core.h>
#include <fmt/ranges.h>



int main() {

    using namespace std::literals;

    auto constexpr map2 = stc::make_map<std::string_view, int>(
        stc::pair{"hello"sv, 43},
        stc::pair{"ans"sv, 0.5}
    );

    static constexpr stc::map<std::string_view, int, 4> impl2{
        stc::pair{"hello"sv, 43},
        std::nullopt,
        std::nullopt,
        stc::pair{"ans"sv, 42},
    };


    static constexpr auto ans = map2.at("ans");
    static constexpr auto ans2 = impl2.at("ans");

    fmt::println("{}", ans.second);
    fmt::println("{}", ans2.second);

    stc::map impl3{
        stc::pair{"hello"sv, 43},
        stc::pair{"ans"sv, 42},
    };

    static constexpr stc::map impl4{
        stc::pair{"hello"sv, 43},
        stc::pair{"ans"sv, 42},
    };

    impl3.erase("hello");
    fmt::println("{}", impl3.at("ans"));
    auto const s = impl3.zero();

    fmt::println("{}", s);
    fmt::println("{}", impl3.at("ans"));

    // for (auto val : impl3.keys()) {
    //     fmt::println("{}", val);
    // }
    fmt::println("{}", impl3.keys_view());
}

// struct Key {
//     enum id {
//         A,
//         B,
//         C,
//     };
//     static constexpr auto Count = static_cast<uint32_t>(3);
// };


// template struct StaticMap<Key::id, int, Key::Count>;
// auto tmp = [](int& val) {};
// template void StaticMap<int, int, 1>::visit(decltype(tmp));
// template class bitset<10>;

// struct MapFixture {
//     StaticMap<Key::id, int, Key::Count> map{};
//     constexpr MapFixture() {
//         map.add(Key::B, 123);
//     }
// };

// auto test1 = []{

//     auto map = MapFixture{}.map;
//     cttest::TestResult res;

//     res = cttest::expect(map.exists(Key::A), cttest::False{});
//     if (res.failed()) { return res; }
//     res = cttest::expect(map.exists(Key::B), cttest::True{});
//     if (res.failed()) { return res; }
//     res = cttest::expect(map.exists(Key::C), cttest::False{});
//     if (res.failed()) { return res; }
    
//     map.add(Key::A, 42);
//     res = cttest::expect(map.exists(Key::A), cttest::True{});
//     if (res.failed()) { return res; }

//     return res;
// };

// auto getSuite = [] {
//     cttest::Suite suite;
//     suite.addTest(test1);
//     suite.addTest([]{
//         auto map = MapFixture{}.map;
//         map.add(Key::A, 42);
//         auto map2 = MapFixture{}.map;
//         map2.add(Key::A, 42);
//         return cttest::expectEq(map, map2);
//     });

//     suite.addTest([]{
//         auto map = MapFixture{}.map;
//         return cttest::expectEq(map.at(Key::B), 123);
//     });

//     suite.addTest([]{
//         auto map = MapFixture{}.map;
//         map.visit([] (int& val) { val++; });
//         return cttest::expectEq(map.at(Key::B), 124);
//     });

//     return suite;
// };


// int main() {

//     getSuite().print();
//     return (getSuite().run()) ? 0 : 1;
// }
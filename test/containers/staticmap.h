#pragma once

#include "bitset.h"

#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>
#include <concepts>

template <typename key_t, typename val_t, size_t staticSize>
struct StaticMap {

    constexpr auto add(key_t key, val_t elem) {
        size_t pos = static_cast<size_t>(key);
        flags.set(pos);
        data.at(pos) = elem;
    }

    constexpr auto remove(key_t key) {
        size_t pos = static_cast<size_t>(key);
        flags.reset(pos);
    }

    constexpr auto exists(key_t key) {
        size_t pos = static_cast<size_t>(key);
        return flags.test(pos);
    }

    constexpr auto& at(key_t key) {
        size_t pos = static_cast<size_t>(key);
        if (!flags.test(pos)) {
            throw  std::runtime_error("Access to missing element");
        }
        return data.at(pos);
    }

    constexpr auto visit(auto func) -> void {
        for (size_t i = 0; i <= data.size(); i++) {
            if (flags.test(i)) { func(data.at(i)); }
        }
    }

    constexpr auto at(key_t key) const -> const val_t& {
        size_t pos = static_cast<size_t>(key);
        if (!flags.test(pos)) {
            throw std::runtime_error{"Access to missing element"};
        }
        return data.at(pos);
    }
    
    bool operator==(const StaticMap<key_t, val_t, staticSize>& other) const = default;

    std::array<val_t, staticSize> data{};
    bitset<staticSize> flags{};
};

#include <cttest.h>
enum class ID {
    A,
    B,
    C,
};

constexpr auto ut_StaticMap = [] {
    cttest::Suite suite{"ut_StaticMap"};
    StaticMap<ID, int, 3> map;
    map.add(ID::A, 42);
    suite.add(
        cttest::expectEq(map.at(ID::A), 42)
    );
    map.add(ID::B, 100);
    suite.add(
        cttest::expectEq(map.at(ID::B), 100)
    );
    return suite;
};

constexpr auto ut_StaticMap2 = [] {
    cttest::Suite suite{"ut_StaticMap2"};
    StaticMap<ID, int, 3> map;
    map.add(ID::A, 42);
    suite.add(
        cttest::expectEq(map.at(ID::A), 42)
    );
    map.add(ID::B, 100);
    suite.add(
        cttest::expectEq(map.at(ID::B), 100)
    );
    return suite;
};

auto ut_StaticMap_ = [] {
    return (
        ut_StaticMap().run() &&
        ut_StaticMap2().run()
    );
}();

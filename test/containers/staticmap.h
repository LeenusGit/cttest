#pragma once

#include "bitset.h"

#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>
#include <concepts>
#include <stdexcept>
#include <optional>
#include <ranges>

#include <fmt/core.h>

namespace stc {
    
    template <typename T, typename U>
    using pair = std::pair<T, U>;

template <typename Key, typename T, std::size_t N>
struct map {
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::optional<stc::pair<key_type, mapped_type>>;
    using container_type = std::array<value_type, N>;

    container_type data{};

    template <typename... Args>
    constexpr map(Args&&... args)
        : data{std::forward<Args>(args)...}
    {
        std::array<key_type, N> keySlots{};
        auto const res = std::ranges::copy(keys_view(), keySlots.begin());
        std::span<key_type> keys{keySlots.begin(), res.out};

        auto const [duplicateBegin, duplicateEnd] = std::ranges::unique(keys);
        if (std::distance(duplicateBegin, duplicateEnd) > 0) {
            throw std::runtime_error{"Duplicate Keys"};
        }
    }

    constexpr auto elements_view() {
        auto res = data
            | std::ranges::views::filter([](value_type& value) {
                return value.has_value();
            })
            // | std::ranges::views::transform([](value_type& value) {
            //     return *value;
            // })
        ;
        return res;
    }

    constexpr auto zero() {
        auto elements = elements_view();
        // auto view = std::ranges::ref_view(elements);

        for (auto& e : elements) {

        }

        std::string s{"cosmos"};
        auto view = s
            | std::ranges::views::filter([](auto& c) { return c == 's'; })
            // | std::ranges::views::transform([](auto& c) { return '1'; })
        ;
        std::ranges::ref_view rv{view};

        for (auto& c : view){
            c = '0';
        }
        return s;
    }

    constexpr std::size_t max_size() const { return N; }

    constexpr std::size_t size() const {
        return std::ranges::count_if(data, [] (value_type const& value) {
            return value.has_value();
        });
    }

    constexpr std::size_t empty() const {
        return size() == 0;
    }

    constexpr auto keys_view() {
        return data
            | std::ranges::views::filter([](value_type const& value) { return value.has_value(); })
            | std::ranges::views::transform([](value_type const& value) { return value->first; });
    }

    constexpr auto find(key_type const& key) const {
        return std::ranges::find_if(data, [&](value_type const& element){
            return element && element->first == key;
        });
    }

    constexpr auto find(key_type const& key) {
        return std::ranges::find_if(data, [&](value_type const& element){
            return element && element->first == key;
        });
    }

    constexpr auto find_slot() const {
        return std::ranges::find_if(data, [&](value_type const& element){
            return element.has_value() == false;
        });
    }

    constexpr auto insert(key_type const& key, value_type const& val) {
        auto iter = find_slot();
        if (iter != data.end()) {

        }
        value_type{key, val};
    }

    constexpr auto clear() noexcept {
        for (value_type& value : data) { value.reset(); }
    }

    constexpr bool erase(key_type const& key) {
        auto iter = find(key);
        if (iter == data.end()) { return false; }
        iter->reset();
        return true;
    }

    constexpr auto at(key_type const& key) const {
        return find(key)->value();
    }
};

// Use this to explicitly define KeyType and ValueType
template <typename KeyType, typename ValueType, typename... Pairs>
constexpr auto make_map(Pairs&&... in) {
    return map<KeyType, ValueType, sizeof...(Pairs)>{std::forward<Pairs>(in)...};
}

template<typename... Args>
map(Args...) -> map<
    typename std::common_type<Args...>::type::first_type,
    typename std::common_type<Args...>::type::second_type,
    sizeof...(Args)
>;

} // NS

// template <typename key_t, typename val_t, size_t staticSize>
// struct StaticMap {

//     constexpr auto add(key_t key, val_t elem) {
//         size_t pos = static_cast<size_t>(key);
//         flags.set(pos);
//         data.at(pos) = elem;
//     }

//     constexpr auto remove(key_t key) {
//         size_t pos = static_cast<size_t>(key);
//         flags.reset(pos);
//     }

//     constexpr auto exists(key_t key) {
//         size_t pos = static_cast<size_t>(key);
//         return flags.test(pos);
//     }

//     constexpr auto& at(key_t key) {
//         size_t pos = static_cast<size_t>(key);
//         if (!flags.test(pos)) {
//             throw  std::runtime_error("Access to missing element");
//         }
//         return data.at(pos);
//     }

//     constexpr auto visit(auto func) -> void {
//         for (size_t i = 0; i <= data.size(); i++) {
//             if (flags.test(i)) { func(data.at(i)); }
//         }
//     }

//     constexpr auto at(key_t key) const -> const val_t& {
//         size_t pos = static_cast<size_t>(key);
//         if (!flags.test(pos)) {
//             throw std::runtime_error{"Access to missing element"};
//         }
//         return data.at(pos);
//     }
    
//     bool operator==(const StaticMap<key_t, val_t, staticSize>& other) const = default;

//     std::array<val_t, staticSize> data{};
//     bitset<staticSize> flags{};
// };

// #include <cttest.h>
// enum class ID {
//     A,
//     B,
//     C,
// };

// constexpr auto ut_StaticMap = [] {
//     cttest::Suite suite{"ut_StaticMap"};
//     StaticMap<ID, int, 3> map;
//     map.add(ID::A, 42);
//     suite.add(
//         cttest::expectEq(map.at(ID::A), 42)
//     );
//     map.add(ID::B, 100);
//     suite.add(
//         cttest::expectEq(map.at(ID::B), 100)
//     );
//     return suite;
// };

// constexpr auto ut_StaticMap2 = [] {
//     cttest::Suite suite{"ut_StaticMap2"};
//     StaticMap<ID, int, 3> map;
//     map.add(ID::A, 42);
//     suite.add(
//         cttest::expectEq(map.at(ID::A), 42)
//     );
//     map.add(ID::B, 100);
//     suite.add(
//         cttest::expectEq(map.at(ID::B), 100)
//     );
//     return suite;
// };

// auto ut_StaticMap_ = [] {
//     return (
//         ut_StaticMap().run() &&
//         ut_StaticMap2().run()
//     );
// }();

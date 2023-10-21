#include "staticvector_new.h"

#include <cttest.h>

#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <awful.hpp>

// static constexpr vector<awful::noncopyable, 1> vec2{
//     awful::noncopyable{},
// };

struct NoDefault {
    int i;
    constexpr NoDefault() = delete;
    constexpr NoDefault(int i)
        : i{i}
    {}

    auto operator<=>(NoDefault const&) const = default;
};


struct DynAllocated {
    int* i{};
    // constexpr DynAllocated() : i{new int{-1}} {}
    constexpr DynAllocated() = delete;
    
    constexpr DynAllocated(int i)
        : i{new int{i}}
    {}

    // constexpr DynAllocated(DynAllocated&& other) {
    //     auto tmp = i;
    //     i = other.i;
    //     other.i = tmp;
    // }

    // constexpr DynAllocated(DynAllocated const& other) {
    //     delete i;
    //     i = new int{*other.i};
    // }

    constexpr DynAllocated& operator=(DynAllocated const& other) {
        delete i;
        i = new int{*other.i};
        return *this;
    }

    constexpr ~DynAllocated() {
        delete i;
    }
    constexpr operator int() { return *i; }
    auto operator<=>(DynAllocated const&) const = default;
};

// static vector<NoDefault, 1> vec3{}; // OK
// static std::array<NoDefault, 1> arr3{}; // ERR

int main() {

    {
        vector<awful::tracked, 2> tracked{awful::tracked{1}};
        fmt::println("");
        vector<awful::tracked, 2> tracked2{2};
    }

    // insert
    {
        auto test = [] () -> int {
            vector<std::unique_ptr<int>, 2> vec{
                new int{100}
            };
            vec.insert(vec.begin(), std::make_unique<int>(42));
            int tmp = *vec.front();
            return tmp;
        };
        constexpr int a1 = test();
        [[maybe_unused]]static constexpr int a2 = test();
        static_assert(a1 == 42);
        // static_assert(test().back() == 101);
    }

    // Size
    {
        auto test = [] {
            vector<DynAllocated, 2> vec{1, 2};
            return vec.size();
        };
        static_assert(test() == 2);
    }

    {
        auto test = []() -> int {
            vector<NoDefault, 4> vec{1, 2, 42};
            return vec.at(2).i;
        };
        static_assert(test() == 42);
    }

    {
        auto test = [] {
            vector<int, 2> vec{42};
            vec.insert(vec.begin(), 100);
            return vec;
        };
        static_assert(test().front() == 100);
        static_assert(test().at(1) == 42);
    }

    // {
    //     auto test = [] {
    //         vector<int, 2> vec{};
    //         auto const res = vec.insert(vec.begin() + 1, 42);
    //         return *res;
    //     };
    //     static_assert(test() == 42);
    // }

    // // front & back
    // {
    //     auto test = [] () {
    //         vector<int, 4> vec{1, 2, 3, 4};
    //         return vec;
    //     };
    //     static_assert(test().front() == 1);
    //     static_assert(test().back() == 4);
    // }

    // // data
    // {
    //     auto test = [] () {
    //         vector<int, 4> vec{1, 2, 3, 4};
    //         return vec;
    //     };
    //     static_assert(*(test().data()) == 1);
    // }

    // // clear
    // {
    //     auto test = [] {
    //         vector<DynAllocated, 4> vec{1, 2, 3, 4};
    //         vec.clear();
    //         return vec;
    //     };
    //     static_assert(test().empty());
    //     static_assert(test().size() == 0);
    // }

    // insert
    {
        auto test = [] () -> int {
            vector<std::unique_ptr<int>, 4> vec{};
            vec.insert(vec.begin() + 0, std::make_unique<int>(1));
            return *vec.front();
        };
        constexpr int a1 = test();
        static_assert(a1 == 1);
        // static_assert(test().back() == 101);
    }

    // // emplace
    // {
    //     auto test = [] () {
    //         vector<NoDefault, 4> vec{};
    //         vec.emplace(std::size_t(0), 1);
    //         vec.emplace(std::size_t(1), 2);
    //         vec.emplace(std::size_t(2), 3);
    //         vec.emplace(std::size_t(3), 4);
    //         return vec;
    //     };
    //     static_assert(test().at(0) == NoDefault{1});
    //     static_assert(test().at(1) == NoDefault{2});
    //     static_assert(test().at(2) == NoDefault{3});
    //     static_assert(test().at(3) == NoDefault{4});
    // }

    // // erase
    // {
    //     auto test = [] () {
    //         vector<int, 4> vec{1, 2, 3, 4};
    //         return vec.erase(vec.begin());
    //     };
    //     static constexpr int next = *test();
    //     static_assert(next == 2);
    // }

    // // erase last element
    // {
    //     auto test = [] () {
    //         vector<int, 4> vec{1, 2, 3, 4};
    //         auto next = vec.erase(vec.end());
    //         return std::distance(next, vec.end());
    //     };
    //     // constexpr int distance = test();
    //     // static_assert(distance == 0);
    // }
}

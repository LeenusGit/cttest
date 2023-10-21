#pragma once

#include <cstddef>
#include <array>
#include <algorithm>
#include <memory>
#include <utility>
#include <cassert>

template <typename T, std::size_t cap>
struct vector;

template <typename U>
struct element_t {
    // friend struct vector;
    // struct Null{};
    // Null null{};
// private:
    union {
        U contained;
    };
public:

    constexpr element_t() {}

    template <typename... Args>
    constexpr element_t(Args&&... args)
        : contained{std::forward<decltype(args)>(args)...}
    {}

    constexpr element_t(element_t&& other)
        : contained{std::move(other.contained)}
    { }

    constexpr element_t(element_t const& other)
        : contained{other.contained}
    { }

    constexpr element_t(U&& in)
        : contained{std::move(in)}
    { }

    constexpr ~element_t() {}

    // constexpr element_t& operator=(element_t & other) {
    //     // contained = std::move(other.contained);
    //     std::swap(contained, other.contained);
    //     return *this;
    // }

    constexpr element_t& operator=(element_t const& other) {
        return *this;
    };

    constexpr element_t& operator=(element_t&& other) {
        // contained = std::move(other.contained);
        // reset();
        std::construct_at(this, std::move(other));
        return *this;
    }

    // constexpr element_t& operator=(element_t const& other) = default;

    constexpr void reset() {
        std::ranges::destroy_at(&contained);
    }
    
    // constexpr void create_from(U const& in) {
    //     contained = in;
    // }

    constexpr void create_from(U&& in) {
        contained = std::move(in);
    }
    
    constexpr operator auto&() { return contained; }
    constexpr operator auto&() const { return contained; }

    constexpr operator auto*() { return &contained; }
    constexpr operator auto*() const { return &contained; }
};

template <typename T, std::size_t cap>
struct vector {


    using storage_t = std::array<element_t<T>, cap>;
    using value_type = storage_t::value_type;
    using iterator = storage_t::iterator;
    using const_iterator = storage_t::const_iterator;
    using pointer = storage_t::pointer;

    storage_t storage;
    std::size_t count;

    // Constructors

    template <typename... Args>
    constexpr vector(Args... args)
        : storage{std::forward<decltype(args)>(args)...}
        , count{sizeof...(Args)}
    {}

    constexpr ~vector() { clear(); }

    // Element access

    constexpr auto at(std::size_t idx) -> T& { return storage.at(idx); }
    constexpr auto at(std::size_t idx) const -> T const& { return storage.at(idx); }

    constexpr auto operator[](std::size_t idx) -> T& { return storage[idx]; }
    constexpr auto operator[](std::size_t idx) const -> T const& { return storage[idx]; }

    constexpr auto front() -> T& { return *begin(); }
    constexpr auto front() const -> T const& { return *begin(); }

    constexpr auto back() -> T& { return *(end() - 1); }
    constexpr auto back() const -> T const& { return *(end() - 1); }

    constexpr auto data() -> pointer { return storage.data(); }

    // Iterators

    constexpr auto begin() const -> const_iterator { return storage.begin(); }
    constexpr auto end() const -> const_iterator { return begin() + count; }

    constexpr auto begin() -> iterator { return storage.begin(); }
    constexpr auto end() -> iterator { return begin() + count; }

    constexpr auto cbegin() const -> const_iterator { return begin(); }
    constexpr auto cend() const -> const_iterator { return end(); }

    // Capacity

    constexpr auto empty() -> bool { return size() == 0; }

    constexpr auto size() const -> std::size_t { return std::distance(begin(), end()); }

    constexpr auto max_size() const { return storage.max_size(); }

    constexpr auto capacity() const { return max_size(); }

    // Modifiers

    constexpr auto clear() -> void {
        std::ranges::for_each(begin(), end(), [] (element_t<T>& element){
            element.reset();
        });
        count = 0;
    }

    // constexpr auto insert(const_iterator pos, T const& value) -> iterator {
    //     auto [last, result_minus_N] = std::ranges::move_backward(pos, end(), end() + 1);
    //     iterator insert_pos = std::prev(result_minus_N);
    //     insert_pos->create_from(value);
    //     count++;
    //     return insert_pos;
    // }

    constexpr auto insert(const_iterator pos, T&& value) -> iterator {

        iterator insert_pos = begin() + std::distance(cbegin(), pos);
        std::ranges::move_backward(insert_pos, end(), end() + 1);

        *insert_pos = std::move(value);

        count++;
        return insert_pos;
    }

    constexpr auto insert(std::size_t index, T const& value) -> iterator {
        return insert(begin() + index, value);
    }

    // TODO:
    // constexpr auto insert_range

    template <typename... Args>
    constexpr auto emplace(const_iterator pos, Args&&... args) -> iterator {
        return insert(pos, T{std::forward(args)...});
    }

    template <typename... Args>
    constexpr auto emplace(std::size_t index, Args&&... args) -> iterator {
        return insert(begin() + index, T{std::forward<decltype(args)>(args)...});
    }

    constexpr auto erase(const_iterator pos ) -> iterator {
        iterator const mutable_pos = begin() + std::distance(cbegin(), pos);
        mutable_pos->reset();
        std::ranges::move(mutable_pos + 1, end(), mutable_pos);
        return mutable_pos;
    }

    constexpr auto push_back(T const& value) -> iterator {
        return insert(end(), value);
    }

    template <typename... Args>
    constexpr auto emplace_back(Args&&... args) -> iterator {
        emplace(end(), args...);
    }

    // TODO:
    // constexpr auto append_range

    // constexpr auto pop_back

    // constexpr auto swap

    // constexpr operator<=>
};

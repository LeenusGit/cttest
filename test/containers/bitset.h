#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <bit>

template<size_t BIT_COUNT>
struct bitset {

    struct QuotRem {
        constexpr QuotRem(size_t bitPos) : 
            quotient(bitPos / 8),
            remainder(bitPos % 8)
        { }
        size_t quotient;
        uint8_t remainder;
    };

    constexpr auto set(size_t bit) {
        const auto [idx, rem] = QuotRem(bit);
        _bits.at(idx) |= 1 << rem;
    }

    constexpr auto reset(size_t bit) {
        const auto [idx, rem] = QuotRem(bit);
        _bits.at(idx) &= ~(1 << rem);
    }

    constexpr auto operator[](size_t bit) const -> bool {
        const auto [idx, rem] = QuotRem(bit);
        return _bits.at(idx) & (1 << rem);
    }

    constexpr auto test(size_t bit) const -> bool {
        return operator[](bit);
    }

    constexpr auto size() -> size_t const {
        return _bits.size() * 8;
    }

    constexpr bool operator==(const bitset<BIT_COUNT>& other) const = default;

    std::array<uint8_t, (BIT_COUNT + 8 - 1) / 8> _bits{};
};

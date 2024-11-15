#pragma once
#include <array>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace stdex {

inline constexpr std::size_t dynamic_extent =
    std::numeric_limits<std::size_t>::max();

template <typename T>
struct type_identity {
    using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

template <typename T, std::size_t Extent = dynamic_extent>
class span {
    template <typename Tp, std::size_t ArrayExtent,
              std::enable_if_t<ArrayExtent == Extent, bool> = true>
    using is_compatible_array = std::is_convertible<Tp (*)[], T (*)[]>;

  public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    static constexpr size_type extent = Extent;

    pointer ptr;

    template <std::size_t N>
    constexpr span(type_identity_t<element_type> (&arr)[N]) noexcept
        : ptr(static_cast<pointer>(arr)) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U, N>::value, bool> = true>
    constexpr span(std::array<U, N> &arr) noexcept
        : ptr(static_cast<pointer>(arr.data())) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U, N>::value, bool> = true>
    constexpr span(const std::array<U, N> &arr) noexcept
        : ptr(static_cast<pointer>(arr.data())) {}

    constexpr span(const span<T> &other) noexcept : ptr(other.ptr) {}

    constexpr std::size_t size() const noexcept { return extent; }
    constexpr T &operator[](std::size_t idx) const {
        if (idx >= extent) {
            throw std::out_of_range("span index out of range");
        }
        return this->ptr[idx];
    }

    constexpr reference front() const noexcept { return ptr[0]; }
    constexpr reference back() const noexcept { return ptr[extent - 1]; }
};

template <typename T>
class span<T, 0> {
    static_assert(false, "TODO: implement span with extent of 0");
};

template <typename T>
class span<T, dynamic_extent> {
    template <typename Tp>
    using is_compatible_array = std::is_convertible<Tp (*)[], T (*)[]>;

  public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    static constexpr size_type extent = dynamic_extent;

    T *ptr;
    std::size_t len;

    constexpr span() noexcept : ptr(nullptr), len(0) {}
    constexpr span(pointer ptr, size_type size) : ptr(ptr), len(size) {}

    template <typename U, std::size_t N>
    constexpr span(type_identity_t<element_type> (&arr)[N]) noexcept
        : ptr(arr), len(N) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U>::value, bool> = true>
    constexpr span(std::array<U, N> arr) noexcept : ptr(arr), len(N) {}

    constexpr span(const span<T> &other) noexcept
        : ptr(other.ptr), len(other.len) {}

    constexpr std::size_t size() const noexcept { return this->len; }

    constexpr T &operator[](std::size_t idx) const {
        if (idx >= this->len) {
            throw std::out_of_range("span index out of range");
        }
        return this->ptr[idx];
    }

    constexpr reference front() const noexcept { return ptr[0]; }
    constexpr reference back() const noexcept { return ptr[this->len - 1]; }
};

template <typename T, std::size_t N>
span(const std::array<T, N>) -> span<T, N>;

template <typename T, std::size_t N>
span(T (&arr)[N]) -> span<T, N>;

}; // namespace stdex

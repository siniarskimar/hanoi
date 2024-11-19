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

    template <std::size_t N>
    constexpr span(type_identity_t<element_type> (&arr)[N]) noexcept
        : m_ptr(static_cast<pointer>(arr)) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U, N>::value, bool> = true>
    constexpr span(std::array<U, N> &arr) noexcept
        : m_ptr(static_cast<pointer>(arr.data())) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U, N>::value, bool> = true>
    constexpr span(const std::array<U, N> &arr) noexcept
        : m_ptr(static_cast<pointer>(arr.data())) {}

    constexpr span(const span<T> &other) noexcept : m_ptr(other.m_ptr) {}

    constexpr std::size_t size() const noexcept { return extent; }
    constexpr T &operator[](std::size_t idx) const {
        if (idx >= extent) {
            throw std::out_of_range("span index out of range");
        }
        return this->m_ptr[idx];
    }

    constexpr reference front() const noexcept { return m_ptr[0]; }
    constexpr reference back() const noexcept { return m_ptr[extent - 1]; }

  private:
    pointer m_ptr;
};

template <typename T>
class span<T, 0> {
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

    constexpr span() noexcept {}

    constexpr pointer data() const noexcept { return nullptr; }
    constexpr std::size_t size() const noexcept { return 0; }
    constexpr bool empty() const noexcept { return size() == 0; }
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


    constexpr span() noexcept : m_ptr(nullptr), m_len(0) {}
    constexpr span(pointer ptr, size_type size) : m_ptr(ptr), m_len(size) {}

    template <typename U, std::size_t N>
    constexpr span(type_identity_t<element_type> (&arr)[N]) noexcept
        : m_ptr(arr), m_len(N) {}

    template <typename U, std::size_t N,
              std::enable_if_t<is_compatible_array<U>::value, bool> = true>
    constexpr span(std::array<U, N> arr) noexcept : m_ptr(arr), m_len(N) {}

    constexpr span(const span<T> &other) noexcept
        : m_ptr(other.m_ptr), m_len(other.len) {}

    constexpr std::size_t size() const noexcept { return this->len; }

    constexpr reference operator[](std::size_t idx) const {
        if (idx >= this->len) {
            throw std::out_of_range("span index out of range");
        }
        return this->m_ptr[idx];
    }

    constexpr bool empty() const noexcept { return size() == 0; }

    constexpr reference front() const noexcept { return m_ptr[0]; }
    constexpr reference back() const noexcept { return data()[size() - 1]; }
    constexpr pointer data() const noexcept { return m_ptr; }

  private:
    pointer m_ptr;
    std::size_t m_len;
};

template <typename T, std::size_t N>
span(const std::array<T, N>) -> span<T, N>;

template <typename T, std::size_t N>
span(T (&arr)[N]) -> span<T, N>;

}; // namespace stdex

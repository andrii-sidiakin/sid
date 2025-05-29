#ifndef _SID_BIO_BUFFER_HPP_INCLUDED_
#define _SID_BIO_BUFFER_HPP_INCLUDED_

#include "sid/bio/basic_buffer.hpp"

#include <cstddef>

namespace sid::bio {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.bio.const_buffer

///
///
///
template <buffer_extent_t Extent>
class const_buffer : public basic_buffer<const std::byte, Extent> {
    using base_type = basic_buffer<const std::byte, Extent>;

  public:
    using basic_buffer<const std::byte, Extent>::basic_buffer;

    ///
    template <aka::byte_representable T, std::size_t N>
    explicit constexpr const_buffer(const T (&arr)[N]) noexcept
        : base_type(std::as_bytes(std::span(arr))) {}

    ///
    template <aka::byte_representable T, std::size_t N>
    explicit constexpr const_buffer(const std::array<T, N> &arr) noexcept
        : base_type(std::as_bytes(std::span(arr))) {}

    ///
    template <aka::byte_representable T, std::size_t N>
    const_buffer(std::array<T, N> &&arr) = delete;

    ///
    template <aka::byte_representable_range Rng>
        requires(std::ranges::borrowed_range<Rng>)
    explicit constexpr const_buffer(Rng &&r) noexcept
        : base_type(std::as_bytes(std::span{std::data(r), std::size(r)})) {}
};

///
template <aka::byte_representable T, std::size_t Size>
const_buffer(const T (&arr)[Size]) -> const_buffer<sizeof(T) * Size>;

///
template <aka::byte_representable T, std::size_t Size>
const_buffer(const std::array<T, Size> &) -> const_buffer<sizeof(T) * Size>;

///
template <aka::byte_representable T, std::size_t Extent>
const_buffer(std::span<const T, Extent>)
    -> const_buffer<Extent == std::dynamic_extent ? bio::dynamic_extent
                                                  : sizeof(T) * Extent>;

///
template <std::ranges::input_range Rng>
const_buffer(Rng &&r) -> const_buffer<dynamic_extent>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.bio.mutable_buffer

///
///
///
template <buffer_extent_t Extent>
class mutable_buffer : public basic_buffer<std::byte, Extent> {
    using base_type = basic_buffer<std::byte, Extent>;

  public:
    using basic_buffer<std::byte, Extent>::basic_buffer;

    ///
    template <aka::byte_representable T, std::size_t N>
        requires(not std::is_const_v<T>)
    explicit constexpr mutable_buffer(T (&arr)[N]) noexcept
        : base_type(std::as_writable_bytes(std::span(arr))) {}

    ///
    template <aka::byte_representable T, std::size_t N>
        requires(not std::is_const_v<T>)
    explicit constexpr mutable_buffer(std::array<T, N> &arr) noexcept
        : base_type(std::as_writable_bytes(std::span(arr))) {}

    ///
    template <aka::byte_representable_range Rng>
        requires(not std::ranges::constant_range<Rng> and
                 not std::is_array_v<Rng>)
    explicit constexpr mutable_buffer(Rng &r) noexcept
        : base_type(std::as_writable_bytes(
              std::span(std::ranges::data(r), std::ranges::size(r)))) {}
};

///
template <aka::byte_representable T, std::size_t Size>
mutable_buffer(T (&arr)[Size]) -> mutable_buffer<sizeof(T) * Size>;

///
template <aka::byte_representable T, std::size_t Size>
mutable_buffer(std::array<T, Size> &) -> mutable_buffer<sizeof(T) * Size>;

///
template <aka::byte_representable T, std::size_t Extent>
mutable_buffer(std::span<T, Extent>)
    -> mutable_buffer<Extent == std::dynamic_extent ? bio::dynamic_extent
                                                    : sizeof(T) * Extent>;

///
template <std::ranges::input_range Rng>
mutable_buffer(Rng &r) -> mutable_buffer<dynamic_extent>;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // namespace sid::bio

#endif

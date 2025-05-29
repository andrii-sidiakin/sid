#ifndef _SID_BIO_BASIC_BUFFER_HPP_INCLUDED_
#define _SID_BIO_BASIC_BUFFER_HPP_INCLUDED_

#include <sid/assert.hpp>

#include <ranges>
#include <span>

namespace sid::bio {

namespace aka {

template <typename T>
concept byte = sizeof(T) == sizeof(std::byte) and
               std::has_unique_object_representations_v<T>;

template <typename T>
concept byte_representable =
    std::is_trivial_v<T> and std::is_trivially_copyable_v<T>;

template <typename T>
concept byte_representable_range =
    std::ranges::contiguous_range<T> and std::ranges::sized_range<T> and
    byte_representable<std::ranges::range_value_t<T>>;

} // namespace aka

///
using buffer_extent_t = std::size_t;

///
constexpr buffer_extent_t dynamic_extent = std::dynamic_extent;

///
///
///
template <aka::byte Byte, buffer_extent_t Extent> class basic_buffer {
  public:
    static constexpr bool is_readonly = std::is_const_v<Byte>;
    static constexpr bool is_writable = !is_readonly;

  public:
    using byte_span = std::span<Byte, Extent>;
    using byte_type = Byte;
    using size_type = byte_span::size_type;

  public:
    constexpr basic_buffer() noexcept = default;

    constexpr basic_buffer(const basic_buffer &) noexcept = default;
    constexpr basic_buffer &operator=(const basic_buffer &) noexcept = default;

    constexpr basic_buffer(basic_buffer &&) noexcept = default;
    constexpr basic_buffer &operator=(basic_buffer &&) noexcept = default;

  public:
    ///
    constexpr basic_buffer(byte_type *bytes, size_type count) noexcept
        requires(Extent == dynamic_extent)
        : m_span(bytes, count) {
        sid_expect(bytes || count == 0, "count is not 0 but nullptr");
    }

    ///
    constexpr basic_buffer(byte_span span) noexcept : m_span(span) {}

  public:
    ///
    template <class Self>
    [[nodiscard]] constexpr byte_type *data(this Self &&self) noexcept {
        return self.m_span.data();
    }

    ///
    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_span.size();
    }

    ///
    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_span.size() == 0;
    }

  private:
    byte_span m_span;
};

} // namespace sid::bio

#endif

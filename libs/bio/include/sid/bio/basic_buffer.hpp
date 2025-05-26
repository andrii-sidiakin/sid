#ifndef _SID_BIO_BASIC_BUFFER_HPP_INCLUDED_
#define _SID_BIO_BASIC_BUFFER_HPP_INCLUDED_

#include <ranges>
#include <span>

namespace sid::bio {

///
///
///
template <typename Byte, std::size_t Extent = std::dynamic_extent>
class basic_buffer {

    static_assert(sizeof(Byte) == sizeof(std::byte));

    template <typename Elem>
    static constexpr bool is_elem_readable =
        std::has_unique_object_representations_v<Elem>;

    template <typename Elem>
    static constexpr bool is_elem_writable =
        is_elem_readable<Elem> and not std::is_const_v<Elem>;

    template <typename Rng>
    static constexpr bool is_range_readable =
        std::ranges::contiguous_range<Rng> and std::ranges::sized_range<Rng> and
        is_elem_readable<std::ranges::range_value_t<Rng>>;

    template <typename Rng>
    static constexpr bool is_range_writable =
        std::ranges::contiguous_range<Rng> and std::ranges::sized_range<Rng> and
        is_elem_writable<std::ranges::range_value_t<Rng>>;

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
    ///
    constexpr basic_buffer(byte_type *bytes, size_type count) noexcept
        requires(Extent == std::dynamic_extent)
        : m_span(bytes, count) {}

    ///
    ///
    constexpr basic_buffer(byte_span span) noexcept : m_span(span) {}

  public:
    ///
    ///
    template <typename T, std::size_t N>
        requires(is_readonly and is_elem_readable<T>)
    explicit basic_buffer(const T (&arr)[N]) noexcept
        : m_span(std::as_bytes(std::span(std::addressof(arr), N))) {}

    ///
    ///
    template <typename T, std::size_t N>
        requires(is_writable and is_elem_writable<T>)
    explicit basic_buffer(T (&arr)[N]) noexcept
        : m_span(std::as_writable_bytes(std::span(std::addressof(arr), N))) {}

  public:
    ///
    ///
    template <std::ranges::input_range Rng>
        requires(is_readonly and is_range_readable<Rng> and
                 std::ranges::borrowed_range<Rng>)
    explicit constexpr basic_buffer(Rng &&r) noexcept
        : m_span(std::as_bytes(
              std::span(std::ranges::data(r), std::ranges::size(r)))) {}

    ///
    ///
    template <std::ranges::input_range Rng>
        requires(is_writable and is_range_writable<Rng> and
                 not std::is_array_v<Rng>)
    explicit constexpr basic_buffer(Rng &r) noexcept
        : m_span(std::as_writable_bytes(
              std::span(std::ranges::data(r), std::ranges::size(r)))) {}

  public:
    ///
    template <class Self>
    [[nodiscard]] constexpr auto data(this Self &&self) noexcept {
        return std::forward_like<decltype(self)>(self.m_span.data());
    }

    ///
    template <class Self>
    [[nodiscard]] constexpr auto size(this Self &&self) noexcept {
        return std::forward_like<decltype(self)>(self.m_span.size());
    }

  private:
    byte_span m_span;
};

///
template <typename T, std::size_t Extent>
basic_buffer(std::span<T, Extent> span)
    -> basic_buffer<std::byte, Extent == std::dynamic_extent
                                   ? std::dynamic_extent
                                   : sizeof(T) * Extent>;

///
template <typename T, std::size_t Extent>
basic_buffer(std::span<const T, Extent> span)
    -> basic_buffer<const std::byte, Extent == std::dynamic_extent
                                         ? std::dynamic_extent
                                         : sizeof(T) * Extent>;

///
template <typename T, std::size_t Size>
basic_buffer(T (&arr)[Size]) -> basic_buffer<std::byte, sizeof(T) * Size>;

///
template <typename T, std::size_t Size>
basic_buffer(const T (&arr)[Size])
    -> basic_buffer<const std::byte, sizeof(T) * Size>;

///
template <std::ranges::input_range Rng>
basic_buffer(Rng &&r) -> basic_buffer<const std::byte, std::dynamic_extent>;

///
template <std::ranges::input_range Rng>
basic_buffer(Rng &r) -> basic_buffer<std::byte, std::dynamic_extent>;

} // namespace sid::bio

#endif

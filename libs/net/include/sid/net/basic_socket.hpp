#ifndef _SID_NET_BASIC_SOCKET_HPP_INCLUDED_
#define _SID_NET_BASIC_SOCKET_HPP_INCLUDED_

// #include "sid/net/buffer.hpp"
#include <sid/bio/buffer.hpp>

#include <system_error>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.net.socket_api

namespace sid::net::details {

namespace _adl_only {
void socket_open(auto...) = delete;
void socket_close(auto...) = delete;
void socket_connect(auto...) = delete;
void socket_recv(auto...) = delete;
void socket_send(auto...) = delete;
} // namespace _adl_only

} // namespace sid::net::details

namespace sid::net::details {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_open_fn {
    template <class Socket, class Options = Socket::options>
    constexpr void operator()(Socket &s, Options opts,
                              std::error_code &ec) const {
        using namespace _adl_only;
        socket_open(s, opts, ec);
    }

    template <class Socket, class Options = Socket::options>
    constexpr bool operator()(Socket &s, Options opts) const {
        using namespace _adl_only;
        std::error_code ec;
        socket_open(s, opts, ec);
        return !ec;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_close_fn {
    template <class Socket>
    constexpr void operator()(Socket &s, std::error_code &ec) const {
        using namespace _adl_only;
        socket_close(s, ec);
    }

    template <class Socket> constexpr bool operator()(Socket &s) const {
        using namespace _adl_only;
        std::error_code ec;
        socket_close(s, ec);
        return !ec;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_connect_fn {
    template <class Socket, class Address = Socket::address>
    constexpr void operator()(Socket &s, Address addr,
                              std::error_code &ec) const {
        using namespace _adl_only;
        socket_connect(s, addr, ec);
    }

    template <class Socket, class Address = Socket::address>
    constexpr bool operator()(Socket &s, Address addr) const {
        using namespace _adl_only;
        std::error_code ec;
        socket_connect(s, addr, ec);
        return !ec;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_bind_fn {
    template <class Socket, class Address = Socket::address>
    constexpr void operator()(Socket &s, Address addr,
                              std::error_code &ec) const {
        using namespace _adl_only;
        socket_bind(s, addr, ec);
    }

    template <class Socket, class Address = Socket::address>
    constexpr bool operator()(Socket &s, Address addr) const {
        using namespace _adl_only;
        std::error_code ec;
        socket_bind(s, addr, ec);
        return !ec;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_recv_fn {
    template <class Socket>
    constexpr auto operator()(Socket &s, std::byte *buf, std::size_t size,
                              std::error_code &ec) const {
        using namespace _adl_only;
        return socket_recv(s, buf, size, ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, std::byte *buf,
                              std::size_t size) const {
        using namespace _adl_only;
        std::error_code ec;
        return socket_recv(s, buf, size, ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, bio::mutable_buffer buf,
                              std::error_code &ec) const {
        return this->operator()(s, buf.data(), buf.size(), ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, bio::mutable_buffer buf) const {
        return this->operator()(s, buf.data(), buf.size());
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
///
///
struct socket_send_fn {
    template <class Socket>
    constexpr auto operator()(Socket &s, const std::byte *buf, std::size_t size,
                              std::error_code &ec) const {
        using namespace _adl_only;
        return socket_send(s, buf, size, ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, const std::byte *buf,
                              std::size_t size) const {
        using namespace _adl_only;
        std::error_code ec;
        return socket_send(s, buf, size, ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, bio::const_buffer buf,
                              std::error_code &ec) const {
        return this->operator()(s, buf.data(), buf.size(), ec);
    }

    template <class Socket>
    constexpr auto operator()(Socket &s, bio::const_buffer buf) const {
        return this->operator()(s, buf.data(), buf.size());
    }
};

} // namespace sid::net::details

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace sid::net {
namespace {

constexpr details::socket_open_fn open{};
constexpr details::socket_close_fn close{};
constexpr details::socket_connect_fn connect{};
constexpr details::socket_bind_fn bind{};
constexpr details::socket_recv_fn recv{};
constexpr details::socket_send_fn send{};

} // namespace

} // namespace sid::net

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.net.basic_socket

namespace sid::net {

///
///
///
template <class Impl> class basic_socket {
  public:
    using options = Impl::options;
    using address = Impl::address;

  public:
    constexpr basic_socket() noexcept = default;

    constexpr basic_socket(basic_socket &&) noexcept = default;
    constexpr basic_socket &operator=(basic_socket &&) noexcept = default;

    constexpr basic_socket(Impl &&impl) noexcept : m_impl{std::move(impl)} {}

    ~basic_socket() noexcept {
        std::error_code ec;
        close(ec);
    }

  public:
    constexpr bool is_valid() const noexcept { return m_impl.is_valid(); }

    explicit constexpr operator bool() const noexcept {
        return m_impl.is_valid();
    }

  public:
    template <typename... Args> auto open(Args &&...args) {
        return sid::net::open(m_impl, std::forward<Args>(args)...);
    }

    template <typename... Args> auto close(Args &&...args) {
        return sid::net::close(m_impl, std::forward<Args>(args)...);
    }

    template <typename... Args> auto connect(Args &&...args) {
        return sid::net::connect(m_impl, std::forward<Args>(args)...);
    }

    template <typename... Args> auto bind(Args &&...args) {
        return sid::net::bind(m_impl, std::forward<Args>(args)...);
    }

    template <typename... Args> auto recv(Args &&...args) {
        return sid::net::recv(m_impl, std::forward<Args>(args)...);
    }

    template <typename... Args> auto send(Args &&...args) {
        return sid::net::send(m_impl, std::forward<Args>(args)...);
    }

  private:
    template <typename... Args>
    friend auto socket_open(basic_socket &s, Args &&...args) {
        return s.open(std::forward<Args>(args)...);
    }

    template <typename... Args>
    friend auto socket_close(basic_socket &s, Args &&...args) {
        return s.close(std::forward<Args>(args)...);
    }

    template <typename... Args>
    friend auto socket_connect(basic_socket &s, Args &&...args) {
        return s.connect(std::forward<Args>(args)...);
    }

    template <typename... Args>
    friend auto socket_recv(basic_socket &s, Args &&...args) {
        return s.recv(std::forward<Args>(args)...);
    }

    template <typename... Args>
    friend auto socket_send(basic_socket &s, Args &&...args) {
        return s.send(std::forward<Args>(args)...);
    }

  private:
    Impl m_impl;
};

} // namespace sid::net

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

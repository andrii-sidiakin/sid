#ifndef _SID_NET_BSD_SOCKET_HPP_INCLUDED_
#define _SID_NET_BSD_SOCKET_HPP_INCLUDED_

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <system_error>
#include <utility>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace sid::net::bsd {

///
///
///
struct socket_options {
    int family{AF_UNSPEC};
    int socktype{0};
    int protocol{0};

    bool nonblock{false};
    bool reuse_addr{false};
    bool reuse_port{false};
};

///
///
///
struct socket_address {
    sockaddr *addr{nullptr};
    socklen_t addrlen{0};
};

///
///
///
class socket_impl {
  private:
    static constexpr int invalid = -1;

  public:
    using options = socket_options;
    using address = socket_address;

  public:
    constexpr socket_impl() noexcept = default;

    constexpr socket_impl(socket_impl &&that) noexcept
        : m_handle{std::exchange(that.m_handle, invalid)} {}

    constexpr socket_impl &operator=(socket_impl &&rhs) noexcept {
        if (this != &rhs) {
            m_handle = std::exchange(rhs.m_handle, invalid);
        }
        return *this;
    }

    explicit constexpr socket_impl(int handle) noexcept : m_handle{handle} {}

  public:
    [[nodiscard]]
    constexpr int handle() const noexcept {
        return m_handle;
    }

    [[nodiscard]]
    constexpr bool is_valid() const noexcept {
        return m_handle >= 0;
    }

  private:
    int m_handle{invalid};
};

}; // namespace sid::net::bsd

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace sid::net::bsd {

///
void socket_open(socket_impl &s, socket_options opts, std::error_code &ec);

///
void socket_close(socket_impl &s, std::error_code &ec);

///
void socket_bind(socket_impl &s, socket_address addr, std::error_code &ec);

///
void socket_connect(socket_impl &s, socket_address addr, std::error_code &ec);

///
size_t socket_recv(socket_impl &s, std::byte *buf, size_t size,
                   std::error_code &ec);

///
size_t socket_send(socket_impl &s, const std::byte *buf, size_t size,
                   std::error_code &ec);

} // namespace sid::net::bsd

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

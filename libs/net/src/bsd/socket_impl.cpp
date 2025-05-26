#include "sid/net/bsd/socket_impl.hpp"
#include "sid/net/error.hpp"

#include <sys/socket.h>
#include <unistd.h>

namespace sid::net::bsd {

static bool socket_setopt(int sd, int opt, int val, std::error_code &ec) {
    if (::setsockopt(sd, SOL_SOCKET, opt, &val, sizeof(val)) != 0) {
        ec = std::error_code(errno, std::generic_category());
    }
    return !ec;
}

void socket_open(socket_impl &s, socket_options opts, std::error_code &ec) {
    if (s.is_valid()) {
        // TODO: report correct error
        ec = make_error_code(fail_code::already_open);
        return;
    }

    int sd = ::socket(opts.family, opts.socktype, opts.protocol);
    if (sd == -1) {
        ec = std::error_code(errno, std::generic_category());
        return;
    }

    if (opts.nonblock) {
        if (!socket_setopt(sd, SOCK_NONBLOCK, 1, ec)) {
            ::close(sd);
            return;
        }
    }
    if (opts.reuse_addr) {
        if (!socket_setopt(sd, SO_REUSEADDR, 1, ec)) {
            ::close(sd);
            return;
        }
    }
    if (opts.reuse_port) {
        if (!socket_setopt(sd, SO_REUSEPORT, 1, ec)) {
            ::close(sd);
            return;
        }
    }

    s = socket_impl{sd};
}

void socket_close(socket_impl &s, std::error_code &ec) {
    socket_impl t = std::exchange(s, {});
    if (t.is_valid()) {
        if (::close(t.handle()) != 0) {
            ec = std::error_code(errno, std::generic_category());
        }
        else {
            s = std::move(t);
        }
    }
}

void socket_bind(socket_impl &s, socket_address addr, std::error_code &ec) {
    if (::bind(s.handle(), addr.addr, addr.addrlen) != 0) {
        ec = std::error_code(errno, std::generic_category());
    }
}

void socket_connect(socket_impl &s, socket_address addr, std::error_code &ec) {
    if (::connect(s.handle(), addr.addr, addr.addrlen) != 0) {
        ec = std::error_code(errno, std::generic_category());
    }
}

size_t socket_recv(socket_impl &s, std::byte *buf, size_t size,
                   std::error_code &ec) {
    if (!s.is_valid()) {
        ec = std::make_error_code(std::errc::bad_file_descriptor);
        return 0;
    }

    ssize_t ret = ::recv(s.handle(), buf, size, 0);
    if (ret < 0) {
        ec = std::error_code(errno, std::generic_category());
        return 0;
    }
    return static_cast<size_t>(ret);
}

size_t socket_send(socket_impl &s, const std::byte *buf, size_t size,
                   std::error_code &ec) {
    if (!s.is_valid()) {
        ec = std::make_error_code(std::errc::bad_file_descriptor);
        return 0;
    }

    ssize_t ret = ::send(s.handle(), buf, size, 0);
    if (ret < 0) {
        ec = std::error_code(errno, std::generic_category());
        return 0;
    }
    return static_cast<size_t>(ret);
}

} // namespace sid::net::bsd

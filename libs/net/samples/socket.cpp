#include <sid/net/error.hpp>
#include <sid/net/socket.hpp>

#include <sid/assert.hpp>

#include <cassert>
#include <print>

#include <netdb.h>

sid::net::socket tcp_connect(const char *host, const char *service) {
    addrinfo hints = {};
    hints.ai_family = AF_INET;
    addrinfo *list = nullptr;

    int ret = getaddrinfo(host, service, &hints, &list);
    if (ret != 0) {
        return {};
    }

    sid::net::socket sock;

    for (auto *ai = list; ai != nullptr; ai = ai->ai_next) {
        std::error_code ec;
        sid::net::open(sock,
                       sid::net::socket::options{
                           .family = ai->ai_family,
                           .socktype = ai->ai_socktype,
                           .protocol = ai->ai_protocol,
                           .nonblock = false,
                       },
                       ec);
        if (ec) {
            continue;
        }

        sid::net::connect(sock,
                          sid::net::socket::address{
                              ai->ai_addr,
                              ai->ai_addrlen,
                          },
                          ec);
        if (ec) {
            sock.close();
            continue;
        }

        break;
    }

    freeaddrinfo(list);

    return sock;
}

int main() {
    std::error_code ec;

    std::println("connecting...");
    sid::net::socket cli = tcp_connect("google.com", "http");
    if (!cli) {
        return 0;
    }

    cli.open(sid::net::socket::options{}, ec);
    sid_expect_eq(ec, sid::net::fail_code::already_open);
    sid_expect_eq(ec, sid::net::fail_condition::open_failure);
    ec = {};

    std::println("sending...");
    std::string_view req{"GET / HTTP/1.0\r\nHost: google.com\r\n\r\n"};
    size_t num_sent = cli.send(sid::bio::const_buffer{req}, ec);
    if (ec) {
        std::println("send fail: {}", ec.message());
        cli.close();
        return 0;
    }
    std::println("send: {} bytes", num_sent);

    std::println("receiving...");
    std::array<char, 512> buf{};
    while (true) {
        size_t num_rcvd = cli.recv(sid::bio::mutable_buffer{buf}, ec);
        if (ec) {
            std::println("recv fail: {}", ec.message());
            break;
        }

        buf[num_rcvd] = 0;
        std::println("recv: {} bytes", num_rcvd);
        std::println("{}", buf.data());

        if (num_rcvd < buf.size()) {
            break;
        }
    }

    cli.close(ec);

    return 0;
}

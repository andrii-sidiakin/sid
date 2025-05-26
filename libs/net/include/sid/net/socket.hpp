#ifndef _SID_NET_SOCKET_HPP_INCLUDED_
#define _SID_NET_SOCKET_HPP_INCLUDED_

#include "sid/net/basic_socket.hpp"

#if defined(__linux__)
#define SID_NET_USE_BSD_SOCKET_IMPL
#endif

#if defined(SID_NET_USE_BSD_SOCKET_IMPL)
#include "bsd/socket_impl.hpp"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.net.socket

namespace sid::net {

#if defined(SID_NET_USE_BSD_SOCKET_IMPL)
using socket = basic_socket<bsd::socket_impl>;
#else
#error "sid.net: Platform not supported"
#endif

} // namespace sid::net

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

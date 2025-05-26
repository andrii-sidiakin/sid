#ifndef _SID_BIO_BUFFER_HPP_INCLUDED_
#define _SID_BIO_BUFFER_HPP_INCLUDED_

#include "sid/bio/basic_buffer.hpp"

#include <cstddef>

namespace sid::bio {

using const_buffer = basic_buffer<const std::byte>;

using mutable_buffer = basic_buffer<std::byte>;

} // namespace sid::bio

#endif

#ifndef _SID_ASSERT_HPP_
#define _SID_ASSERT_HPP_

#include <sid/dbg/assert_stmt.hpp>

namespace sid {

using dbg::check_stmt;
using dbg::is_check_stmt_tag;
using dbg::is_check_stmt_tag_v;

using dbg::assert_tag_t;
using dbg::ensure_tag_t;
using dbg::expect_tag_t;

using dbg::assert_tag;
using dbg::ensure_tag;
using dbg::expect_tag;

} // namespace sid

#endif

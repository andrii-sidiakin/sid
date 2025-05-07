#ifndef _SID_DBG_ASSERT_STMT_HPP_
#define _SID_DBG_ASSERT_STMT_HPP_

#include <sid/dbg/check_stmt.hpp>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace sid::dbg {

///
enum class stmt_kind : std::uint8_t {
    assert, ///< generic assertion
    expect, ///< pre condition
    ensure, ///< post condition
};

namespace _dbg_impl {

template <stmt_kind Kind> struct stmt_tag_of_kind_t {
    static constexpr stmt_kind kind = Kind;
};

} // namespace _dbg_impl

template <stmt_kind Kind>
using stmt_tag_of_kind_t = _dbg_impl::stmt_tag_of_kind_t<Kind>;

// make stmt_tag_of_kind_t to be a check_stmt tag
template <stmt_kind Kind>
struct is_check_stmt_tag<stmt_tag_of_kind_t<Kind>> : std::true_type {};

// handy types
using assert_tag_t = stmt_tag_of_kind_t<stmt_kind::assert>;
using expect_tag_t = stmt_tag_of_kind_t<stmt_kind::expect>;
using ensure_tag_t = stmt_tag_of_kind_t<stmt_kind::ensure>;
// handy constants
inline constexpr assert_tag_t assert_tag{};
inline constexpr expect_tag_t expect_tag{};
inline constexpr ensure_tag_t ensure_tag{};

//
constexpr void print_stmt_to_stderr(const char *prefix,
                                    const check_stmt &stmt) noexcept {
    std::fprintf(stderr, "\n%s[ (%s) %s (%s) ]: %s\n ~ %s\n ~ %s:%i\n", prefix,
                 stmt.lhs, stmt.cmp, stmt.rhs, (stmt.msg.c_str()),
                 stmt.src.function_name(), stmt.src.file_name(),
                 stmt.src.line());
}

namespace _dbg_impl {

// stmt violation handler for stmt_tag_of_kind_t
template <stmt_kind Kind>
void handle_stmt_violation(stmt_tag_of_kind_t<Kind> /*tag*/,
                           const check_stmt &stmt) {
    constexpr bool verbose = true; // TODO: get from traits

    if constexpr (verbose) {
        constexpr auto get_prefix = []() {
            switch (Kind) {
            case stmt_kind::assert:
                return "Assert ";
            case stmt_kind::expect:
                return "Expect ";
            case stmt_kind::ensure:
                return "Ensure ";
            }
            return "";
        };

        print_stmt_to_stderr(get_prefix(), stmt);
    }
    std::abort();
}

} // namespace _dbg_impl

} // namespace sid::dbg

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid_assert

///
#define sid_assert_eq(Lhs, Rhs, ...)                                           \
    sid_check_eq(Lhs, Rhs, sid::dbg::assert_tag_t {} __VA_OPT__(, __VA_ARGS__))

///
#define sid_assert_ne(Lhs, Rhs, ...)                                           \
    sid_check_ne(Lhs, Rhs, sid::dbg::assert_tag_t {} __VA_OPT__(, __VA_ARGS__))

///
#define sid_assert_true(Expr, ...)                                             \
    sid_assert_eq(Expr, true __VA_OPT__(, __VA_ARGS__))

///
#define sid_assert_false(Expr, ...)                                            \
    sid_assert_eq(Expr, false __VA_OPT__(, __VA_ARGS__))

///
#define sid_assert(Expr, ...) sid_assert_true(Expr __VA_OPT__(, __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid_expect

///
#define sid_expect_eq(Lhs, Rhs, ...)                                           \
    sid_check_eq(Lhs, Rhs, sid::expect_tag __VA_OPT__(, __VA_ARGS__))

///
#define sid_expect_ne(Lhs, Rhs, ...)                                           \
    sid_check_ne(Lhs, Rhs, sid::expect_tag __VA_OPT__(, __VA_ARGS__))

///
#define sid_expect_true(Expr, ...)                                             \
    sid_expect_eq(Expr, true __VA_OPT__(, __VA_ARGS__))

///
#define sid_expect_false(Expr, ...)                                            \
    sid_expect_eq(Expr, false __VA_OPT__(, __VA_ARGS__))

///
#define sid_expect(Expr, ...) sid_expect_true(Expr __VA_OPT__(, __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid_ensure

///
#define sid_ensure_eq(Lhs, Rhs, ...)                                           \
    sid_check_eq(Lhs, Rhs, sid::ensure_tag __VA_OPT__(, __VA_ARGS__))

///
#define sid_ensure_ne(Lhs, Rhs, ...)                                           \
    sid_check_ne(Lhs, Rhs, sid::ensure_tag __VA_OPT__(, __VA_ARGS__))

///
#define sid_ensure_true(Expr, ...)                                             \
    sid_ensure_eq(Expr, true __VA_OPT__(, __VA_ARGS__))

///
#define sid_ensure_false(Expr, ...)                                            \
    sid_ensure_eq(Expr, false __VA_OPT__(, __VA_ARGS__))

///
#define sid_ensure(Expr, ...) sid_ensure_true(Expr __VA_OPT__(, __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

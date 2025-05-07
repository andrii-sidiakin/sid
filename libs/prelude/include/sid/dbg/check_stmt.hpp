#ifndef _SID_DBG_CHECK_STMT_HPP_
#define _SID_DBG_CHECK_STMT_HPP_

#include <sid/tseq.hpp>

#include <format>
#include <source_location>
#include <utility>

#ifdef __cpp_exceptions
#include <exception>
#endif

namespace sid::dbg {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
struct check_stmt {
    const char *cmp{};
    const char *lhs{};
    const char *rhs{};
    std::source_location src;
    std::string msg;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

///
template <class T> struct is_check_stmt_tag : std::false_type {};

///
template <class T>
    requires(std::is_reference_v<T>)
struct is_check_stmt_tag<T> : is_check_stmt_tag<std::remove_cvref_t<T>> {};

///
template <class T>
inline constexpr auto is_check_stmt_tag_v = is_check_stmt_tag<T>::value;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace _check_stmt_ns {
template <typename... Tags> struct check_stmt_tag {};
} // namespace _check_stmt_ns

///
template <typename... Tags>
using check_stmt_tag = _check_stmt_ns::check_stmt_tag<Tags...>;

///
template <typename... Tags>
    requires(... and is_check_stmt_tag_v<Tags>)
struct is_check_stmt_tag<check_stmt_tag<Tags...>> : std::true_type {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace _check_stmt_ns {

//
// makes a check_stmt_tag<...> using those Ts where is_check_stmt_tag<T> is true
//
struct make_check_stmt_tag_op {

    template <typename... Ts>
    static constexpr auto make_tag(mt::tseq<Ts...> /*tag*/) noexcept {
        return check_stmt_tag<Ts...>{};
    };

    template <typename... Ts>
    constexpr auto operator()(Ts &&.../*args*/) const noexcept {
        using tag_seq_t = mt::select_if_t<is_check_stmt_tag, mt::tseq<Ts...>>;
        return make_tag(tag_seq_t{});
    }
};

//
// from a given args in form of (tags..., format_string<As> fmt, as...)
// makes a callable: [](){ return std::format(fmt, as...); }
//
template <typename Tags> struct skip_tags_format_op;
//
template <typename... Tags>
struct skip_tags_format_op<check_stmt_tag<Tags...>> {

    constexpr auto operator()(Tags... /*tag*/) const noexcept {
        return []() { return std::string{}; };
    }

    template <typename... Args>
    constexpr auto operator()(Tags &&.../*tag*/,
                              std::format_string<Args...> fmt,
                              Args &&...args) const noexcept {
        return [fmt, &args...]() {
            return std::format(fmt, std::forward<Args>(args)...);
        };
    }
};

//
// a violation handler fallback (if no tags provided)
//
[[noreturn]]
void handle_stmt_violation(check_stmt_tag<> /*tag*/,
                           const check_stmt & /*stmt*/) {
#ifdef __cpp_exceptions
    std::terminate();
#else
    std::abort();
#endif
    std::unreachable();
}

//
// in a reverse order calls handlers from a chain
//
template <typename Tag, typename... Tags>
constexpr bool handle_stmt_violation(check_stmt_tag<Tag, Tags...> /*tag*/,
                                     const check_stmt &stmt) {
    // first, call tailed handlers until any one handles the issue
    if constexpr (sizeof...(Tags)) {
        if (handle_stmt_violation(check_stmt_tag<Tags...>{}, stmt)) {
            return true;
        }
    }

    // than, call the top one and return its status
    using ret_t = decltype(handle_stmt_violation(Tag{}, stmt));
    if constexpr (std::same_as<ret_t, bool>) {
        return handle_stmt_violation(Tag{}, stmt);
    }
    else {
        handle_stmt_violation(Tag{}, stmt);
        return false;
    }
}

//
//
struct emit_violated_stmt_fn {
    template <class Tag>
    constexpr auto operator()(Tag &&tag, check_stmt &&stmt) const {
        static_assert(
            requires { handle_stmt_violation(tag, std::move(stmt)); },
            "No violation handler found.");
        handle_stmt_violation(std::forward<Tag>(tag), std::move(stmt));
    }
};

} // namespace _check_stmt_ns

///
inline constexpr _check_stmt_ns::make_check_stmt_tag_op make_check_stmt_tag{};

///
template <typename Tags>
inline constexpr _check_stmt_ns::skip_tags_format_op<Tags> skip_tags_format{};

///
inline constexpr _check_stmt_ns::emit_violated_stmt_fn emit_violated_stmt{};

} // namespace sid::dbg

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _sid_dbg_escape(...) __VA_ARGS__
#define _sid_dbg_strfy(...) #__VA_ARGS__

//
#define _sid_dbg_make_lazy_format(...)                                         \
    sid::dbg::skip_tags_format<decltype(sid::dbg::make_check_stmt_tag(         \
        __VA_ARGS__))>(__VA_ARGS__)

///
#define sid_check_stmt_impl(Lhs, Cmp, Rhs, ...)                                \
    [](auto tag, bool likely_true, auto source, auto get_msg) {                \
        if (likely_true) [[likely]] {                                          \
            return true;                                                       \
        }                                                                      \
        else {                                                                 \
            sid::dbg::emit_violated_stmt(std::move(tag),                       \
                                         sid::check_stmt{                      \
                                             .cmp = _sid_dbg_strfy(Cmp),       \
                                             .lhs = _sid_dbg_strfy(Lhs),       \
                                             .rhs = _sid_dbg_strfy(Rhs),       \
                                             .src = source,                    \
                                             .msg = get_msg(),                 \
                                         });                                   \
            return false;                                                      \
        }                                                                      \
    }(sid::dbg::make_check_stmt_tag(__VA_ARGS__),                              \
      _sid_dbg_escape((Lhs)Cmp(Rhs)), std::source_location::current(),         \
      _sid_dbg_make_lazy_format(__VA_ARGS__))

///
#define sid_check_eq(Lhs, Rhs, ...)                                            \
    sid_check_stmt_impl(_sid_dbg_escape(Lhs), _sid_dbg_escape(==),             \
                        _sid_dbg_escape(Rhs) __VA_OPT__(, __VA_ARGS__))

///
#define sid_check_ne(Lhs, Rhs, ...)                                            \
    sid_check_stmt_impl(_sid_dbg_escape(Lhs), _sid_dbg_escape(!=),             \
                        _sid_dbg_escape(Rhs) __VA_OPT__(, __VA_ARGS__))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

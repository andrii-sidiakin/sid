#ifndef _SID_NET_ERROR_HPP_INCLUDED_
#define _SID_NET_ERROR_HPP_INCLUDED_

#include <system_error>

namespace sid::net {

enum class fail_code : int {
    already_open = 1, ///< attempt to open already open
};

enum class fail_condition : int {
    open_failure = 1,
};

namespace details {

class error_category final : public std::error_category {
  public:
    const char *name() const noexcept override { return "network"; }

    std::string message(int code) const override;

    std::error_condition
    default_error_condition(int code) const noexcept override;
};

} // namespace details

constexpr std::error_category &error_category() noexcept {
    static net::details::error_category cat{};
    return cat;
}

constexpr std::error_code make_error_code(fail_code e) {
    return std::error_code(static_cast<int>(e), net::error_category());
}

constexpr std::error_condition make_error_condition(fail_condition e) {
    return std::error_condition(static_cast<int>(e), net::error_category());
}

} // namespace sid::net

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace std {

template <>
struct is_error_code_enum<sid::net::fail_code> : public true_type {};

template <>
struct is_error_condition_enum<sid::net::fail_condition> : public true_type {};

} // namespace std

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

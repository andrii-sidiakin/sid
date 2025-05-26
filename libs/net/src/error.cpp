#include "sid/net/error.hpp"

namespace sid::net {

namespace details {

std::string error_category::message(int code) const {
    switch (static_cast<fail_code>(code)) {
    case fail_code::already_open:
        return "Already open";
    }
    return "Unknown error";
}

std::error_condition
error_category::default_error_condition(int code) const noexcept {
    switch (static_cast<fail_code>(code)) {
    case fail_code::already_open:
        return make_error_condition(fail_condition::open_failure);
    }
    return default_error_condition(code);
}

} // namespace details

} // namespace sid::net

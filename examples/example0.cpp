#include <sid/assert.hpp>

namespace lib {

struct custom_tag_t {};
inline constexpr custom_tag_t custom_tag{};

constexpr bool handle_stmt_violation(custom_tag_t /*tag*/,
                                     const sid::check_stmt &stmt) {

    sid::dbg::print_stmt_to_stderr("Statement Violation ", stmt);
    return true; // do not propagate
}

} // namespace lib

template <>
struct sid::is_check_stmt_tag<lib::custom_tag_t> : std::true_type {};

int main(int argc, char * /*argv*/[]) {
    sid_assert(argc > 1, lib::custom_tag, "argc={}", argc);
    sid_expect(argc > 2, lib::custom_tag, "argc={}", argc);
    sid_ensure(argc > 3, lib::custom_tag, "argc={}", argc);

    sid_assert(argc > 1, "argc={}", argc);
    sid_expect(argc > 2, "argc={}", argc);
    sid_ensure(argc > 3, "argc={}", argc);

    return 0;
}

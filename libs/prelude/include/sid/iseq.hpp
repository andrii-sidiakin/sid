#ifndef _SID_MT_ISEQ_HPP_
#define _SID_MT_ISEQ_HPP_

#include <cstdlib>

namespace sid::mt {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.iseq

/// sequence of indices
template <std::size_t... Is> struct iseq {};

///
template <std::size_t... Is> consteval std::size_t size(iseq<Is...> /*seq*/) {
    return sizeof...(Is);
}

///
template <std::size_t N> consteval auto make_iseq() {
    constexpr auto impl = []<std::size_t... Is>(this auto self, iseq<Is...>) {
        if constexpr (sizeof...(Is) < N) {
            return self(iseq<Is..., sizeof...(Is)>{});
        }
        else {
            return iseq<Is...>{};
        }
    };
    return impl(iseq<>{});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.iseq_for

///
template <typename... Ts> struct iseq_for {
    using type = decltype(make_iseq<sizeof...(Ts)>());
};

///
template <typename... Ts> using iseq_for_t = iseq_for<Ts...>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // namespace sid::mt

#endif

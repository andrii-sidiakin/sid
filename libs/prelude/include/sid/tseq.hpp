#ifndef _SID_MT_TSEQ_HPP_
#define _SID_MT_TSEQ_HPP_

#include <sid/iseq.hpp>

#include <type_traits>

namespace sid::mt {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.tseq

/// sequence of types
template <typename... Ts> struct tseq {};

///
template <typename... Ts> consteval std::size_t size(tseq<Ts...> /*seq*/) {
    return sizeof...(Ts);
}

///
template <typename... Ts> struct iseq_for<tseq<Ts...>> {
    using type = decltype(make_iseq<sizeof...(Ts)>());
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.tseq_elem

///
template <std::size_t I, typename TT> struct tseq_elem;

///
template <std::size_t I, typename TT>
using tseq_elem_t = tseq_elem<I, TT>::type;

///
template <std::size_t I, typename... Ts> struct tseq_elem<I, tseq<Ts...>> {

    static_assert(I < sizeof...(Ts), "Index is out of range");

    template <std::size_t J, typename UU> struct impl;

    // stop rule: at J=0
    template <typename U0, typename... Us> struct impl<0, tseq<U0, Us...>> {
        using type = U0;
    };

    // recursive step: decrement J and drop Uj
    template <std::size_t J, typename Uj, typename... Us>
    struct impl<J, tseq<Uj, Us...>> {
        using type = impl<J - 1, tseq<Us...>>::type;
    };

    using type = impl<I, tseq<Ts...>>::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.copy_index_if

/// Defines a member type 'type' as a sequence of those indices from a given
/// sequence II for which Pred<TT[I]> holds.
///
/// Pre:  each I from II less than size(TT)
///
template <template <typename> class Pred, typename II, typename TT>
struct copy_index_if;

///
template <template <typename> class Pred, typename II, typename TT>
using copy_index_if_t = copy_index_if<Pred, II, TT>;

///
template <template <typename> class Pred, std::size_t... Is, typename... Ts>
struct copy_index_if<Pred, iseq<Is...>, tseq<Ts...>> {

    static_assert((... and (Is < sizeof...(Ts))), "Index is out of range");

    // helper to test TT[J] with Pred
    template <std::size_t J>
    static constexpr auto is_match_v = Pred<tseq_elem_t<J, tseq<Ts...>>>::value;

    // copy indices from [JJ] to [KK] if Pred<Ti> is true
    template <typename JJ, typename KK> struct impl;

    // stop rule: empty([JJ])
    template <std::size_t... Ks> struct impl<iseq<>, iseq<Ks...>> {
        using type = iseq<Ks...>;
    };

    // recursive step: iterate over [JJ]
    template <std::size_t J, std::size_t... Js, std::size_t... Ks>
    struct impl<iseq<J, Js...>, iseq<Ks...>> {
        using type = impl<iseq<Js...>,                       // drop J
                          std::conditional_t<is_match_v<J>,  // test
                                             iseq<Ks..., J>, // copy
                                             iseq<Ks...>     // skip
                                             >>::type;
    };

    using type = impl<iseq<Is...>, iseq<>>::type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.index_if

/// Defines a member type 'type' as a sequence of indices for which Pred<TT[I]>
/// holds.
template <template <typename> class Pred, typename TT> struct index_if;

///
template <template <typename> class Pred, typename TT>
using index_if_t = index_if<Pred, TT>::type;

///
template <template <typename> class Pred, typename... Ts>
struct index_if<Pred, tseq<Ts...>>
    : copy_index_if<Pred, iseq_for_t<Ts...>, tseq<Ts...>> {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.select_index

///
template <typename Index, typename TT> struct select_index;

///
template <std::size_t... Is, typename... TS>
struct select_index<iseq<Is...>, tseq<TS...>> {
    static_assert((... and (Is < sizeof...(TS))), "Index is out of range");

    using type = tseq<tseq_elem_t<Is, tseq<TS...>>...>;
};

///
template <typename Index, typename... Types>
using select_index_t = select_index<Index, Types...>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.pred*

/// TODO: find a better name
template <template <typename> class Pred, bool Expect> struct pred_if_equal_to {
    template <typename T> struct type {
        static constexpr auto value = (Pred<T>::value == Expect);
    };
};

///
template <template <typename> class Pred>
using pred_if = pred_if_equal_to<Pred, true>;

///
template <template <typename> class Pred>
using pred_if_not = pred_if_equal_to<Pred, false>;

///
template <typename> struct pred_always_true : std::true_type {};

///
template <typename> struct pred_always_false : std::false_type {};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.select_if

///
template <template <typename> class Pred, typename TT> struct select_if {
    using index_type = index_if_t<pred_if<Pred>::template type, TT>;
    using type = select_index_t<index_type, TT>;
};

///
template <template <typename> class Pred, typename Tuple>
using select_if_t = select_if<Pred, Tuple>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// sid.mt.select_if_not

///
///
template <template <typename> class Pred, typename TT> struct select_if_not {
    using index_type = index_if_t<pred_if_not<Pred>::template type, TT>;
    using type = select_index_t<index_type, TT>;
};

///
template <template <typename> class Pred, typename Tuple>
using select_if_not_t = select_if_not<Pred, Tuple>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// ...

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} // namespace sid::mt

#endif

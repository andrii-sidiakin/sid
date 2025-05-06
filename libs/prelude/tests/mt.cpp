#include <sid/iseq.hpp>
#include <sid/tseq.hpp>

#include <concepts>

void test_mt_xseq_for() {
    using namespace sid::mt;

    static_assert(std::same_as<iseq_for_t<>, iseq<>>);
    static_assert(std::same_as<iseq_for_t<int>, iseq<0>>);
    static_assert(std::same_as<iseq_for_t<int, int>, iseq<0, 1>>);

    static_assert(std::same_as<iseq_for_t<tseq<>>, iseq<>>);
    static_assert(std::same_as<iseq_for_t<tseq<int>>, iseq<0>>);
    static_assert(std::same_as<iseq_for_t<tseq<int, int>>, iseq<0, 1>>);
}

template <class TT>
using integrals_t = sid::mt::select_if_t<std::is_integral, TT>;

template <class TT>
using not_integrals_t = sid::mt::select_if_not_t<std::is_integral, TT>;

void test_mt_select() {
    using namespace sid::mt;

    static_assert(std::same_as<integrals_t<tseq<>>, tseq<>>);
    static_assert(std::same_as<not_integrals_t<tseq<>>, tseq<>>);

    static_assert(std::same_as<integrals_t<tseq<int>>, tseq<int>>);
    static_assert(std::same_as<integrals_t<tseq<float>>, tseq<>>);
    static_assert(std::same_as<integrals_t<tseq<int, float>>, tseq<int>>);
    static_assert(std::same_as<integrals_t<tseq<float, int>>, tseq<int>>);

    static_assert(std::same_as<not_integrals_t<tseq<int>>, tseq<>>);
    static_assert(std::same_as<not_integrals_t<tseq<float>>, tseq<float>>);
    static_assert(std::same_as<not_integrals_t<tseq<int, float>>, tseq<float>>);
    static_assert(std::same_as<not_integrals_t<tseq<float, int>>, tseq<float>>);

    static_assert(std::same_as<integrals_t<tseq<float, int, int>>,
                               integrals_t<tseq<int, int, float>>>);
    static_assert(std::same_as<integrals_t<tseq<int, float, int>>,
                               integrals_t<tseq<int, int, float>>>);

    static_assert(std::same_as<not_integrals_t<tseq<float, int, int>>,
                               not_integrals_t<tseq<int, int, float>>>);
    static_assert(std::same_as<not_integrals_t<tseq<int, float, int>>,
                               not_integrals_t<tseq<int, int, float>>>);
}

int main() {
    test_mt_xseq_for();
    test_mt_select();
    return 0;
}

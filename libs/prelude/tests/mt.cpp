#include <sid/iseq.hpp>
#include <sid/tseq.hpp>

#include <concepts>

void test_mt() {
    using namespace sid::mt;

    static_assert(std::same_as<iseq_for_t<>, iseq<>>);
    static_assert(std::same_as<iseq_for_t<int>, iseq<0>>);
    static_assert(std::same_as<iseq_for_t<int, int>, iseq<0, 1>>);

    static_assert(std::same_as<iseq_for_t<tseq<>>, iseq<>>);
    static_assert(std::same_as<iseq_for_t<tseq<int>>, iseq<0>>);
    static_assert(std::same_as<iseq_for_t<tseq<int, int>>, iseq<0, 1>>);
}

int main() {
    test_mt();
    return 0;
}

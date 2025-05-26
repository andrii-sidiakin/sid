#include "sid/bio/buffer.hpp"
#include <sid/bio/basic_buffer.hpp>

#include <array>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("basic") {
    sid::bio::basic_buffer<char> a;
    CHECK_EQ(a.size(), 0);
    CHECK_EQ(a.data(), nullptr);

    sid::bio::basic_buffer<char, std::dynamic_extent> b;
    CHECK_EQ(b.size(), 0);
    CHECK_EQ(b.data(), nullptr);

    sid::bio::basic_buffer c{"123"};
    CHECK_EQ(c.size(), 4);
    CHECK_EQ(c.data()[0], std::byte{'1'});
    CHECK_EQ(c.data()[1], std::byte{'2'});
    CHECK_EQ(c.data()[2], std::byte{'3'});
    CHECK_EQ(c.data()[3], std::byte{0});

    std::array arr{1, 2, 3};
    sid::bio::basic_buffer d{arr};
    CHECK_EQ(d.size(), sizeof(int) * 3);
    const int *ints_123 = reinterpret_cast<const int *>(d.data());
    CHECK_EQ(ints_123[0], 1);
    CHECK_EQ(ints_123[1], 2);
    CHECK_EQ(ints_123[2], 3);

    short shorts[] = {1, 2, 3};
    sid::bio::basic_buffer e{shorts};
    CHECK_EQ(e.size(), sizeof(short) * 3);
    short *shorts_123 = reinterpret_cast<short *>(e.data());
    CHECK_EQ(shorts_123[0], 1);
    CHECK_EQ(shorts_123[1], 2);
    CHECK_EQ(shorts_123[2], 3);
}

template <class T>
constexpr bool is_const_buffer_allowed_from =
    requires { sid::bio::const_buffer{T{}}; };

template <class T>
constexpr bool is_const_buffer_allowed_from_moved =
    requires { sid::bio::const_buffer{std::move(T{})}; };

TEST_CASE("const_buffer.construct_from") {
    CHECK(is_const_buffer_allowed_from<decltype("chars")>);
    CHECK(is_const_buffer_allowed_from<std::string_view>);
    CHECK(is_const_buffer_allowed_from<std::wstring_view>);

    CHECK_FALSE(is_const_buffer_allowed_from<std::string>);
    CHECK_FALSE(is_const_buffer_allowed_from<std::array<int, 3>>);
    CHECK_FALSE(is_const_buffer_allowed_from<std::vector<float>>);

    CHECK_FALSE(is_const_buffer_allowed_from_moved<std::string>);
    CHECK_FALSE(is_const_buffer_allowed_from_moved<std::array<int, 3>>);
    CHECK_FALSE(is_const_buffer_allowed_from_moved<std::vector<float>>);
}

template <class T>
constexpr bool is_mutable_buffer_allowed_from =
    requires { sid::bio::mutable_buffer{T{}}; };

template <class T>
constexpr bool is_mutable_buffer_allowed_from_moved =
    requires { sid::bio::mutable_buffer{std::move(T{})}; };

TEST_CASE("mutable_buffer.mutableruct_from") {
    CHECK_FALSE(is_mutable_buffer_allowed_from<decltype("chars")>);
    CHECK_FALSE(is_mutable_buffer_allowed_from<std::string_view>);
    CHECK_FALSE(is_mutable_buffer_allowed_from<std::wstring_view>);

    CHECK_FALSE(is_mutable_buffer_allowed_from<std::string>);
    CHECK_FALSE(is_mutable_buffer_allowed_from<std::array<int, 3>>);
    CHECK_FALSE(is_mutable_buffer_allowed_from<std::vector<float>>);

    CHECK_FALSE(is_mutable_buffer_allowed_from_moved<std::string>);
    CHECK_FALSE(is_mutable_buffer_allowed_from_moved<std::array<int, 3>>);
    CHECK_FALSE(is_mutable_buffer_allowed_from_moved<std::vector<float>>);
}

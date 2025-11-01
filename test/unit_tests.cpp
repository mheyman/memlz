#include <algorithm>
#include <doctest/doctest.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <memlz.h>
#include <ranges>
#include <vector>

#include "doctest_util.h"

namespace
{
	template<typename T>
    auto encode(std::vector<T> in) -> std::vector<uint8_t>
    {
        auto byte_count {in.size() * sizeof(T)};
		auto ret{ std::vector<uint8_t>(memlz_max_compressed_len(byte_count)) };
        ret.resize(memlz_compress(ret.data(), in.data(), byte_count));
		return ret;
    }

	template<typename T>
	auto decode(std::vector<uint8_t> in) -> std::vector<T>
	{
        size_t byte_count = memlz_decompressed_len(in.data());
		if (byte_count % sizeof(T) != 0)
		{
			throw std::runtime_error("memlz.decode: corrupted data");
		}

        size_t count = byte_count / sizeof(T);
		auto ret{ std::vector<T>(count) };
		memlz_decompress(ret.data(), in.data());
		return ret;
	}

}

TEST_CASE("memlz.encode_decode")
{
	auto truth{ std::views::iota(static_cast<size_t>(0), static_cast<size_t>(1'000'000)) | std::ranges::to<std::vector>() };
	auto compressed{ encode(truth) };
	auto check{ decode<size_t>(compressed) };
	CHECK_EQ(check.size(), truth.size());
	std::ranges::for_each(std::views::zip(truth, check), [](auto&& v)
		{
			auto [t, c] {v};
			CHECK_EQ(t, c);
		});
    fmt::print("{} {}/{}, {:0.5f} seconds\n", get_current_test_name(), get_current_test_assert_count() - get_current_test_assert_failed_count(), get_current_test_assert_count(), get_current_test_elapsed());
}

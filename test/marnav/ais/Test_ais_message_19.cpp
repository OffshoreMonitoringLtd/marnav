#include <marnav/ais/message_19.hpp>
#include <marnav/ais/ais.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_ais_message_19 : public ::testing::Test
{
};

TEST_F(test_ais_message_19, parse)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("C000000000H0htY08D41qwv0000000000000000000000000000@", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_19>(result);
	ASSERT_TRUE(m != nullptr);
}

TEST_F(test_ais_message_19, wrong_number_of_bits)
{
	EXPECT_ANY_THROW(ais::message_parse<ais::message_19>(ais::raw(311)));
	EXPECT_ANY_THROW(ais::message_parse<ais::message_19>(ais::raw(313)));
}

TEST_F(test_ais_message_19, encode_default_values)
{
	ais::message_19 m;

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("C0000003wk?8mP=18D3Q3wv0000000000000000000000000000P", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_19, set_lat)
{
	ais::message_19 m;
	m.set_lat(geo::latitude{12.34});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("C0000003wk?8mP1hvN3Q3wv0000000000000000000000000000P", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_19, get_lat)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("C000000003?8mP1hvN3Q3wv00000000000000000000000000000", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_19>(result);
	ASSERT_TRUE(m != nullptr);

	const geo::latitude expected{12.34};
	const geo::latitude decoded = *m->get_lat();

	EXPECT_DOUBLE_EQ(expected, decoded);
}

TEST_F(test_ais_message_19, set_lon)
{
	ais::message_19 m;
	m.set_lon(geo::longitude{123.45});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("C0000003wj=Agp=18D3Q3wv0000000000000000000000000000P", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_19, get_lon)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("C000000002=Agp=18D3Q3wv00000000000000000000000000000", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_19>(result);
	ASSERT_TRUE(m != nullptr);

	const geo::longitude expected{123.45};
	const geo::longitude decoded = *m->get_lon();

	EXPECT_DOUBLE_EQ(expected, decoded);
}
}

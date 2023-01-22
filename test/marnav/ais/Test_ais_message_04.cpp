#include <marnav/ais/message_04.hpp>
#include <marnav/ais/ais.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_ais_message_04 : public ::testing::Test
{
};

TEST_F(test_ais_message_04, parse)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("4020ssAuho;N?PeNwjOAp<70089A", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_04>(result);
	ASSERT_TRUE(m != nullptr);
}

TEST_F(test_ais_message_04, wrong_number_of_bits)
{
	EXPECT_ANY_THROW(ais::message_parse<ais::message_04>(ais::raw(167)));
	EXPECT_ANY_THROW(ais::message_parse<ais::message_04>(ais::raw(169)));
}

TEST_F(test_ais_message_04, encode_default_values)
{
	ais::message_04 m;

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("4000000000Htt<tSF0l4Q@000000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_04, set_lat)
{
	ais::message_04 m;
	m.set_lat(geo::latitude{12.34});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("4000000000Htt<tSF073qp000000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_04, get_lat)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("4000000000Htt<tSF073qp000000", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_04>(result);
	ASSERT_TRUE(m != nullptr);

	const geo::latitude expected{12.34};
	const geo::latitude decoded = *m->get_lat();

	EXPECT_DOUBLE_EQ(expected, decoded);
}

TEST_F(test_ais_message_04, set_lon)
{
	ais::message_04 m;
	m.set_lon(geo::longitude{123.45});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("4000000000Htt8m6wPl4Q@000000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(test_ais_message_04, get_lon)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.emplace_back("4000000000Htt8m6wPl4Q@000000", 0);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_04>(result);
	ASSERT_TRUE(m != nullptr);

	const geo::longitude expected{123.45};
	const geo::longitude decoded = *m->get_lon();

	EXPECT_DOUBLE_EQ(expected, decoded);
}
}

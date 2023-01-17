#include <marnav/ais/ais.hpp>
#include <marnav/ais/message.hpp>
#include <marnav/ais/message_01.hpp>
#include <marnav/ais/rate_of_turn.hpp>
#include <marnav/geo/angle.hpp>
#include <marnav/nmea/ais_helper.hpp>
#include <marnav/nmea/vdm.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class Test_ais_message_01 : public ::testing::Test
{
};

TEST_F(Test_ais_message_01, parse)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.push_back(std::make_pair("133m@ogP00PD;88MD5MTDww@2D7k", 0));

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m = ais::message_cast<ais::message_01>(result);
	ASSERT_TRUE(m != nullptr);

	EXPECT_EQ(0u, m->get_repeat_indicator());
	EXPECT_EQ(205344990u, m->get_mmsi());
	EXPECT_FALSE(m->get_rot().available());
	EXPECT_NEAR(0.0, m->get_sog()->value(), 1e-4);
	EXPECT_EQ(true, m->get_position_accuracy());
	auto lon = m->get_lon();
	EXPECT_TRUE(!!lon);
	EXPECT_NEAR(4.40705, lon.value(), 4e-5);
	auto lat = m->get_lat();
	EXPECT_TRUE(!!lat);
	EXPECT_NEAR(51.2296, lat.value(), 4e-5);
	EXPECT_NEAR(110.7, *m->get_cog(), 1e-5);
	EXPECT_TRUE(!m->get_hdg());
	EXPECT_EQ(40u, m->get_timestamp());
	EXPECT_EQ(ais::maneuver_indicator_id::not_available, m->get_maneuver_indicator());
	EXPECT_EQ(true, m->get_raim());
	EXPECT_EQ(82419u, m->get_radio_status());
}

TEST_F(Test_ais_message_01, wrong_number_of_bits)
{
	EXPECT_ANY_THROW(ais::message_parse<ais::message_01>(ais::raw(167)));
	EXPECT_ANY_THROW(ais::message_parse<ais::message_01>(ais::raw(169)));
}

TEST_F(Test_ais_message_01, encode_default_values)
{
	ais::message_01 m;

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("100000?P?w<tSF0l4Q@>4?wp0000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(Test_ais_message_01, set_lat)
{
	ais::message_01 m;
	m.set_lat(geo::latitude{12.34});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("100000?P?w<tSF073qp>4?wp0000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(Test_ais_message_01, set_lon)
{
	ais::message_01 m;
	m.set_lon(geo::longitude{123.45});

	auto v = ais::encode_message(m);

	ASSERT_EQ(1u, v.size());
	EXPECT_STREQ("100000?P?w8m6wPl4Q@>4?wp0000", v[0].first.c_str());
	EXPECT_EQ(0u, v[0].second);
}

TEST_F(Test_ais_message_01, github_issue_2)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.push_back(std::make_pair("15RTgt0PAso;90TKcjM8h6g208CQ", 0));

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	const auto m = ais::message_cast<ais::message_01>(result);
	ASSERT_TRUE(m != nullptr);

	EXPECT_NEAR(-123.3954, *m->get_lon(), 1e-4);
	EXPECT_NEAR(48.3816, *m->get_lat(), 1e-4);
}

TEST_F(Test_ais_message_01, github_issue_51_decode)
{
	std::vector<std::pair<std::string, uint32_t>> v;
	v.push_back(std::make_pair("15RTgt0PAso;90TKcjM8h6g208CQ", 0));

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	const auto m = ais::message_cast<ais::message_01>(result);
	ASSERT_TRUE(m != nullptr);

	EXPECT_FALSE(m->get_repeat_indicator());
	EXPECT_EQ(utils::mmsi{371798000}, m->get_mmsi());
	EXPECT_EQ(ais::navigation_status::under_way_using_engine, m->get_nav_status());
	EXPECT_EQ(-127, m->get_rot().raw());
	EXPECT_NEAR(12.3, m->get_sog()->value(), 1e-4);
	EXPECT_TRUE(m->get_position_accuracy());
	EXPECT_NEAR(-123.3954, *m->get_lon(), 1e-4);
	EXPECT_NEAR(48.3816, *m->get_lat(), 1e-4);
	EXPECT_NEAR(224.0, *m->get_cog(), 1e-4);
	EXPECT_EQ(215, *m->get_hdg());
	EXPECT_EQ(33, m->get_timestamp());
	EXPECT_EQ(ais::maneuver_indicator_id::not_available, m->get_maneuver_indicator());
	EXPECT_FALSE(m->get_raim());
	EXPECT_EQ(34017, m->get_radio_status());
}

TEST_F(Test_ais_message_01, github_issue_51_encode)
{
	ais::message_01 m;
	m.set_repeat_indicator(0);
	m.set_mmsi(utils::mmsi{371798000});
	m.set_nav_status(ais::navigation_status::under_way_using_engine);
	m.set_rot(ais::rate_of_turn(static_cast<int8_t>(-127)));
	m.set_sog(units::knots{12.3});
	m.set_position_accuracy(true);
	m.set_lon(geo::longitude(-123.395382));
	m.set_lat(geo::latitude(48.3816, geo::latitude::hemisphere::north));
	m.set_cog(224.0);
	m.set_hdg(215);
	m.set_timestamp(33);
	m.set_maneuver_indicator(ais::maneuver_indicator_id::not_available);
	m.set_raim(false);
	m.set_radio_status(34017);

	const auto payload = ais::encode_message(m);
	const auto sentences = nmea::make_vdms(payload, {}, nmea::ais_channel::A);

	ASSERT_EQ(1u, sentences.size());
	EXPECT_STREQ("!AIVDM,1,1,,A,15RTgt0PAso;90TKcjH8h6g208CQ,0*4F", nmea::to_string(*sentences.front()).c_str());
}

TEST_F(Test_ais_message_01, error_bit_length)
{
	std::vector<std::pair<std::string, uint32_t>> v;

	// message was encountered near Gibraltar in 2016
	v.push_back(std::make_pair("13miaA70120H5DvQlv5IRWeF05ItGbgv>1", 0));

	EXPECT_ANY_THROW(ais::make_message(v));
}

TEST_F(Test_ais_message_01, set_invalid_sog)
{
	ais::message_01 m;

	EXPECT_ANY_THROW(m.set_sog(units::knots{-1.0}));
}

TEST_F(Test_ais_message_01, set_get_sog)
{
	ais::message_01 m;

	m.set_sog(units::knots{4.5});

	EXPECT_NEAR(4.5, m.get_sog()->value(), 1e-6);
	EXPECT_EQ(marnav::units::knots{4.5}, *m.get_sog());
}

TEST_F(Test_ais_message_01, sog_max_value)
{
	ais::message_01 m;

	m.set_sog(units::knots{10000.0}); // over SOG max value

	EXPECT_TRUE(m.get_sog().has_value());
	EXPECT_EQ(marnav::units::knots{102.2}, *m.get_sog());
}

TEST_F(Test_ais_message_01, sog_not_available)
{
	ais::message_01 m;

	EXPECT_FALSE(m.get_sog().has_value());

	m.set_sog(units::knots{4.5});

	EXPECT_TRUE(m.get_sog().has_value());

	m.set_sog({});

	EXPECT_TRUE(m.get_sog().has_value());

	m.set_sog_unavailable();

	EXPECT_FALSE(m.get_sog().has_value());
}

TEST_F(Test_ais_message_01, github_issue_51_negative_east)
{
	const auto deg = -3.3118; // negative means western hemisphere

	ais::message_01 m;

	m.set_lon(geo::longitude{deg});
	const auto lon = m.get_lon();

	ASSERT_TRUE(lon);
	EXPECT_EQ(geo::longitude::hemisphere::west, lon->hem());
	EXPECT_NEAR(deg, lon->get(), 1e-5);
}

TEST_F(Test_ais_message_01, github_issue_51_roundtrip_negative_east)
{
	const auto deg = -3.3118; // negative means western hemisphere

	ais::message_01 m;
	m.set_lon(geo::longitude{deg});

	const auto v = ais::encode_message(m);

	auto result = ais::make_message(v);
	ASSERT_TRUE(result != nullptr);

	auto m1 = ais::message_cast<ais::message_01>(result);
	ASSERT_TRUE(m1 != nullptr);

	const auto lon = m1->get_lon();
	ASSERT_TRUE(lon);
	EXPECT_EQ(geo::longitude::hemisphere::west, lon->hem());
	EXPECT_NEAR(deg, lon->get(), 1e-5);
}
}

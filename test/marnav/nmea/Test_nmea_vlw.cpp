#include <marnav/nmea/vlw.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_vlw : public ::testing::Test
{
};

TEST_F(test_nmea_vlw, contruction)
{
	EXPECT_NO_THROW(nmea::vlw vlw);
}

TEST_F(test_nmea_vlw, properties)
{
	nmea_sentence_traits<nmea::vlw>();
}

TEST_F(test_nmea_vlw, parse)
{
	auto s = nmea::make_sentence("$IIVLW,7803.2,N,0.00,N*43");
	ASSERT_NE(nullptr, s);

	auto vlw = nmea::sentence_cast<nmea::vlw>(s);
	ASSERT_NE(nullptr, vlw);

	auto distance = vlw->get_distance_cum();
	EXPECT_TRUE(distance.has_value());
	EXPECT_NEAR(7803.2, distance->get<units::nautical_miles>().value(), 1e-8);
}

TEST_F(test_nmea_vlw, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vlw>(nmea::talker::none, {3, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vlw>(nmea::talker::none, {5, "@"}));
}

TEST_F(test_nmea_vlw, empty_to_string)
{
	nmea::vlw vlw;

	EXPECT_STREQ("$IIVLW,,,,*4D", nmea::to_string(vlw).c_str());
}

TEST_F(test_nmea_vlw, set_distance_cum_nm)
{
	nmea::vlw vlw;
	vlw.set_distance_cum_nm(units::nautical_miles{123.4});

	EXPECT_STREQ("$IIVLW,123.4,N,,*29", nmea::to_string(vlw).c_str());
}

TEST_F(test_nmea_vlw, set_distance_reset_nm)
{
	nmea::vlw vlw;
	vlw.set_distance_reset_nm(units::nautical_miles{12.4});

	EXPECT_STREQ("$IIVLW,,,12.4,N*1A", nmea::to_string(vlw).c_str());
}
}

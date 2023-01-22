#include <marnav/nmea/vwr.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_vwr : public ::testing::Test
{
};

TEST_F(test_nmea_vwr, contruction)
{
	EXPECT_NO_THROW(nmea::vwr vwr);
}

TEST_F(test_nmea_vwr, properties)
{
	nmea_sentence_traits<nmea::vwr>();
}

TEST_F(test_nmea_vwr, parse)
{
	auto s = nmea::make_sentence("$IIVWR,084.0,R,10.4,N,5.4,M,19.3,K*4A");
	ASSERT_NE(nullptr, s);

	auto vwr = nmea::sentence_cast<nmea::vwr>(s);
	ASSERT_NE(nullptr, vwr);

	auto angle = vwr->get_angle();
	EXPECT_TRUE(angle.has_value());
	EXPECT_NEAR(84.0, angle.value(), 1e-8);

	auto side = vwr->get_angle_side();
	EXPECT_TRUE(side.has_value());
	EXPECT_EQ(nmea::side::right, side.value());
}

TEST_F(test_nmea_vwr, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vwr>(nmea::talker::none, {7, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vwr>(nmea::talker::none, {9, "@"}));
}

TEST_F(test_nmea_vwr, empty_to_string)
{
	nmea::vwr vwr;

	EXPECT_STREQ("$IIVWR,,,,,,,,*53", nmea::to_string(vwr).c_str());
}

TEST_F(test_nmea_vwr, set_angle_left)
{
	nmea::vwr vwr;
	vwr.set_angle(22.5, nmea::side::left);

	EXPECT_STREQ("$IIVWR,22.5,L,,,,,,*04", nmea::to_string(vwr).c_str());
}

TEST_F(test_nmea_vwr, set_angle_right)
{
	nmea::vwr vwr;
	vwr.set_angle(22.5, nmea::side::right);

	EXPECT_STREQ("$IIVWR,22.5,R,,,,,,*1A", nmea::to_string(vwr).c_str());
}

TEST_F(test_nmea_vwr, set_speed_knots)
{
	nmea::vwr vwr;
	vwr.set_speed_knots(units::knots{12.5});

	EXPECT_STREQ("$IIVWR,,,12.5,N,,,,*05", nmea::to_string(vwr).c_str());
}

TEST_F(test_nmea_vwr, set_speed_mps)
{
	nmea::vwr vwr;
	vwr.set_speed_mps(units::meters_per_second{2.5});

	EXPECT_STREQ("$IIVWR,,,,,2.5,M,,*37", nmea::to_string(vwr).c_str());
}

TEST_F(test_nmea_vwr, set_speed_kmh)
{
	nmea::vwr vwr;
	vwr.set_speed_kmh(units::kilometers_per_hour{13.5});

	EXPECT_STREQ("$IIVWR,,,,,,,13.5,K*01", nmea::to_string(vwr).c_str());
}
}

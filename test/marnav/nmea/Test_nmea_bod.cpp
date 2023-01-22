#include <marnav/nmea/bod.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_bod : public ::testing::Test
{
};

TEST_F(test_nmea_bod, contruction)
{
	EXPECT_NO_THROW(nmea::bod bod);
}

TEST_F(test_nmea_bod, properties)
{
	nmea_sentence_traits<nmea::bod>();
}

TEST_F(test_nmea_bod, parse)
{
	auto s = nmea::make_sentence("$GPBOD,,T,,M,,*47");
	ASSERT_NE(nullptr, s);

	auto bod = nmea::sentence_cast<nmea::bod>(s);
	ASSERT_NE(nullptr, bod);
}

TEST_F(test_nmea_bod, create_sentence)
{
	const auto s = nmea::create_sentence<nmea::bod>("$GPBOD,,T,,M,,*47");
	EXPECT_EQ(nmea::sentence_id::BOD, s.id());
}

TEST_F(test_nmea_bod, create_sentence_exception)
{
	EXPECT_ANY_THROW(nmea::create_sentence<nmea::bod>(""));
	EXPECT_ANY_THROW(nmea::create_sentence<nmea::bod>("$IIMWV,084.0,R,10.4,N,A*04"));
}

TEST_F(test_nmea_bod, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::bod>(nmea::talker::none, {5, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::bod>(nmea::talker::none, {7, "@"}));
}

TEST_F(test_nmea_bod, empty_to_string)
{
	nmea::bod bod;

	EXPECT_STREQ("$GPBOD,,,,,,*5E", nmea::to_string(bod).c_str());
}

TEST_F(test_nmea_bod, set_bearing_true)
{
	nmea::bod bod;
	bod.set_bearing_true(12.5);

	EXPECT_STREQ("$GPBOD,12.5,T,,,,*12", nmea::to_string(bod).c_str());
}

TEST_F(test_nmea_bod, set_bearing_magn)
{
	nmea::bod bod;
	bod.set_bearing_magn(10.2);

	EXPECT_STREQ("$GPBOD,,,10.2,M,,*0E", nmea::to_string(bod).c_str());
}

TEST_F(test_nmea_bod, set_waypoint_to)
{
	nmea::bod bod;
	bod.set_waypoint_to(nmea::waypoint{"wpt-to"});

	EXPECT_STREQ("$GPBOD,,,,,wpt-to,*1B", nmea::to_string(bod).c_str());
}

TEST_F(test_nmea_bod, set_waypoint_from)
{
	nmea::bod bod;
	bod.set_waypoint_from(nmea::waypoint{"wpt-from"});

	EXPECT_STREQ("$GPBOD,,,,,,wpt-from*16", nmea::to_string(bod).c_str());
}
}

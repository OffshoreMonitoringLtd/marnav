#include <marnav/nmea/mwd.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_mwd : public ::testing::Test
{
};

TEST_F(test_nmea_mwd, contruction)
{
	EXPECT_NO_THROW(nmea::mwd mwd);
}

TEST_F(test_nmea_mwd, properties)
{
	nmea_sentence_traits<nmea::mwd>();
}

TEST_F(test_nmea_mwd, parse)
{
	auto s = nmea::make_sentence("$WIMWD,12.4,T,,,,,,*0D");
	ASSERT_NE(nullptr, s);

	auto mwd = nmea::sentence_cast<nmea::mwd>(s);
	ASSERT_NE(nullptr, mwd);
}

TEST_F(test_nmea_mwd, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::mwd>(nmea::talker::none, {1, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::mwd>(nmea::talker::none, {3, "@"}));
}

TEST_F(test_nmea_mwd, empty_to_string)
{
	nmea::mwd mwd;

	EXPECT_STREQ("$WIMWD,,,,,,,,*40", nmea::to_string(mwd).c_str());
}

TEST_F(test_nmea_mwd, set_direction_true)
{
	nmea::mwd mwd;
	mwd.set_direction_true(12.3);

	EXPECT_STREQ("$WIMWD,12.3,T,,,,,,*0A", nmea::to_string(mwd).c_str());
}

TEST_F(test_nmea_mwd, set_direction_mag)
{
	nmea::mwd mwd;
	mwd.set_direction_mag(12.3);

	EXPECT_STREQ("$WIMWD,,,12.3,M,,,,*13", nmea::to_string(mwd).c_str());
}

TEST_F(test_nmea_mwd, set_speed_knot)
{
	nmea::mwd mwd;
	mwd.set_speed_knots(marnav::units::knots{12.3});

	EXPECT_STREQ("$WIMWD,,,,,12.3,N,,*10", nmea::to_string(mwd).c_str());
}

TEST_F(test_nmea_mwd, set_speed_mps)
{
	nmea::mwd mwd;
	mwd.set_speed_mps(marnav::units::meters_per_second{12.3});

	EXPECT_STREQ("$WIMWD,,,,,,,12.3,M*13", nmea::to_string(mwd).c_str());
}
}

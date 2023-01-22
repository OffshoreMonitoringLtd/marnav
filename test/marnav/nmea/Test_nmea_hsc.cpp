#include <marnav/nmea/hsc.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_hsc : public ::testing::Test
{
};

TEST_F(test_nmea_hsc, contruction)
{
	EXPECT_NO_THROW(nmea::hsc hsc);
}

TEST_F(test_nmea_hsc, properties)
{
	nmea_sentence_traits<nmea::hsc>();
}

TEST_F(test_nmea_hsc, parse)
{
	auto s = nmea::make_sentence("$GPHSC,45.8,T,,*0C");
	ASSERT_NE(nullptr, s);

	auto hsc = nmea::sentence_cast<nmea::hsc>(s);
	ASSERT_NE(nullptr, hsc);
}

TEST_F(test_nmea_hsc, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::hsc>(nmea::talker::none, {3, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::hsc>(nmea::talker::none, {5, "@"}));
}

TEST_F(test_nmea_hsc, empty_to_string)
{
	nmea::hsc hsc;

	EXPECT_STREQ("$GPHSC,,,,*4F", nmea::to_string(hsc).c_str());
}

TEST_F(test_nmea_hsc, set_heading_true)
{
	nmea::hsc hsc;
	hsc.set_heading_true(45.8);

	EXPECT_STREQ("$GPHSC,45.8,T,,*0C", nmea::to_string(hsc).c_str());
}

TEST_F(test_nmea_hsc, set_heading_mag)
{
	nmea::hsc hsc;
	hsc.set_heading_mag(45.8);

	EXPECT_STREQ("$GPHSC,,,45.8,M*15", nmea::to_string(hsc).c_str());
}
}

#include <marnav/nmea/vdr.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_vdr : public ::testing::Test
{
};

TEST_F(test_nmea_vdr, contruction)
{
	EXPECT_NO_THROW(nmea::vdr vdr);
}

TEST_F(test_nmea_vdr, properties)
{
	nmea_sentence_traits<nmea::vdr>();
}

TEST_F(test_nmea_vdr, parse)
{
	auto s = nmea::make_sentence("$IIVDR,211.0,T,1.00,M,1.25,N*3C");
	ASSERT_NE(nullptr, s);

	auto vdr = nmea::sentence_cast<nmea::vdr>(s);
	ASSERT_NE(nullptr, vdr);
}

TEST_F(test_nmea_vdr, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vdr>(nmea::talker::none, {5, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vdr>(nmea::talker::none, {7, "@"}));
}

TEST_F(test_nmea_vdr, empty_to_string)
{
	nmea::vdr vdr;

	EXPECT_STREQ("$IIVDR,,,,,,*40", nmea::to_string(vdr).c_str());
}

TEST_F(test_nmea_vdr, set_degrees_true)
{
	nmea::vdr vdr;
	vdr.set_degrees_true(45.8);

	EXPECT_STREQ("$IIVDR,45.8,T,,,,*03", nmea::to_string(vdr).c_str());
}

TEST_F(test_nmea_vdr, set_degrees_mag)
{
	nmea::vdr vdr;
	vdr.set_degrees_magn(13.5);

	EXPECT_STREQ("$IIVDR,,,13.5,M,,*14", nmea::to_string(vdr).c_str());
}

TEST_F(test_nmea_vdr, set_speed)
{
	nmea::vdr vdr;
	vdr.set_speed(units::knots{12.5});

	EXPECT_STREQ("$IIVDR,,,,,12.5,N*16", nmea::to_string(vdr).c_str());
}
}

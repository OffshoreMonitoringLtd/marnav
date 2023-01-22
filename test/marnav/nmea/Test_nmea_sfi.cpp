#include <marnav/nmea/sfi.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_sfi : public ::testing::Test
{
};

TEST_F(test_nmea_sfi, contruction)
{
	EXPECT_NO_THROW(nmea::sfi sfi);
}

TEST_F(test_nmea_sfi, properties)
{
	nmea_sentence_traits<nmea::sfi>();
}

TEST_F(test_nmea_sfi, parse)
{
	auto s = nmea::make_sentence("$GPSFI,1,1,156025,M*03");
	ASSERT_NE(nullptr, s);

	auto sfi = nmea::sentence_cast<nmea::sfi>(s);
	ASSERT_NE(nullptr, sfi);
}

TEST_F(test_nmea_sfi, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::sfi>(nmea::talker::none, {1, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::sfi>(nmea::talker::none, {3, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::sfi>(nmea::talker::none, {23, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::sfi>(nmea::talker::none, {24, "@"}));
}

TEST_F(test_nmea_sfi, empty_to_string)
{
	nmea::sfi sfi;

	EXPECT_STREQ("$GPSFI,0,0*4B", nmea::to_string(sfi).c_str());
}
}

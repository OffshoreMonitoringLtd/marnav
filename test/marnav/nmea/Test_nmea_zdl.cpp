#include <marnav/nmea/zdl.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_zdl : public ::testing::Test
{
};

TEST_F(test_nmea_zdl, contruction)
{
	EXPECT_NO_THROW(nmea::zdl zdl);
}

TEST_F(test_nmea_zdl, properties)
{
	nmea_sentence_traits<nmea::zdl>();
}

TEST_F(test_nmea_zdl, parse)
{
	auto s = nmea::make_sentence("$GPZDL,383401,12.3,R*28");
	ASSERT_NE(nullptr, s);

	auto zdl = nmea::sentence_cast<nmea::zdl>(s);
	ASSERT_NE(nullptr, zdl);
}

TEST_F(test_nmea_zdl, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::zdl>(nmea::talker::none, {2, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::zdl>(nmea::talker::none, {4, "@"}));
}

TEST_F(test_nmea_zdl, empty_to_string)
{
	nmea::zdl zdl;

	EXPECT_STREQ("$GPZDL,000000,0.0,R*15", nmea::to_string(zdl).c_str());
}
}

#include <marnav/nmea/dpt.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_dpt : public ::testing::Test
{
};

TEST_F(test_nmea_dpt, contruction)
{
	EXPECT_NO_THROW(nmea::dpt dpt);
}

TEST_F(test_nmea_dpt, properties)
{
	nmea_sentence_traits<nmea::dpt>();
}

TEST_F(test_nmea_dpt, parse_two_fields)
{
	auto s = nmea::make_sentence("$IIDPT,9.3,1.0*4B");
	ASSERT_NE(nullptr, s);

	auto dpt = nmea::sentence_cast<nmea::dpt>(s);
	ASSERT_NE(nullptr, dpt);
}

TEST_F(test_nmea_dpt, parse_three_fields)
{
	auto s = nmea::make_sentence("$IIDPT,9.3,1.0,1.0*48");
	ASSERT_NE(nullptr, s);

	auto dpt = nmea::sentence_cast<nmea::dpt>(s);
	ASSERT_NE(nullptr, dpt);
}

TEST_F(test_nmea_dpt, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::dpt>(nmea::talker::none, {1, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::dpt>(nmea::talker::none, {4, "@"}));
}

TEST_F(test_nmea_dpt, empty_to_string)
{
	nmea::dpt dpt;

	EXPECT_STREQ("$IIDPT,0,0,*6C", nmea::to_string(dpt).c_str());
}

TEST_F(test_nmea_dpt, set_depth_feet)
{
	nmea::dpt dpt;
	dpt.set_depth_meter(units::meters{12.5});

	EXPECT_STREQ("$IIDPT,12.5,0,*44", nmea::to_string(dpt).c_str());
}

TEST_F(test_nmea_dpt, set_transducer_offset)
{
	nmea::dpt dpt;
	dpt.set_transducer_offset(units::meters{12.5});

	EXPECT_STREQ("$IIDPT,0,12.5,*44", nmea::to_string(dpt).c_str());
}

TEST_F(test_nmea_dpt, set_max_depth)
{
	nmea::dpt dpt;
	dpt.set_max_depth(units::meters{2.5});

	EXPECT_STREQ("$IIDPT,0,0,2.5*45", nmea::to_string(dpt).c_str());
}
}

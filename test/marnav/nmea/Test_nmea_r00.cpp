#include <marnav/nmea/r00.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_r00 : public ::testing::Test
{
};

TEST_F(test_nmea_r00, contruction)
{
	EXPECT_NO_THROW(nmea::r00 r00);
}

TEST_F(test_nmea_r00, properties)
{
	nmea_sentence_traits<nmea::r00>();
}

TEST_F(test_nmea_r00, parse)
{
	auto s = nmea::make_sentence("$GPR00,EGLL,EGLM,EGTB,EGUB,EGTK,MBOT,EGTB,,,,,,,*58");
	ASSERT_NE(nullptr, s);

	auto r00 = nmea::sentence_cast<nmea::r00>(s);
	ASSERT_NE(nullptr, r00);
}

TEST_F(test_nmea_r00, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::r00>(nmea::talker::none, {13, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::r00>(nmea::talker::none, {15, "@"}));
}

TEST_F(test_nmea_r00, empty_to_string)
{
	nmea::r00 r00;

	EXPECT_STREQ("$GPR00,,,,,,,,,,,,,,*45", nmea::to_string(r00).c_str());
}

TEST_F(test_nmea_r00, set_id_invalid_index)
{
	nmea::r00 r00;

	EXPECT_ANY_THROW(r00.set_waypoint_id(-1, nmea::waypoint{"ABC"}));
	EXPECT_ANY_THROW(r00.set_waypoint_id(15, nmea::waypoint{"ABC"}));
}

TEST_F(test_nmea_r00, set_id)
{
	nmea::r00 r00;

	r00.set_waypoint_id(0, nmea::waypoint{"ABC"});
	EXPECT_STREQ("$GPR00,ABC,,,,,,,,,,,,,*05", nmea::to_string(r00).c_str());
}

TEST_F(test_nmea_r00, get_id)
{
	auto s = nmea::make_sentence("$GPR00,EGLL,EGLM,EGTB,EGUB,EGTK,MBOT,EGTB,,,,,,,*58");
	ASSERT_NE(nullptr, s);

	auto r00 = nmea::sentence_cast<nmea::r00>(s);
	ASSERT_NE(nullptr, r00);

	{
		auto wp = *r00->get_waypoint_id(0);
		EXPECT_STREQ("EGLL", wp.c_str());
	}
	{
		auto wp = *r00->get_waypoint_id(1);
		EXPECT_STREQ("EGLM", wp.c_str());
	}
	{
		auto wp = *r00->get_waypoint_id(2);
		EXPECT_STREQ("EGTB", wp.c_str());
	}
}

TEST_F(test_nmea_r00, found_on_internet)
{
	{
		auto s = nmea::make_sentence("$GPR00,EGLL,EGLM,EGTB,EGUB,EGTK,MBOT,EGTB,,,,,,,*58");
		EXPECT_NE(nullptr, s);
	}
	{
		auto s = nmea::make_sentence(
			"$GPR00,MINST,CHATN,CHAT1,CHATW,CHATM,CHATE,003,004,005,006,007,,,*05");
		EXPECT_NE(nullptr, s);
	}
}
}

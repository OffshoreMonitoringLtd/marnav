#include <marnav/nmea/mtw.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_mtw : public ::testing::Test
{
};

TEST_F(test_nmea_mtw, contruction)
{
	EXPECT_NO_THROW(nmea::mtw mtw);
}

TEST_F(test_nmea_mtw, properties)
{
	nmea_sentence_traits<nmea::mtw>();
}

TEST_F(test_nmea_mtw, parse)
{
	auto s = nmea::make_sentence("$IIMTW,9.5,C*2F");
	ASSERT_NE(nullptr, s);

	auto mtw = nmea::sentence_cast<nmea::mtw>(s);
	ASSERT_NE(nullptr, mtw);

	auto temperature = mtw->get_temperature().get<marnav::units::celsius>();
	EXPECT_NEAR(9.5, temperature.value(), 1e-8);
}

TEST_F(test_nmea_mtw, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::mtw>(nmea::talker::none, {1, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::mtw>(nmea::talker::none, {3, "@"}));
}

TEST_F(test_nmea_mtw, empty_to_string)
{
	nmea::mtw mtw;

	EXPECT_STREQ("$IIMTW,0,C*3D", nmea::to_string(mtw).c_str());
}

TEST_F(test_nmea_mtw, set_temperature_to_string)
{
	nmea::mtw mtw;
	mtw.set_temperature(units::celsius{22.5});

	EXPECT_STREQ("$IIMTW,22.5,C*16", nmea::to_string(mtw).c_str());
}
}

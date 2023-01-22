#include <marnav/nmea/hdt.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_hdt : public ::testing::Test
{
};

TEST_F(test_nmea_hdt, contruction)
{
	EXPECT_NO_THROW(nmea::hdt hdt);
}

TEST_F(test_nmea_hdt, properties)
{
	nmea_sentence_traits<nmea::hdt>();
}

TEST_F(test_nmea_hdt, parse)
{
	auto s = nmea::make_sentence("$IIHDT,45.8,T*1B");
	ASSERT_NE(nullptr, s);

	auto hdt = nmea::sentence_cast<nmea::hdt>(s);
	ASSERT_NE(nullptr, hdt);

	auto heading = hdt->get_heading();
	EXPECT_TRUE(heading.has_value());
	EXPECT_NEAR(45.8, heading.value(), 1e-8);
}

TEST_F(test_nmea_hdt, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::hdt>(nmea::talker::none, {1, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::hdt>(nmea::talker::none, {3, "@"}));
}

TEST_F(test_nmea_hdt, empty_to_string)
{
	nmea::hdt hdt;

	EXPECT_STREQ("$IIHDT,,*58", nmea::to_string(hdt).c_str());
}

TEST_F(test_nmea_hdt, set_heading)
{
	nmea::hdt hdt;
	hdt.set_heading(45.8);

	EXPECT_STREQ("$IIHDT,45.8,T*1B", nmea::to_string(hdt).c_str());
}
}

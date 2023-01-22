#include <marnav/nmea/tfi.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_tfi : public ::testing::Test
{
};

TEST_F(test_nmea_tfi, contruction)
{
	EXPECT_NO_THROW(nmea::tfi tfi);
}

TEST_F(test_nmea_tfi, properties)
{
	nmea_sentence_traits<nmea::tfi>();
}

TEST_F(test_nmea_tfi, parse)
{
	auto s = nmea::make_sentence("$GPTFI,0,1,2*53");
	ASSERT_NE(nullptr, s);

	auto tfi = nmea::sentence_cast<nmea::tfi>(s);
	ASSERT_NE(nullptr, tfi);
}

TEST_F(test_nmea_tfi, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::tfi>(nmea::talker::none, {2, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::tfi>(nmea::talker::none, {4, "@"}));
}

TEST_F(test_nmea_tfi, empty_to_string)
{
	nmea::tfi tfi;

	EXPECT_STREQ("$GPTFI,2,2,2*52", nmea::to_string(tfi).c_str());
}
}

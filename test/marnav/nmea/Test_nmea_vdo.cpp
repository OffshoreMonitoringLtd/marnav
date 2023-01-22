#include <marnav/nmea/vdo.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_vdo : public ::testing::Test
{
};

TEST_F(test_nmea_vdo, contruction)
{
	EXPECT_NO_THROW(nmea::vdo vdo);
}

TEST_F(test_nmea_vdo, properties)
{
	nmea_sentence_traits<nmea::vdo>();
}

TEST_F(test_nmea_vdo, parse)
{
	auto s = nmea::make_sentence("!AIVDO,1,1,,B,177KQJ5000G?tO`K>RA1wUbN0TKH,0*5E");
	ASSERT_NE(nullptr, s);

	auto vdo = nmea::sentence_cast<nmea::vdo>(s);
	ASSERT_NE(nullptr, vdo);

	auto fragments = vdo->get_n_fragments();
	EXPECT_EQ(1u, fragments);

	auto fragment = vdo->get_fragment();
	EXPECT_EQ(1u, fragment);
}

TEST_F(test_nmea_vdo, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vdo>(nmea::talker::none, {5, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::vdo>(nmea::talker::none, {7, "@"}));
}
}

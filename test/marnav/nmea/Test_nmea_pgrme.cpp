#include <marnav/nmea/pgrme.hpp>
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_pgrme : public ::testing::Test
{
};

TEST_F(test_nmea_pgrme, contruction)
{
	nmea::pgrme pgrme;
}

TEST_F(test_nmea_pgrme, properties)
{
	nmea_sentence_traits<nmea::pgrme>();
}

TEST_F(test_nmea_pgrme, parse)
{
	auto s = nmea::make_sentence("$PGRME,1.1,M,2.2,M,3.3,M*2E");
	ASSERT_NE(nullptr, s);

	auto pgrme = nmea::sentence_cast<nmea::pgrme>(s);
	ASSERT_NE(nullptr, pgrme);
}

TEST_F(test_nmea_pgrme, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::pgrme>(nmea::talker::none, {5, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::pgrme>(nmea::talker::none, {7, "@"}));
}

TEST_F(test_nmea_pgrme, empty_to_string)
{
	nmea::pgrme pgrme;

	EXPECT_STREQ("$PGRME,,M,,M,,M*00", nmea::to_string(pgrme).c_str());
}

TEST_F(test_nmea_pgrme, set_horizontal_position_error)
{
	nmea::pgrme pgrme;
	pgrme.set_horizontal_position_error(units::meters{1.1});

	EXPECT_STREQ("$PGRME,1.1,M,,M,,M*2E", nmea::to_string(pgrme).c_str());
}

TEST_F(test_nmea_pgrme, set_vertical_position_error)
{
	nmea::pgrme pgrme;
	pgrme.set_vertical_position_error(units::meters{2.2});

	EXPECT_STREQ("$PGRME,,M,2.2,M,,M*2E", nmea::to_string(pgrme).c_str());
}

TEST_F(test_nmea_pgrme, set_overall_spherical_equiv_position_error)
{
	nmea::pgrme pgrme;
	pgrme.set_overall_spherical_equiv_position_error(units::meters{3.3});

	EXPECT_STREQ("$PGRME,,M,,M,3.3,M*2E", nmea::to_string(pgrme).c_str());
}
}

#include <marnav/nmea/alr.hpp>
#include "marnav/nmea/checksum.hpp"
#include "marnav/nmea/checksum_enum.hpp"
#include "type_traits_helper.hpp"
#include <marnav/nmea/nmea.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_alr : public ::testing::Test
{
};

TEST_F(test_nmea_alr, contruction)
{
	EXPECT_NO_THROW(nmea::alr alr);
}

TEST_F(test_nmea_alr, properties)
{
	nmea_sentence_traits<nmea::alr>();
}

TEST_F(test_nmea_alr, parse)
{
	auto s = nmea::make_sentence("$INALR,000000,260,A,V,Emergency Call*1C");
	ASSERT_NE(nullptr, s);

	auto alr = nmea::sentence_cast<nmea::alr>(s);
	ASSERT_NE(nullptr, alr);
}

TEST_F(test_nmea_alr, parse_invalid_number_of_arguments)
{
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::alr>(nmea::talker::none, {4, "@"}));
	EXPECT_ANY_THROW(
		nmea::detail::factory::sentence_parse<nmea::alr>(nmea::talker::none, {6, "@"}));
}

TEST_F(test_nmea_alr, empty_to_string)
{
	nmea::alr alr;

	EXPECT_STREQ("$IIALR,000000,0,A,A,*43", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_time_utc)
{
	nmea::alr alr;
	alr.set_time_utc(nmea::time{12, 34, 56});

	EXPECT_STREQ("$IIALR,123456,0,A,A,*44", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_number)
{
	nmea::alr alr;
	alr.set_number(123);

	EXPECT_STREQ("$IIALR,000000,123,A,A,*43", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_condition_exceeded)
{
	nmea::alr alr;
	alr.set_condition(nmea::alr::condition::threshold_exceeded);

	EXPECT_STREQ("$IIALR,000000,0,A,A,*43", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_condition_not_exceeded)
{
	nmea::alr alr;
	alr.set_condition(nmea::alr::condition::threshold_not_exceeded);

	EXPECT_STREQ("$IIALR,000000,0,V,A,*54", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_acknowledge_acknowledged)
{
	nmea::alr alr;
	alr.set_acknowledge(nmea::alr::acknowledge::acknowledged);

	EXPECT_STREQ("$IIALR,000000,0,A,A,*43", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_acknowledge_not_acknowledged)
{
	nmea::alr alr;
	alr.set_acknowledge(nmea::alr::acknowledge::not_acknowledged);

	EXPECT_STREQ("$IIALR,000000,0,A,V,*54", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_text)
{
	nmea::alr alr;
	alr.set_text("alarm text");

	EXPECT_STREQ("$IIALR,000000,0,A,A,alarm text*0D", nmea::to_string(alr).c_str());
}

TEST_F(test_nmea_alr, set_text_too_long)
{
	nmea::alr alr;
	EXPECT_ANY_THROW(alr.set_text(
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas mattis libero"));
}

TEST_F(test_nmea_alr, get_text)
{
	auto s = nmea::make_sentence("$INALR,000000,260,A,V,Emergency Call*1C");
	ASSERT_NE(nullptr, s);

	auto alr = nmea::sentence_cast<nmea::alr>(s);
	ASSERT_NE(nullptr, alr);

	EXPECT_STREQ("Emergency Call", alr->get_text().c_str());
}

TEST_F(test_nmea_alr, get_acknowledge)
{
	auto s = nmea::make_sentence("$INALR,000000,260,A,V,Emergency Call*1C");
	ASSERT_NE(nullptr, s);

	auto alr = nmea::sentence_cast<nmea::alr>(s);
	ASSERT_NE(nullptr, alr);

	EXPECT_EQ(nmea::alr::acknowledge::not_acknowledged, alr->get_acknowledge());
}

TEST_F(test_nmea_alr, get_condition)
{
	auto s = nmea::make_sentence("$INALR,000000,260,A,V,Emergency Call*1C");
	ASSERT_NE(nullptr, s);

	auto alr = nmea::sentence_cast<nmea::alr>(s);
	ASSERT_NE(nullptr, alr);

	EXPECT_EQ(nmea::alr::condition::threshold_exceeded, alr->get_condition());
}

TEST_F(test_nmea_alr, get_number)
{
	auto s = nmea::make_sentence("$INALR,000000,260,A,V,Emergency Call*1C");
	ASSERT_NE(nullptr, s);

	auto alr = nmea::sentence_cast<nmea::alr>(s);
	ASSERT_NE(nullptr, alr);

	EXPECT_EQ(260u, alr->get_number());
}

TEST_F(test_nmea_alr, various_alarm_descriptions)
{
	// descriptions found: https://www.cruisersforum.com/forums/f134/opencpn-and-external-nmea0183-alarms-81396.html
	static const std::string preamble = "$IIALR,000000,0,A,A,";
	// clang-format off
	static const std::vector<std::pair<std::string, std::string>> descriptions = {
		{ "AIS: TX malfunction",                     "4C" },
		{ "AIS: Antenna VSWR exceeds limit",         "61" },
		{ "AIS: Rx channel 1 malfunction",           "38" },
		{ "AIS: Rx channel 2 malfunction",           "3B" },
		{ "AIS: general failure",                    "34" },
		{ "AIS: no sensor position in use",          "68" },
		{ "AIS: no valid SOG information",           "62" },
		{ "AIS: no valid COG information",           "72" },
		{ "AIS: 12V alarm",                          "04" },
		{ "AIS: 5V alarm",                           "32" },
		{ "AIS: Loss of serial interface integrity", "2A" },
		{ "AIS: Background noise above -77dBm",      "6B" },
	};
	// clang-format on

	for (const auto & [d, c] : descriptions) {
		try {
			auto s = nmea::make_sentence(preamble + d + '*' + c);
			ASSERT_NE(nullptr, s) << "desc: " << d;

			auto alr = nmea::sentence_cast<nmea::alr>(s);
			ASSERT_NE(nullptr, alr);

			EXPECT_STREQ(d.c_str(), alr->get_text().c_str());
		} catch (nmea::checksum_error & e) {
			ASSERT_FALSE(true) << e.what() << " for desc: " << d;
		}
	}
}
}

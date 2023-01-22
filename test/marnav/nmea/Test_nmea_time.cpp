#include <marnav/nmea/time.hpp>
#include <gtest/gtest.h>

namespace
{
using namespace marnav;

class test_nmea_time : public ::testing::Test
{
};

TEST_F(test_nmea_time, explicit_construction)
{
	EXPECT_ANY_THROW((nmea::time{24, 0, 0, 0}));
	EXPECT_ANY_THROW((nmea::time{0, 60, 0, 0}));
	EXPECT_ANY_THROW((nmea::time{0, 0, 60, 0}));
	EXPECT_ANY_THROW((nmea::time{0, 0, 0, 1000}));
}

TEST_F(test_nmea_time, copy_construction)
{
	const auto t = nmea::time{12, 34, 56};

	const auto v = nmea::time(t);

	EXPECT_EQ(t, v);
}

TEST_F(test_nmea_time, move_construction)
{
	const auto v = nmea::time(std::move(nmea::time{12, 34, 56}));

	EXPECT_EQ(nmea::time(12, 34, 56), v);
}

TEST_F(test_nmea_time, default_construction)
{
	EXPECT_EQ(nmea::time(0, 0, 0, 0), nmea::time{});
}

TEST_F(test_nmea_time, hour)
{
	nmea::time t{1, 2, 3, 4};

	EXPECT_EQ(1u, t.hour());
}

TEST_F(test_nmea_time, minutes)
{
	nmea::time t{1, 2, 3, 4};

	EXPECT_EQ(2u, t.minutes());
}

TEST_F(test_nmea_time, seconds)
{
	nmea::time t{1, 2, 3, 4};

	EXPECT_EQ(3u, t.seconds());
}

TEST_F(test_nmea_time, milliseconds)
{
	nmea::time t{1, 2, 3, 4};

	EXPECT_EQ(4u, t.milliseconds());
}

TEST_F(test_nmea_time, comparison_equal)
{
	nmea::time t0{1, 2, 3, 4};
	nmea::time t1{1, 2, 3, 4};

	EXPECT_TRUE(t0 == t1);
}

TEST_F(test_nmea_time, invalid_format_for_double)
{
	EXPECT_ANY_THROW(nmea::time::parse("123.455.6"));
}

TEST_F(test_nmea_time, to_string_no_ms)
{
	nmea::time t{1, 2, 3, 0};

	using namespace marnav::nmea;
	const std::string s = to_string(t);

	EXPECT_STREQ("010203", s.c_str());
}

TEST_F(test_nmea_time, to_string_with_ms)
{
	nmea::time t{1, 2, 3, 4};

	using namespace marnav::nmea;
	const std::string s = to_string(t);

	EXPECT_STREQ("010203.004", s.c_str());
}
}

#include <marnav/nmea/its.hpp>
#include <marnav/nmea/io.hpp>
#include "checks.hpp"

namespace marnav::nmea
{
constexpr sentence_id its::ID;
constexpr const char * its::TAG;

its::its()
	: sentence(ID, TAG, talker::global_positioning_system)
{
}

its::its(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 2)
		throw std::invalid_argument{"invalid number of fields in its"};

	unit::distance distance_unit;

	read(*(first + 0), distance_);
	read(*(first + 1), distance_unit);

	check_value(distance_unit, {unit::distance::meter}, "distance unit");
}

void its::append_data_to(std::string & s, const version &) const
{
	append(s, to_string(distance_));
	append(s, to_string(unit::distance::meter));
}
}

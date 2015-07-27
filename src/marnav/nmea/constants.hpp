#ifndef __NMEA__CONSTANTS__HPP__
#define __NMEA__CONSTANTS__HPP__

#include <stdexcept>
#include <string>

namespace marnav
{

// forward declaration
namespace geo
{
class latitude;
class longitude;
}

namespace nmea
{
char convert_hemisphere(const geo::latitude & p);
char convert_hemisphere(const geo::longitude & p);

namespace direction
{
constexpr const char EAST = 'E';
constexpr const char WEST = 'W';
constexpr const char NORTH = 'N';
constexpr const char SOUTH = 'S';
}

namespace status
{
constexpr const char OK = 'A';
constexpr const char WARNING = 'V';
}

/// Positioning System Mode Indicator constants.
namespace positioning_system_mode_indicator
{
constexpr const char INVALID = 'V';
constexpr const char AUTONOMOUS = 'A';
constexpr const char DIFFERENTIAL = 'D';
constexpr const char ESTIMATED = 'E';
constexpr const char MANUAL_INPUT = 'M';
constexpr const char SIMULATED = 'S';
constexpr const char DATA_NOT_VALID = 'N';
constexpr const char PRECISE = 'P';
}

namespace quality
{
constexpr const int INVALID = 0;
constexpr const int GPS_FIX = 1;
constexpr const int DPGS_FIX = 2;
constexpr const int GUESS = 6;
}

namespace unit
{
constexpr const char METER = 'M';
constexpr const char FEET = 'f';
constexpr const char NM = 'N'; // nautical miles
constexpr const char KM = 'K'; // kilometers
constexpr const char FATHOM = 'F';
constexpr const char KNOT = 'N';
constexpr const char KMH = 'K'; // kilometers per hour
constexpr const char MPS = 'M'; // meters per second
constexpr const char CELSIUS = 'C';
constexpr const char BAR = 'B'; // pressure in Bar
constexpr const char PASCAL = 'P'; // pressure in Pascal
}

/// Constants for possible references of data.
namespace reference
{
constexpr const char TRUE = 'T';
constexpr const char MAGNETIC = 'M';
constexpr const char RELATIVE = 'R';
}

enum class side : char { LEFT, RIGHT };

enum class route : char { COMPLETE, WORKING };

enum class selection_mode : char { MANUAL, AUTOMATIC };

enum class ais_channel : char { A, B };

namespace type_of_point
{
constexpr const char collision = 'C';
constexpr const char turning_point = 'T';
constexpr const char reference = 'R';
constexpr const char wheelover = 'W';
}
}
}

#endif

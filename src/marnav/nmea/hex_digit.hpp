#ifndef MARNAV_NMEA_HEX_DIGIT_HPP
#define MARNAV_NMEA_HEX_DIGIT_HPP

namespace marnav::nmea::detail
{
inline constexpr char hex_digit(unsigned int t) noexcept
{
	return "0123456789ABCDEF"[t & 0xf];
}
}

#endif

#ifndef MARNAV_AIS_MESSAGE_23_HPP
#define MARNAV_AIS_MESSAGE_23_HPP

#include <marnav/ais/message.hpp>
#include <marnav/geo/position.hpp>
#include <marnav/utils/mmsi.hpp>
#include <optional>

namespace marnav::ais
{
/// @brief Group Assignment Command
///
class message_23 : public message
{
	friend class detail::factory;

public:
	constexpr static message_id ID = message_id::group_assignment_command;
	constexpr static std::size_t SIZE_BITS = 160u;

	enum class report_interval : uint8_t {
		autonomous_mode = 0,
		minutes_10 = 1,
		minutes_06 = 2,
		minutes_03 = 3,
		minutes_01 = 4,
		seconds_30 = 5,
		seconds_15 = 6,
		seconds_10 = 7,
		seconds_05 = 8,
		next_shorter = 9,
		next_longer = 10,
		// reserved for future use: 11 - 15
	};

	message_23();
	message_23(const message_23 &) = default;
	message_23 & operator=(const message_23 &) = default;
	message_23(message_23 &&) = default;
	message_23 & operator=(message_23 &&) = default;

protected:
	message_23(const raw & bits);
	void read_data(const raw & bits);
	raw get_data() const override;

private:
	// clang-format off
	bitset_value<  6,  2, uint32_t       > repeat_indicator_ = 0;
	bitset_value<  8, 30, uint32_t       > mmsi_ = 0;
	bitset_value< 40, 18, uint32_t       > ne_lon_ = longitude_not_available_short;
	bitset_value< 58, 17, uint32_t       > ne_lat_ = latitude_not_available_short;
	bitset_value< 75, 18, uint32_t       > sw_lon_ = longitude_not_available_short;
	bitset_value< 93, 17, uint32_t       > sw_lat_ = latitude_not_available_short;
	bitset_value<110,  4, uint32_t       > station_type_ = 0;
	bitset_value<114,  8, ship_type      > shiptype_ = ship_type::not_available;
	bitset_value<144,  2, uint32_t       > txrx_mode_ = 0;
	bitset_value<146,  4, report_interval> interval_ = report_interval::autonomous_mode;
	bitset_value<150,  4, uint32_t       > quiet_time_ = 0; // minutes (0=none, 1..15 minutes)
	// clang-format on

public:
	uint32_t get_repeat_indicator() const noexcept { return repeat_indicator_; }
	utils::mmsi get_mmsi() const noexcept { return utils::mmsi{mmsi_}; }
	uint32_t get_station_type() const noexcept { return station_type_; }
	ship_type get_shiptype() const noexcept { return shiptype_; }
	uint32_t get_txrx_mode() const noexcept { return txrx_mode_; }
	report_interval get_interval() const noexcept { return interval_; }
	uint32_t get_quiet_time() const noexcept { return quiet_time_; }

	std::optional<geo::position> get_position_ne() const;
	std::optional<geo::position> get_position_sw() const;

	void set_repeat_indicator(uint32_t t) noexcept { repeat_indicator_ = t; }
	void set_mmsi(const utils::mmsi & t) noexcept { mmsi_ = t; }
	void set_station_type(uint32_t t) noexcept { station_type_ = t; }
	void set_txrx_mode(uint32_t t) noexcept { txrx_mode_ = t; }
	void set_interval(report_interval t) { interval_ = t; }
	void set_quiet_time(uint32_t t) noexcept { quiet_time_ = t; }

	void set_position_ne(const geo::position & t) noexcept;
	void set_position_sw(const geo::position & t) noexcept;
};
}

#endif

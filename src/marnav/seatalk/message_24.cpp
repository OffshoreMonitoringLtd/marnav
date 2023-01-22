#include <marnav/seatalk/message_24.hpp>

namespace marnav::seatalk
{

message_24::message_24()
	: message(ID)
{
}

std::unique_ptr<message> message_24::parse(const raw & data)
{
	check_size(data, SIZE);

	std::unique_ptr<message> result = std::make_unique<message_24>();
	auto & msg = static_cast<message_24 &>(*result);

	msg.unit_ = static_cast<unit_type>(data[4]);

	return result;
}

raw message_24::get_data() const
{
	return raw{static_cast<uint8_t>(ID), 0x02, 0x00, 0x00, unit_};
}
}

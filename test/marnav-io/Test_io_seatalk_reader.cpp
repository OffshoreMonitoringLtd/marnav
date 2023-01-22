#include <marnav-io/seatalk_reader.hpp>
#include <marnav-io/device.hpp>
#include <gtest/gtest.h>

namespace
{

using namespace marnav;

// The data presented here is not the data you would discover directly
// on the SeaTalk-1 line. It is necessary to detect parity issues because
// they are misused as command indicator flags by SeaTalk. To detect
// them, it is necessary to configure the serial port (using termios)
// to PARMRK, which will send two bytes as indicators for parity bit issues.
// The two bytes can have the following values: 0xff 0x00 or 0xff 0xff.
// In case of a parity error, 0xff 0x00 is sent as mark. In case of
// a non-error situation where 0xff is received, it will be escaped as 0xff 0xff.
//
// Read more about it here: http://www.gnu.org/software/libc/manual/html_node/Input-Modes.html
//
static const uint8_t DATA[] = {
	// preliminary garbage
	0x01, // bit=0 parity=0 : no error : ?
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : ?
	0x01, // bit=0 parity=0 : no error : ?
	0x01, // bit=0 parity=0 : no error : ?
	0xff, 0xff, // bit=0 parity=1 : error    : ?
	0xff, 0xff, // bit=0 parity=1 : error    : ?
	0x01, // bit=0 parity=0 : no error : ?

	// depth
	0x00, // bit=1 parity=1 : no error : cmd
	0x02, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x60, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x65, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data

	// speed through water */
	0xff, 0x00, 0x26, // bit=1 parity=0 : error    : cmd
	0x04, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data

	// water temperature
	0x27, // bit=1 parity=1 : no error : cmd
	0x01, // bit=0 parity=0 : no error : data
	0x64, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data

	// apparent wind speed
	0x11, // bit=1 parity=1 : no error : cmd
	0x01, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x06, // bit=0 parity=1 : error    : data
	0x01, // bit=0 parity=0 : no error : data

	// speed through water
	0xff, 0x00, 0x20, // bit=1 parity=0 : error    : cmd
	0x01, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data

	// water temperature
	0xff, 0x00, 0x23, // bit=1 parity=0 : error    : cmd
	0x01, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x33, // bit=0 parity=1 : error    : data
	0x5b, // bit=0 parity=0 : no error : data

	// apparent wind angle
	0xff, 0x00, 0x10, // bit=1 parity=0 : error    : cmd
	0x01, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x14, // bit=0 parity=1 : error    : data

	// depth
	0x00, // bit=1 parity=1 : no error : cmd
	0x02, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x60, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x65, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data

	// depth, collision
	0x00, // bit=1 parity=1 : no error : cmd
	0x02, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x60, // bit=0 parity=1 : error    : data
	// collision here, two bytes lost

	// apparent wind angle
	0xff, 0x00, 0x10, // bit=1 parity=0 : error    : cmd
	0x01, // bit=0 parity=0 : no error : data
	0xff, 0x00, 0x00, // bit=0 parity=1 : error    : data
	0xff, 0x00, 0x14, // bit=0 parity=1 : error    : data
};

class dummy_device : public ::io::device
{
public:
	dummy_device() = default;

	void open() override {}
	void close() override {}

	/// Just go through the data once.
	int read(char * buffer, uint32_t size) override
	{
		if (size != sizeof(*buffer))
			throw std::invalid_argument{"buffer type not supported"};
		if (index_ >= sizeof(DATA))
			return 0; // end of data
		*buffer = static_cast<char>(DATA[index_]);
		++index_;
		return 1;
	}

	int write(const char *, uint32_t) override
	{
		throw std::runtime_error{"operation not supported"};
	}

private:
	uint32_t index_{0};
};

class dummy_reader : public ::io::seatalk_reader
{
public:
	dummy_reader()
		: seatalk_reader(std::make_unique<dummy_device>())
	{
	}

	int get_num_messages() const { return num_messages_; }

protected:
	void process_message(const seatalk::raw &) override { ++num_messages_; }

private:
	int num_messages_{0};
};

/// Works only in a single threaded context (true for dummuy_device and seatalk_reader).
class message_reader : public ::io::seatalk_reader
{
public:
	message_reader()
		: seatalk_reader(std::make_unique<dummy_device>())
	{
	}

	bool read_message(seatalk::raw & data)
	{
		while (read()) {
			if (message_received_) {
				data = message_;
				message_received_ = false;
				return true;
			}
		}
		return false;
	}

protected:
	void process_message(const seatalk::raw & msg) override
	{
		message_ = msg;
		message_received_ = true;
	}

private:
	bool message_received_{false};
	seatalk::raw message_;
};

class test_io_seatalk_reader : public ::testing::Test
{
};

TEST_F(test_io_seatalk_reader, read_count_messages_and_collisions)
{
	dummy_reader device;

	while (device.read())
		;

	EXPECT_EQ(9, device.get_num_messages());
	EXPECT_EQ(1u, device.get_collisions());
}

TEST_F(test_io_seatalk_reader, read_message)
{
	message_reader dev;
	seatalk::raw msg;

	int num_messages = 0;
	seatalk::raw data;

	while (dev.read_message(data))
		++num_messages;

	EXPECT_EQ(9, num_messages);
}

TEST_F(test_io_seatalk_reader, read_first_message_depth)
{
	message_reader dev;
	seatalk::raw msg;

	ASSERT_NO_THROW(dev.read_message(msg));
	EXPECT_EQ(5u, msg.size());
	EXPECT_EQ(0x00u, msg[0]);
	EXPECT_EQ(0x02u, msg[1]);
	EXPECT_EQ(0x60u, msg[2]);
	EXPECT_EQ(0x65u, msg[3]);
	EXPECT_EQ(0x00u, msg[4]);
}

TEST_F(test_io_seatalk_reader, read_third_message_water_temperature)
{
	message_reader dev;
	seatalk::raw msg;

	ASSERT_NO_THROW(dev.read_message(msg));
	ASSERT_NO_THROW(dev.read_message(msg));
	ASSERT_NO_THROW(dev.read_message(msg));
	EXPECT_EQ(4u, msg.size());
	EXPECT_EQ(0x27u, msg[0]);
	EXPECT_EQ(0x01u, msg[1]);
	EXPECT_EQ(0x64u, msg[2]);
	EXPECT_EQ(0x00u, msg[3]);
}
}

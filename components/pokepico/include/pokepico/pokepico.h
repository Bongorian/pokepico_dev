#pragma once

#include <MIDIBLE/MIDIBLE.h>
#include <StreamLogger/Logger.h>
#include <SAA1099.h>
#include <string>

#define GPIO_OUTPUT_LATCH 32
#define GPIO_OUTPUT_CLOCK 33
#define GPIO_OUTPUT_DATA  12
#define GPIO_OUTPUT_WE 13
#define GPIO_OUTPUT_CS  25
#define GPIO_OUTPUT_AZ  26

namespace pokepico
{
using namespace m2d;
using namespace m2d::ESP32;

class Device
{
private:
	std::string name;
	uint8_t device_udid;
	MIDIBLE::BLEInterface *interface;
	ESP32::SAA1099 *saa;

public:
	Device(std::string name, uint16_t udid)
	{
		this->interface = new MIDIBLE::BLEInterface(name, udid);

		static Logger::Group logger("pokepico");
		this->saa = new ESP32::SAA1099(GPIO_NUM_19, GPIO_NUM_18,GPIO_NUM_23,GPIO_NUM_16,GPIO_NUM_17,GPIO_NUM_32);
		saa->reset();
		saa->soundEnable();
		this->interface->note_on_handler = [&](MIDIBLE::MIDI::Channel channel, uint8_t note, uint8_t velocity) {
			saa->setNote(PSG::Channel::c0,note);
			saa->setNote(PSG::Channel::c1,note+12);
			saa->setNote(PSG::Channel::c2,note-12);
			saa->setVolume(PSG::Channel::c0,0xff);
			saa->setVolume(PSG::Channel::c1,0xff);
			saa->setVolume(PSG::Channel::c2,0xff);
		};

		this->interface->note_off_handler = [&](MIDIBLE::MIDI::Channel channel, uint8_t note, uint8_t velocity) {
			saa->setVolume(PSG::Channel::c0,0x0);
			saa->setVolume(PSG::Channel::c1,0x0);
			saa->setVolume(PSG::Channel::c2,0x0);
		};
	}

	void begin()
	{
		this->interface->begin();
	}
};
}
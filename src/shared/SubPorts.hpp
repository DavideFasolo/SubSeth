/**************************************************************
*
* Portions of this file are derived from SubmarineFree
* Copyright © 2018 David O'Rourke
* Source: https://github.com/david-c14/SubmarineFree
*
* Adapted for SubSeth.
*
**************************************************************/
#pragma once

#include "../plugin.hpp"

struct SilverPort : PortWidget {
	NVGcolor col = nvgRGB(0xf0, 0xf0, 0xf0);
	SilverPort() {
		box.size = Vec(25.f, 25.f);
	}
	void draw(const DrawArgs& args) override;
};

struct RedPort : SilverPort {
	RedPort() {
		col = nvgRGB(0xed, 0x2c, 0x24);
	}
};

struct BluePort : SilverPort {
	BluePort() {
		col = nvgRGB(0x29, 0xb2, 0xef);
	}
};

struct BlackPort : SilverPort {
	BlackPort() {
		col = nvgRGB(0x10, 0x10, 0x10);
	}
};

struct PurplePort : SilverPort {
	PurplePort() {
		col = nvgRGB(0xD0, 0x00, 0xF0);
	}
};

struct PinkPort : SilverPort {
	PinkPort() {
		col = nvgRGB(0xe0, 0x80, 0xb0);
	}
};

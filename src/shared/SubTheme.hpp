#pragma once

#include "../plugin.hpp"

namespace SubTheme {

static inline NVGcolor panelBg() {
	return nvgRGB(0x29, 0x4F, 0x77);
}

static inline NVGcolor panelDark() {
	return nvgRGB(0x16, 0x2D, 0x50);
}

static inline NVGcolor modTitle() {
	return nvgRGB(0x6D, 0xA1, 0xD3);
}

static inline NVGcolor objLabel() {
	return nvgRGB(0xFF, 0xFF, 0xFF);
}

static inline NVGcolor portHalo() {
	return nvgRGBA(0x16, 0x2D, 0x50, 56);
}

} // namespace SubTheme

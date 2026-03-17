#pragma once

#include "../plugin.hpp"

namespace SubFonts {

static inline std::shared_ptr<Font> foModTitle() {
	return APP->window->loadFont(
		asset::plugin(pluginInstance, "res/fonts/Barlow/Barlow-Medium.ttf")
	);
}

static inline std::shared_ptr<Font> foPort() {
	return APP->window->loadFont(
		asset::plugin(pluginInstance, "res/fonts/Barlow/Barlow-Regular.ttf")
	);
}

} // namespace SubFonts

#pragma once

#include "../plugin.hpp"
#include "SubTheme.hpp"
#include "SubLogoPath.hpp"

namespace SubLogo {

static constexpr float TARGET_W = 14.4f;
static constexpr float TARGET_H = 12.0f;
static constexpr float BOTTOM_MARGIN = 2.0f;

static inline void drawLogo(const Widget::DrawArgs& args, const Vec& panelSize) {
	const float x = std::round((panelSize.x - TARGET_W) * 0.5f);
	const float y = panelSize.y - TARGET_H - BOTTOM_MARGIN;

	nvgSave(args.vg);
	nvgTranslate(args.vg, x, y);
	nvgScale(args.vg, TARGET_W / SubLogoPath::WIDTH, TARGET_H / SubLogoPath::HEIGHT);
	nvgTranslate(args.vg, -SubLogoPath::MIN_X, -SubLogoPath::MIN_Y);

	nvgBeginPath(args.vg);
	SubLogoPath::build(args.vg);
	nvgFillColor(args.vg, SubTheme::modTitle());
	nvgFill(args.vg);
	nvgRestore(args.vg);
}

} // namespace SubLogo

#pragma once

#include "../plugin.hpp"
#include "SubFonts.hpp"
#include "SubTheme.hpp"
#include <cmath>
#include <vector>

namespace SubDraw {

static constexpr float MOD_TITLE_Y = 12.f;
static constexpr float MOD_TITLE_FONT_SIZE = 15.f;
static constexpr float OBJ_LABEL_FONT_SIZE = 11.f;

enum class Justify {
	Left,
	Center,
	Right
};

static inline int toNvgHorizontalAlign(Justify justify) {
	switch (justify) {
		case Justify::Left:
			return NVG_ALIGN_LEFT;
		case Justify::Right:
			return NVG_ALIGN_RIGHT;
		case Justify::Center:
		default:
			return NVG_ALIGN_CENTER;
	}
}

static inline void drawModTitle(
	const Widget::DrawArgs& args,
	const Widget& w,
	const char* text
) {
	auto font = SubFonts::foModTitle();
	if (!font) {
		return;
	}

	const float x = w.box.size.x * 0.5f;
	const float y = MOD_TITLE_Y;

	nvgFontFaceId(args.vg, font->handle);
	nvgFontSize(args.vg, MOD_TITLE_FONT_SIZE);
	nvgTextLetterSpacing(args.vg, 0.f);
	nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_BASELINE);
	nvgFillColor(args.vg, SubTheme::modTitle());
	nvgText(args.vg, x, y, text, nullptr);
}

static inline void drawObjLabel(
	const Widget::DrawArgs& args,
	const Widget& obj,
	float angDeg,
	float dist,
	const char* labelText,
	Justify justify = Justify::Center
) {
	auto font = SubFonts::foPort();
	if (!font) {
		return;
	}

	const float rad = angDeg * (float) M_PI / 180.f;

	const float cx = obj.box.pos.x + obj.box.size.x * 0.5f;
	const float cy = obj.box.pos.y + obj.box.size.y * 0.5f;

	const float x = cx + std::cos(rad) * dist;
	const float y = cy - std::sin(rad) * dist;

	nvgFontFaceId(args.vg, font->handle);
	nvgFontSize(args.vg, OBJ_LABEL_FONT_SIZE);
	nvgTextLetterSpacing(args.vg, 0.f);
	nvgTextAlign(args.vg, toNvgHorizontalAlign(justify) | NVG_ALIGN_MIDDLE);
	nvgFillColor(args.vg, SubTheme::objLabel());
	nvgText(args.vg, x, y, labelText, nullptr);
}


static inline void drawPortHalos(
	const Widget::DrawArgs& args,
	const std::vector<Vec>& centers,
	float radius = 10.f
) {
	nvgFillColor(args.vg, SubTheme::portHalo());

	for (const Vec& p : centers) {
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, p.x, p.y, radius);
		nvgFill(args.vg);
	}
}


static inline void drawPortHalosForSiblingPorts(
	const Widget::DrawArgs& args,
	const Widget& w,
	float radius = 10.f
) {
	if (!w.parent) {
		return;
	}

	std::vector<Vec> centers;
	for (Widget* child : w.parent->children) {
		if (!child || child == &w) {
			continue;
		}

		if (dynamic_cast<PortWidget*>(child)) {
			centers.push_back(Vec(
				child->box.pos.x + child->box.size.x * 0.5f,
				child->box.pos.y + child->box.size.y * 0.5f
			));
		}
	}

	if (!centers.empty()) {
		drawPortHalos(args, centers, radius);
	}
}

} // namespace SubDraw

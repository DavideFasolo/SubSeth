#pragma once

#include "../plugin.hpp"
#include "SubDraw.hpp"
#include "SubTheme.hpp"
#include <cstdlib>
#include <string>
#include <vector>

namespace SubPanel {

struct BasePanel : Widget {
	int hp = 0;

	BasePanel(int hp_) : hp(hp_) {
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
	}

	void draw(const Widget::DrawArgs& args) override {
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 0.f, 0.f, box.size.x, box.size.y);
		nvgFillColor(args.vg, SubTheme::panelBg());
		nvgFill(args.vg);
	}
};

struct TitledPanel : BasePanel {
	std::string title;

	TitledPanel(int hp_, const char* title_)
		: BasePanel(hp_), title(title_ ? title_ : "") {
	}

	void draw(const Widget::DrawArgs& args) override {
		BasePanel::draw(args);
		SubDraw::drawModTitle(args, *this, title.c_str());
	}
};

struct LabelPlacement {
	float angDeg = 270.f;
	float dist = 20.f;
	SubDraw::Justify justify = SubDraw::Justify::Center;
};

struct ObjLabelSpec {
	Widget* obj = nullptr;
	std::string text;
	LabelPlacement placement;
};

struct LabeledPanel : TitledPanel {
	std::vector<ObjLabelSpec> objLabels;

	LabeledPanel(int hp_, const char* title_)
		: TitledPanel(hp_, title_) {
	}

	void addObjLabel(
		Widget* obj,
		const char* text,
		LabelPlacement placement = {}
	) {
		if (!obj || !text || !text[0]) {
			return;
		}

		ObjLabelSpec spec;
		spec.obj = obj;
		spec.text = text;
		spec.placement = placement;
		objLabels.push_back(spec);
	}

	void draw(const Widget::DrawArgs& args) override {
		TitledPanel::draw(args);
		SubDraw::drawPortHalosForSiblingPorts(args, *this);

		for (const auto& lab : objLabels) {
			if (!lab.obj || lab.text.empty()) {
				continue;
			}

			SubDraw::drawObjLabel(
				args,
				*lab.obj,
				lab.placement.angDeg,
				lab.placement.dist,
				lab.text.c_str(),
				lab.placement.justify
			);
		}
	}
};

static inline BasePanel* generatePanel(int hp) {
	return new BasePanel(hp);
}

static inline BasePanel* generatePanel(const char* hpText) {
	if (!hpText) {
		return new BasePanel(0);
	}
	return new BasePanel(std::atoi(hpText));
}

static inline TitledPanel* createPanel(int hp, const char* title) {
	return new TitledPanel(hp, title);
}

static inline LabeledPanel* createLabeledPanel(int hp, const char* title) {
	return new LabeledPanel(hp, title);
}

} // namespace SubPanel

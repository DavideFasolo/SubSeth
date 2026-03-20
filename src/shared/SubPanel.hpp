#pragma once

#include "../plugin.hpp"
#include "SubDraw.hpp"
#include "SubTheme.hpp"
#include "SubLogo.hpp"

#include <cstdlib>
#include <string>
#include <vector>

namespace SubPanel {

struct PanelOptions {
	bool showTitle = false;
	bool showSmallLogo = true;

	static PanelOptions plain() {
		return {};
	}

	static PanelOptions titled() {
		PanelOptions o;
		o.showTitle = true;
		return o;
	}

	PanelOptions withTitle(bool enabled = true) const {
		PanelOptions o = *this;
		o.showTitle = enabled;
		return o;
	}

	PanelOptions notitle() const {
		return withTitle(false);
	}

	PanelOptions withSmallLogo(bool enabled = true) const {
		PanelOptions o = *this;
		o.showSmallLogo = enabled;
		return o;
	}

	PanelOptions nologo() const {
		return withSmallLogo(false);
	}
};

struct BasePanel : Widget {
	int hp = 0;
	PanelOptions options;
	std::string title;

	BasePanel(
		int hp_,
		const char* title_ = nullptr,
		PanelOptions options_ = PanelOptions::plain()
	) : hp(hp_), options(options_), title(title_ ? title_ : "") {
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
	}

	virtual void drawPanelBase(const Widget::DrawArgs& args) {
		nvgBeginPath(args.vg);
		nvgRect(args.vg, 0.f, 0.f, box.size.x, box.size.y);
		nvgFillColor(args.vg, SubTheme::panelBg());
		nvgFill(args.vg);

		if (options.showSmallLogo) {
			SubLogo::drawLogo(args, box.size);
		}

		if (options.showTitle && !title.empty()) {
			SubDraw::drawModTitle(args, *this, title.c_str());
		}
	}

	void draw(const Widget::DrawArgs& args) override {
		drawPanelBase(args);
	}
};

struct TitledPanel : BasePanel {
	TitledPanel(
		int hp_,
		const char* title_,
		PanelOptions options_ = PanelOptions::titled()
	) : BasePanel(hp_, title_, options_) {
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

	LabeledPanel(
		int hp_,
		const char* title_,
		PanelOptions options_ = PanelOptions::titled()
	) : TitledPanel(hp_, title_, options_) {
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
		drawPanelBase(args);
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

static inline BasePanel* generatePanel(
	int hp,
	PanelOptions options = PanelOptions::plain()
) {
	return new BasePanel(hp, nullptr, options);
}

static inline BasePanel* generatePanel(
	const char* hpText,
	PanelOptions options = PanelOptions::plain()
) {
	if (!hpText) {
		return new BasePanel(0, nullptr, options);
	}
	return new BasePanel(std::atoi(hpText), nullptr, options);
}

static inline TitledPanel* createPanel(
	int hp,
	const char* title,
	PanelOptions options = PanelOptions::titled()
) {
	return new TitledPanel(hp, title, options);
}

static inline LabeledPanel* createLabeledPanel(
	int hp,
	const char* title,
	PanelOptions options = PanelOptions::titled()
) {
	return new LabeledPanel(hp, title, options);
}

} // namespace SubPanel

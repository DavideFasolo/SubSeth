#include "plugin.hpp"

#include "shared/SubLogoPath.hpp"
#include "shared/SubModuleWidget.hpp"
#include "shared/SubPanel.hpp"
#include "shared/SubTheme.hpp"

#include <algorithm>
#include <cmath>

struct UB20 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	UB20() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}
};

struct UB20Panel : SubPanel::LabeledPanel {
	static constexpr float LOGO_X_MARGIN_RATIO = 0.08f;
	static constexpr float LOGO_Y_MARGIN = 38.f;

	UB20Panel()
	: SubPanel::LabeledPanel(
		20,
		"U-B-20",
		SubPanel::PanelOptions::plain().notitle().nologo()
	) {
	}

	void draw(const Widget::DrawArgs& args) override {
		LabeledPanel::draw(args);

		const float maxW = box.size.x * (1.f - LOGO_X_MARGIN_RATIO * 2.f);
		const float maxH = box.size.y - LOGO_Y_MARGIN * 2.f;
		const float sx = maxW / SubLogoPath::WIDTH;
		const float sy = maxH / SubLogoPath::HEIGHT;
		const float scale = std::min(sx, sy);

		const float logoW = SubLogoPath::WIDTH * scale;
		const float logoH = SubLogoPath::HEIGHT * scale;
		const float x = std::round((box.size.x - logoW) * 0.5f);
		const float y = std::round((box.size.y - logoH) * 0.5f);

		nvgSave(args.vg);
		nvgTranslate(args.vg, x, y);
		nvgScale(args.vg, scale, scale);
		nvgTranslate(args.vg, -SubLogoPath::MIN_X, -SubLogoPath::MIN_Y);
		nvgBeginPath(args.vg);
		SubLogoPath::build(args.vg);
		nvgFillColor(args.vg, SubTheme::modTitle());
		nvgFill(args.vg);
		nvgRestore(args.vg);
	}
};

struct UB20Widget : SubModuleWidget<UB20, UB20Panel> {
	UB20Widget(UB20* module) : SubModuleWidget(module) {
	}
};

Model* modelUB20 = createModel<UB20, UB20Widget>("U-B-20");

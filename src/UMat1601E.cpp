#include "plugin.hpp"

#include "shared/SubPanel.hpp"
#include "shared/SubModuleWidget.hpp"
#include "shared/SubButton.hpp"
#include "shared/SubPorts.hpp"
#include "shared/PortLayout.hpp"

struct UMat1601E : Module {
	enum ParamIds {
		ENUMS(A_PARAM, 16),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(INPUT, 16),
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_A,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(A_LIGHT, 16),
		NUM_LIGHTS
	};

	bool prevA[16] = {};

	UMat1601E() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		for (int i = 0; i < 16; ++i) {
			configInput(INPUT + i, string::f("in%d", i + 1));
			configSwitch(A_PARAM + i, 0.f, 1.f, 0.f, string::f("Select in%d for outA", i + 1));
		}

		configOutput(OUT_A, "outA");
	}

	void process(const ProcessArgs& args) override {
		int risingIndex = -1;
		int firstOnIndex = -1;
		int onCount = 0;

		for (int i = 0; i < 16; ++i) {
			const bool on = params[A_PARAM + i].getValue() > 0.5f;

			if (on && !prevA[i]) {
				risingIndex = i;
			}

			if (on) {
				if (firstOnIndex < 0) {
					firstOnIndex = i;
				}
				++onCount;
			}
		}

		if (risingIndex >= 0) {
			for (int i = 0; i < 16; ++i) {
				params[A_PARAM + i].setValue(i == risingIndex ? 1.f : 0.f);
			}
		}
		else if (onCount > 1) {
			for (int i = 0; i < 16; ++i) {
				params[A_PARAM + i].setValue(i == firstOnIndex ? 1.f : 0.f);
			}
		}

		float outA = 0.f;

		for (int i = 0; i < 16; ++i) {
			const bool on = params[A_PARAM + i].getValue() > 0.5f;

			if (on) {
				outA += inputs[INPUT + i].getVoltage();
			}

			lights[A_LIGHT + i].setBrightnessSmooth(on ? 1.f : 0.f, args.sampleTime);
			prevA[i] = on;
		}

		outputs[OUT_A].setVoltage(outA);
	}
};

struct UMat1601EPanel : SubPanel::LabeledPanel {
	float groupX = 0.f;
	float groupY = 0.f;
	float groupW = 0.f;
	float groupH = 0.f;
	float groupR = 5.f;

	UMat1601EPanel() : SubPanel::LabeledPanel(8, "U-Mat-1601E") {}

	void draw(const Widget::DrawArgs& args) override {
		TitledPanel::draw(args);

		if (groupW > 0.f && groupH > 0.f) {
			nvgBeginPath(args.vg);
			nvgRoundedRect(args.vg, groupX, groupY, groupW, groupH, groupR);
			nvgFillColor(args.vg, SubTheme::panelDark());
			nvgFill(args.vg);
		}

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

struct UMat1601EWidget : SubModuleWidget<UMat1601E, UMat1601EPanel> {
	UMat1601EWidget(UMat1601E* module) : SubModuleWidget<UMat1601E, UMat1601EPanel>(module) {
		const float yTop = 45.0f;
		const float buttonDiameter = 12.75f;
		const float oldStep = (362.0f - 18.0f) / 15.0f;
		const float rowStep = buttonDiameter + (oldStep - buttonDiameter) * 0.5f + 2.0f;

		const ZigZagPortLayout inputLayout = {
			16,   // count
			33.f, // centerX
			35.f, // topY
			26.f, // dx
			16.f  // dy
		};
		const auto inputPts = makeZigZagPortCenters(inputLayout);

		const float xA = 82.f;
		const Vec outPos = Vec(33.f, 328.f);

		SubPanel::LabelPlacement rightNumPlacement;
		rightNumPlacement.angDeg = 0.f;
		rightNumPlacement.dist = 15.f;
		rightNumPlacement.justify = SubDraw::Justify::Left;

		SubPanel::LabelPlacement topColPlacement;
		topColPlacement.angDeg = 90.f;
		topColPlacement.dist = 16.1f;
		topColPlacement.justify = SubDraw::Justify::Center;

		const float buttonR = buttonDiameter * 0.5f;
		panel->groupX = xA - buttonR - 10.f;
		panel->groupY = (yTop - topColPlacement.dist) - 8.f;
		panel->groupW = buttonDiameter + rightNumPlacement.dist + 15.f;
		panel->groupH = (rowStep * 15.f) + buttonDiameter + topColPlacement.dist + 10.f;
		panel->groupR = 4.f;

		for (int i = 0; i < 16; ++i) {
			const float y = yTop + rowStep * i;

			addInput(createInputCentered<SilverPort>(
				inputPts[i], module, UMat1601E::INPUT + i
			));

			auto* pA = SubButton::addLightedParamCentered<SubButton::CyanLatch, SubButton::CyanLight>(
				this, Vec(xA, y), module, UMat1601E::A_PARAM + i, UMat1601E::A_LIGHT + i
			);

			panel->addObjLabel(pA, std::to_string(i + 1).c_str(), rightNumPlacement);

			if (i == 0) {
				panel->addObjLabel(pA, "A", topColPlacement);
			}
		}

		addOutput(createOutputCentered<GreenPort>(
			outPos, module, UMat1601E::OUT_A
		));
	}
};

Model* modelUMat1601E = createModel<UMat1601E, UMat1601EWidget>("U-Mat-1601E");

#include "plugin.hpp"
#include "shared/SubPanel.hpp"
#include "shared/SubModuleWidget.hpp"
#include "shared/SubButton.hpp"
#include "shared/SubPorts.hpp"
#include "shared/PortLayout.hpp"

struct UMat1604 : Module {
	enum ParamIds {
		ENUMS(A_PARAM, 16),
		ENUMS(B_PARAM, 16),
		ENUMS(C_PARAM, 16),
		ENUMS(D_PARAM, 16),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(INPUT, 16),
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_A,
		OUT_B,
		OUT_C,
		OUT_D,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(A_LIGHT, 16),
		ENUMS(B_LIGHT, 16),
		ENUMS(C_LIGHT, 16),
		ENUMS(D_LIGHT, 16),
		NUM_LIGHTS
	};

	UMat1604() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		for (int i = 0; i < 16; ++i) {
			configInput(INPUT + i, string::f("in%d", i + 1));

			configSwitch(A_PARAM + i, 0.f, 1.f, 0.f, string::f("Route in%d to outA", i + 1));
			configSwitch(B_PARAM + i, 0.f, 1.f, 0.f, string::f("Route in%d to outB", i + 1));
			configSwitch(C_PARAM + i, 0.f, 1.f, 0.f, string::f("Route in%d to outC", i + 1));
			configSwitch(D_PARAM + i, 0.f, 1.f, 0.f, string::f("Route in%d to outD", i + 1));
		}

		configOutput(OUT_A, "outA");
		configOutput(OUT_B, "outB");
		configOutput(OUT_C, "outC");
		configOutput(OUT_D, "outD");
	}

	void process(const ProcessArgs& args) override {
		float outA = 0.f;
		float outB = 0.f;
		float outC = 0.f;
		float outD = 0.f;

		for (int i = 0; i < 16; ++i) {
			const float in = inputs[INPUT + i].getVoltage();

			const float a = params[A_PARAM + i].getValue();
			const float b = params[B_PARAM + i].getValue();
			const float c = params[C_PARAM + i].getValue();
			const float d = params[D_PARAM + i].getValue();

			if (a > 0.5f) outA += in;
			if (b > 0.5f) outB += in;
			if (c > 0.5f) outC += in;
			if (d > 0.5f) outD += in;

			lights[A_LIGHT + i].setBrightnessSmooth(a, args.sampleTime);
			lights[B_LIGHT + i].setBrightnessSmooth(b, args.sampleTime);
			lights[C_LIGHT + i].setBrightnessSmooth(c, args.sampleTime);
			lights[D_LIGHT + i].setBrightnessSmooth(d, args.sampleTime);
		}

		outputs[OUT_A].setVoltage(outA);
		outputs[OUT_B].setVoltage(outB);
		outputs[OUT_C].setVoltage(outC);
		outputs[OUT_D].setVoltage(outD);
	}
};

struct UMat1604Panel : SubPanel::LabeledPanel {
	float groupX = 0.f;
	float groupY = 0.f;
	float groupW = 0.f;
	float groupH = 0.f;
	float groupR = 5.f;

	UMat1604Panel() : SubPanel::LabeledPanel(12, "U-Mat-1604") {}

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

struct UMat1604Widget : SubModuleWidget<UMat1604, UMat1604Panel> {
	UMat1604Widget(UMat1604* module) : SubModuleWidget<UMat1604, UMat1604Panel>(module) {
		const float yTop = 45.0f;
		const float buttonDiameter = 12.75f;
		const float oldStep = (362.0f - 18.0f) / 15.0f;
		const float rowStep = buttonDiameter + (oldStep - buttonDiameter) * 0.5f + 2.0f;

		const ZigZagPortLayout inputLayout = {
			16,    // count
			33.f,  // centerX
			35.f,  // topY
			26.f,  // dx
			16.f   // dy
		};
		const auto inputPts = makeZigZagPortCenters(inputLayout);

		const ZigZagPortLayout outputLayout = {
			4,     // count
			33.f,  // centerX
			310.f, // topY
			26.f,  // dx
			12.f   // dy
		};
		const auto outputPts = makeZigZagPortCenters(outputLayout);

		const float xA = 85.f;
		const float xB = xA + rowStep;
		const float xC = xB + rowStep;
		const float xD = xC + rowStep;

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
		panel->groupW = (xD - xA) + buttonDiameter + rightNumPlacement.dist + 15.f;
		panel->groupH = (rowStep * 15.f) + buttonDiameter + topColPlacement.dist + 10.f;
		panel->groupR = 4.f;

		for (int i = 0; i < 16; ++i) {
			const float y = yTop + rowStep * i;

			addInput(createInputCentered<SilverPort>(
				inputPts[i], module, UMat1604::INPUT + i
			));

			auto* pA = SubButton::addLightedParamCentered<SubButton::PinkLatch, SubButton::PinkLight>(
				this, Vec(xA, y), module, UMat1604::A_PARAM + i, UMat1604::A_LIGHT + i
			);

			auto* pB = SubButton::addLightedParamCentered<SubButton::PinkLatch, SubButton::PinkLight>(
				this, Vec(xB, y), module, UMat1604::B_PARAM + i, UMat1604::B_LIGHT + i
			);

			auto* pC = SubButton::addLightedParamCentered<SubButton::PinkLatch, SubButton::PinkLight>(
				this, Vec(xC, y), module, UMat1604::C_PARAM + i, UMat1604::C_LIGHT + i
			);

			auto* pD = SubButton::addLightedParamCentered<SubButton::PinkLatch, SubButton::PinkLight>(
				this, Vec(xD, y), module, UMat1604::D_PARAM + i, UMat1604::D_LIGHT + i
			);

			panel->addObjLabel(pD, std::to_string(i + 1).c_str(), rightNumPlacement);

			if (i == 0) {
				panel->addObjLabel(pA, "A", topColPlacement);
				panel->addObjLabel(pB, "B", topColPlacement);
				panel->addObjLabel(pC, "C", topColPlacement);
				panel->addObjLabel(pD, "D", topColPlacement);
			}
		}

		addOutput(createOutputCentered<GreenPort>(
			outputPts[0], module, UMat1604::OUT_A
		));
		addOutput(createOutputCentered<GreenPort>(
			outputPts[1], module, UMat1604::OUT_B
		));
		addOutput(createOutputCentered<GreenPort>(
			outputPts[2], module, UMat1604::OUT_C
		));
		addOutput(createOutputCentered<GreenPort>(
			outputPts[3], module, UMat1604::OUT_D
		));
	}
};

Model* modelUMat1604 = createModel<UMat1604, UMat1604Widget>("U-Mat-1604");

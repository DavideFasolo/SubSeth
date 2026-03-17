#include "plugin.hpp"
#include "shared/PortLayout.hpp"
#include "shared/SubPanel.hpp"
#include "shared/SubControls.hpp"
#include "shared/SubConnector.hpp"
#include "shared/SubModuleWidget.hpp"
#include <vector>

static constexpr int U_POL_S_OUTPUT_COUNT = 16;

static const ZigZagPortLayout U_POL_S_OUTPUT_LAYOUT = {
	U_POL_S_OUTPUT_COUNT,
	30.f, // centerX
	95.f, // topY
	26.f, // dx
	16.f  // dy
};

static const Vec U_POL_S_INPUT_POS = Vec(30.f, 40.f);

static inline ConnectorSpec makeUPolSPolyInSpec(int id) {
	return makePolyInput(
		id,
		1,
		U_POL_S_INPUT_POS,
		"Poly in",
		"Poly IN"
	);
}

static std::vector<ConnectorSpec> makeUPolSOutputSpecs();

struct UPolS : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		POLY_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_1_OUTPUT,
		OUT_2_OUTPUT,
		OUT_3_OUTPUT,
		OUT_4_OUTPUT,
		OUT_5_OUTPUT,
		OUT_6_OUTPUT,
		OUT_7_OUTPUT,
		OUT_8_OUTPUT,
		OUT_9_OUTPUT,
		OUT_10_OUTPUT,
		OUT_11_OUTPUT,
		OUT_12_OUTPUT,
		OUT_13_OUTPUT,
		OUT_14_OUTPUT,
		OUT_15_OUTPUT,
		OUT_16_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	UPolS() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		SubModule::configConnector(this, makeUPolSPolyInSpec(POLY_INPUT));

		auto outputSpecs = makeUPolSOutputSpecs();
		for (const auto& c : outputSpecs) {
			SubModule::configConnector(this, c);
		}
	}

	void process(const ProcessArgs& args) override {
		int inChannels = 0;
		if (inputs[POLY_INPUT].isConnected()) {
			inChannels = inputs[POLY_INPUT].getChannels();
		}

		for (int i = 0; i < U_POL_S_OUTPUT_COUNT; ++i) {
			float v = 0.f;
			if (i < inChannels) {
				v = inputs[POLY_INPUT].getVoltage(i);
			}

			outputs[OUT_1_OUTPUT + i].setChannels(1);
			outputs[OUT_1_OUTPUT + i].setVoltage(v, 0);
		}
	}
};

static std::vector<ConnectorSpec> makeUPolSOutputSpecs() {
	return makeMonoOutputSpecsFromPoints(
		makeZigZagPortCenters(U_POL_S_OUTPUT_LAYOUT),
		UPolS::OUT_1_OUTPUT
	);
}

struct UPolSPanel : SubPanel::LabeledPanel {
	UPolSPanel() : SubPanel::LabeledPanel(4, "U·Pol·S") {}
};

struct UPolSWidget : SubModuleWidget<UPolS, UPolSPanel> {
	UPolSWidget(UPolS* module) : SubModuleWidget(module) {
		addConnector(
			makeUPolSPolyInSpec(UPolS::POLY_INPUT)
		);

		auto outputSpecs = makeUPolSOutputSpecs();
		for (const auto& c : outputSpecs) {
			addConnector(c);
		}
	}
};

Model* modelUPolS = createModel<UPolS, UPolSWidget>("U-Pol-S");

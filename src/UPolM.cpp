#include "plugin.hpp"
#include "shared/PortLayout.hpp"
#include "shared/SubPanel.hpp"
#include "shared/SubControls.hpp"
#include "shared/SubConnector.hpp"
#include "shared/SubModuleWidget.hpp"
#include <vector>

static constexpr int U_POL_M_INPUT_COUNT = 16;

static const ZigZagPortLayout U_POL_M_INPUT_LAYOUT = {
	U_POL_M_INPUT_COUNT,
	30.f, // centerX
	95.f, // topY
	26.f, // dx
	16.f  // dy
};

static const Vec U_POL_M_OUTPUT_POS = Vec(30.f, 40.f);

static inline ConnectorSpec makeUPolMPolyOutSpec(int id) {
	return makePolyOutput(
		id,
		1,
		U_POL_M_OUTPUT_POS,
		"Poly out",
		"Poly OUT"
	);
}

static std::vector<ConnectorSpec> makeUPolMInputSpecs();

struct UPolM : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		IN_1_INPUT,
		IN_2_INPUT,
		IN_3_INPUT,
		IN_4_INPUT,
		IN_5_INPUT,
		IN_6_INPUT,
		IN_7_INPUT,
		IN_8_INPUT,
		IN_9_INPUT,
		IN_10_INPUT,
		IN_11_INPUT,
		IN_12_INPUT,
		IN_13_INPUT,
		IN_14_INPUT,
		IN_15_INPUT,
		IN_16_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		POLY_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	UPolM() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		SubModule::configConnector(this, makeUPolMPolyOutSpec(POLY_OUTPUT));

		auto inputSpecs = makeUPolMInputSpecs();
		for (const auto& c : inputSpecs) {
			SubModule::configConnector(this, c);
		}
	}

	void process(const ProcessArgs& args) override {
		int lastConnected = -1;

		for (int i = 0; i < U_POL_M_INPUT_COUNT; ++i) {
			if (inputs[IN_1_INPUT + i].isConnected()) {
				lastConnected = i;
			}
		}

		if (lastConnected < 0) {
			outputs[POLY_OUTPUT].setChannels(1);
			outputs[POLY_OUTPUT].setVoltage(0.f, 0);
			return;
		}

		int channels = lastConnected + 1;
		outputs[POLY_OUTPUT].setChannels(channels);

		for (int c = 0; c < channels; ++c) {
			float v = 0.f;
			if (inputs[IN_1_INPUT + c].isConnected()) {
				v = inputs[IN_1_INPUT + c].getVoltage();
			}
			outputs[POLY_OUTPUT].setVoltage(v, c);
		}
	}
};

static std::vector<ConnectorSpec> makeUPolMInputSpecs() {
	return makeMonoInputSpecsFromPoints(
		makeZigZagPortCenters(U_POL_M_INPUT_LAYOUT),
		UPolM::IN_1_INPUT
	);
}

struct UPolMPanel : SubPanel::LabeledPanel {
	UPolMPanel() : SubPanel::LabeledPanel(4, "U·Pol·M") {}
};

struct UPolMWidget : SubModuleWidget<UPolM, UPolMPanel> {
	UPolMWidget(UPolM* module) : SubModuleWidget(module) {

		addConnector(
			makeUPolMPolyOutSpec(UPolM::POLY_OUTPUT)
		);

		auto inputSpecs = makeUPolMInputSpecs();
		for (const auto& c : inputSpecs) {
			addConnector(c);
		}
	}
};

Model* modelUPolM = createModel<UPolM, UPolMWidget>("U-Pol-M");

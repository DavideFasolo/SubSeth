#include "plugin.hpp"

#include "shared/SubPanel.hpp"
#include "shared/SubModuleWidget.hpp"
#include "shared/SubPorts.hpp"
#include "shared/SubKnob.hpp"

struct UKnob1 : Module {
	enum ParamIds {
		KNOB_PARAM,
		NUM_PARAMS
	};

	enum InputIds {
		NUM_INPUTS
	};

	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};

	enum LightIds {
		KNOB_LIGHT,
		NUM_LIGHTS
	};

	UKnob1() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(KNOB_PARAM, 0.f, 1.f, 0.5f, "Knob", " V", 0.f, 10.f);
		configOutput(OUT_OUTPUT, "Knob CV");
	}

	void process(const ProcessArgs& args) override {
		(void) args;
		outputs[OUT_OUTPUT].setVoltage(params[KNOB_PARAM].getValue() * 10.f);

		// Vera light VCV sempre accesa
		lights[KNOB_LIGHT].setBrightness(1.f);
	}
};

struct UKnob1Panel : SubPanel::LabeledPanel {
	UKnob1Panel() : SubPanel::LabeledPanel(8, "U-Knob-1") {}
};

struct UKnob1Widget : SubModuleWidget<UKnob1, UKnob1Panel> {
	UKnob1Widget(UKnob1* module) : SubModuleWidget<UKnob1, UKnob1Panel>(module) {
		auto* knob = createParamCentered<SubKnob::DefaultKnob>(Vec(30.f, 88.f), module, UKnob1::KNOB_PARAM);
		knob->lightId = UKnob1::KNOB_LIGHT;
		addParam(knob);

		addOutput(createOutputCentered<GreenPort>(Vec(30.f, 176.f), module, UKnob1::OUT_OUTPUT));
	}
};

Model* modelUKnob1 = createModel<UKnob1, UKnob1Widget>("U-Knob-1");

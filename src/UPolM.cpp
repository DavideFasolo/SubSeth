#include "plugin.hpp"
#include "shared/SubPorts.hpp"

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
		configOutput(POLY_OUTPUT, "Poly out");

		configInput(IN_1_INPUT, "Input 1");
		configInput(IN_2_INPUT, "Input 2");
		configInput(IN_3_INPUT, "Input 3");
		configInput(IN_4_INPUT, "Input 4");
		configInput(IN_5_INPUT, "Input 5");
		configInput(IN_6_INPUT, "Input 6");
		configInput(IN_7_INPUT, "Input 7");
		configInput(IN_8_INPUT, "Input 8");
		configInput(IN_9_INPUT, "Input 9");
		configInput(IN_10_INPUT, "Input 10");
		configInput(IN_11_INPUT, "Input 11");
		configInput(IN_12_INPUT, "Input 12");
		configInput(IN_13_INPUT, "Input 13");
		configInput(IN_14_INPUT, "Input 14");
		configInput(IN_15_INPUT, "Input 15");
		configInput(IN_16_INPUT, "Input 16");
	}

	void process(const ProcessArgs& args) override {
		int lastConnected = -1;

		for (int i = 0; i < 16; ++i) {
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

struct UPolMWidget : ModuleWidget {
	UPolMWidget(UPolM* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/U-Pol-M.svg")));

		addOutput(createOutputCentered<BluePort>(Vec(30.f, 54.f), module, UPolM::POLY_OUTPUT));

		addInput(createInputCentered<SilverPort>(Vec(17.f, 92.f), module, UPolM::IN_1_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 108.f), module, UPolM::IN_2_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 124.f), module, UPolM::IN_3_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 140.f), module, UPolM::IN_4_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 156.f), module, UPolM::IN_5_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 172.f), module, UPolM::IN_6_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 188.f), module, UPolM::IN_7_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 204.f), module, UPolM::IN_8_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 220.f), module, UPolM::IN_9_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 236.f), module, UPolM::IN_10_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 252.f), module, UPolM::IN_11_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 268.f), module, UPolM::IN_12_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 284.f), module, UPolM::IN_13_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 300.f), module, UPolM::IN_14_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(17.f, 316.f), module, UPolM::IN_15_INPUT));
		addInput(createInputCentered<SilverPort>(Vec(43.f, 332.f), module, UPolM::IN_16_INPUT));
	}
};

Model* modelUPolM = createModel<UPolM, UPolMWidget>("U-Pol-M");

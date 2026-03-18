#include "plugin.hpp"
#include "shared/SubLabel.hpp"
#include "shared/SubModuleWidget.hpp"
#include "shared/SubPanel.hpp"
#include <algorithm>

static constexpr int U_MULT8_ROWS = 8;

static const float U_MULT8_PANEL_W  = RACK_GRID_WIDTH * 12.f;
static const float U_MULT8_MARGIN_Y = 30.f;
static const float U_MULT8_ROW_STEP = (RACK_GRID_HEIGHT - 2.f * U_MULT8_MARGIN_Y) / (U_MULT8_ROWS - 1);

static const float U_MULT8_INPUT_X  = 18.f;
static const float U_MULT8_OUTPUT_X = U_MULT8_PANEL_W - 18.f;
static const float U_MULT8_LABEL_X  = 0.5f * (U_MULT8_PANEL_W - SubLabel::WIDTH);

static inline float umult8RowCenterY(int row) {
	return U_MULT8_MARGIN_Y + row * U_MULT8_ROW_STEP;
}

struct UMult8 : Module {
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
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	SubLabel::State labels[U_MULT8_ROWS];

	UMult8() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		for (int i = 0; i < U_MULT8_ROWS; ++i) {
			configInput(IN_1_INPUT + i, string::f("Signal in %d", i + 1));
			configOutput(OUT_1_OUTPUT + i, string::f("Signal out %d", i + 1));
		}
	}

	json_t* dataToJson() override {
		json_t* rootJ = json_object();
		json_t* labelsJ = json_array();

		for (int i = 0; i < U_MULT8_ROWS; ++i) {
			json_array_append_new(labelsJ, labels[i].toJson());
		}

		json_object_set_new(rootJ, "labels", labelsJ);
		return rootJ;
	}

	void dataFromJson(json_t* rootJ) override {
		json_t* labelsJ = json_object_get(rootJ, "labels");
		if (!json_is_array(labelsJ)) {
			return;
		}

		int count = std::min(U_MULT8_ROWS, (int) json_array_size(labelsJ));
		for (int i = 0; i < count; ++i) {
			labels[i].fromJson(json_array_get(labelsJ, i));
		}
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < U_MULT8_ROWS; ++i) {
			int inId  = IN_1_INPUT + i;
			int outId = OUT_1_OUTPUT + i;

			if (!inputs[inId].isConnected()) {
				outputs[outId].setChannels(0);
				continue;
			}

			int channels = std::max(1, inputs[inId].getChannels());
			outputs[outId].setChannels(channels);

			for (int c = 0; c < channels; ++c) {
				outputs[outId].setVoltage(inputs[inId].getVoltage(c), c);
			}
		}
	}
};

struct UMult8Panel : SubPanel::LabeledPanel {
	UMult8Panel() : SubPanel::LabeledPanel(12, "U·Mult8") {}
};

struct UMult8Widget : SubModuleWidget<UMult8, UMult8Panel> {
	UMult8Widget(UMult8* module) : SubModuleWidget(module) {
		for (int i = 0; i < U_MULT8_ROWS; ++i) {
			float y = umult8RowCenterY(i);

			addInput(createInputCentered<BlackPort>(
				Vec(U_MULT8_INPUT_X, y),
				module,
				UMult8::IN_1_INPUT + i
			));

			addOutput(createOutputCentered<BlackPort>(
				Vec(U_MULT8_OUTPUT_X, y),
				module,
				UMult8::OUT_1_OUTPUT + i
			));

			SubLabel::State* state = module ? &module->labels[i] : nullptr;
			addChild(SubLabel::createLabelDisplay(
				U_MULT8_LABEL_X,
				y - 0.5f * SubLabel::HEIGHT,
				state
			));
		}
	}
};

Model* modelUMult8 = createModel<UMult8, UMult8Widget>("U-Mult8");

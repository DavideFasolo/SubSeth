#pragma once

#include "../plugin.hpp"

#include "SubLabel.hpp"
#include "SubModuleWidget.hpp"
#include "SubPanel.hpp"
#include <algorithm>

template <int Rows>
struct UMultModule : Module {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        NUM_INPUTS = Rows
    };
    enum OutputIds {
        NUM_OUTPUTS = Rows
    };
    enum LightIds {
        NUM_LIGHTS
    };

    UMultModule() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int i = 0; i < Rows; ++i) {
            configInput(i, string::f("Signal in %d", i + 1));
            configOutput(i, string::f("Signal out %d", i + 1));
        }
    }

    void process(const ProcessArgs& args) override {
        for (int i = 0; i < Rows; ++i) {
            if (!inputs[i].isConnected()) {
                outputs[i].setChannels(0);
                continue;
            }

            int channels = std::max(1, inputs[i].getChannels());
            outputs[i].setChannels(channels);
            for (int c = 0; c < channels; ++c) {
                outputs[i].setVoltage(inputs[i].getVoltage(c), c);
            }
        }
    }
};

template <int Rows>
struct UMultLabeledModule : Module {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        NUM_INPUTS = Rows
    };
    enum OutputIds {
        NUM_OUTPUTS = Rows
    };
    enum LightIds {
        NUM_LIGHTS
    };

    SubLabel::State labels[Rows];

    UMultLabeledModule() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int i = 0; i < Rows; ++i) {
            configInput(i, string::f("Signal in %d", i + 1));
            configOutput(i, string::f("Signal out %d", i + 1));
        }
    }

    json_t* dataToJson() override {
        json_t* rootJ = json_object();
        json_t* labelsJ = json_array();
        for (int i = 0; i < Rows; ++i) {
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

        int count = std::min(Rows, (int) json_array_size(labelsJ));
        for (int i = 0; i < count; ++i) {
            labels[i].fromJson(json_array_get(labelsJ, i));
        }
    }

    void process(const ProcessArgs& args) override {
        for (int i = 0; i < Rows; ++i) {
            if (!inputs[i].isConnected()) {
                outputs[i].setChannels(0);
                continue;
            }

            int channels = std::max(1, inputs[i].getChannels());
            outputs[i].setChannels(channels);
            for (int c = 0; c < channels; ++c) {
                outputs[i].setVoltage(inputs[i].getVoltage(c), c);
            }
        }
    }
};

template <int Rows>
static inline float umultRowCenterY(float marginY, int row) {
    const float rowStep = (RACK_GRID_HEIGHT - 2.f * marginY) / (Rows - 1);
    return marginY + row * rowStep;
}

template <int Rows, typename ModuleT, typename PanelT>
struct UMultCompactWidget : SubModuleWidget<ModuleT, PanelT> {
    UMultCompactWidget(ModuleT* module, int panelHp)
    : SubModuleWidget<ModuleT, PanelT>(module) {
        const float panelW = RACK_GRID_WIDTH * panelHp;
        const float marginY = 30.f;
        const float inputX = 15.f;
        const float outputX = panelW - 15.f;

        for (int i = 0; i < Rows; ++i) {
            float y = umultRowCenterY<Rows>(marginY, i);

            this->addInput(createInputCentered<BlackPort>(
                Vec(inputX, y),
                module,
                i
            ));

            this->addOutput(createOutputCentered<BlackPort>(
                Vec(outputX, y),
                module,
                i
            ));
        }
    }
};

template <int Rows, typename ModuleT, typename PanelT>
struct UMultLabeledWidget : SubModuleWidget<ModuleT, PanelT> {
    UMultLabeledWidget(ModuleT* module, int panelHp)
    : SubModuleWidget<ModuleT, PanelT>(module) {
        const float panelW = RACK_GRID_WIDTH * panelHp;
        const float marginY = 30.f;
        const float inputX = 18.f;
        const float outputX = panelW - 18.f;
        const float labelX = 0.5f * (panelW - SubLabel::WIDTH);

        for (int i = 0; i < Rows; ++i) {
            float y = umultRowCenterY<Rows>(marginY, i);

            this->addInput(createInputCentered<BlackPort>(
                Vec(inputX, y),
                module,
                i
            ));

            this->addOutput(createOutputCentered<BlackPort>(
                Vec(outputX, y),
                module,
                i
            ));

            SubLabel::State* state = module ? &module->labels[i] : nullptr;
            this->addChild(SubLabel::createLabelDisplay(
                labelX,
                y - 0.5f * SubLabel::HEIGHT,
                state
            ));
        }
    }
};

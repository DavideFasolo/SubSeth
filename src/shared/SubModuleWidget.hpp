#pragma once

#include "../plugin.hpp"
#include "SubControls.hpp"

template <typename TModule, typename TPanel>
struct SubModuleWidget : ModuleWidget {
	TPanel* panel = nullptr;
	TModule* moduleRef = nullptr;

	SubModuleWidget(TModule* module) {
		moduleRef = module;
		setModule(module);

		panel = new TPanel();
		box.size = panel->box.size;
		addChild(panel);
	}

	PortWidget* addConnector(const ConnectorSpec& c) {
		return SubControls::addConnector(this, panel, moduleRef, c);
	}
};

#pragma once

#include "../plugin.hpp"
#include "SubPanel.hpp"
#include "SubConnector.hpp"
#include "SubPorts.hpp"

namespace SubControls {

template <typename TPort>
static inline TPort* addInput(
	ModuleWidget* moduleWidget,
	SubPanel::LabeledPanel* panel,
	Module* module,
	Vec pos,
	int inputId,
	const char* labelText = nullptr,
	SubPanel::LabelPlacement placement = {}
) {
	TPort* port = createInputCentered<TPort>(pos, module, inputId);
	moduleWidget->addInput(port);

	if (panel && labelText && labelText[0]) {
		panel->addObjLabel(port, labelText, placement);
	}

	return port;
}

template <typename TPort>
static inline TPort* addOutput(
	ModuleWidget* moduleWidget,
	SubPanel::LabeledPanel* panel,
	Module* module,
	Vec pos,
	int outputId,
	const char* labelText = nullptr,
	SubPanel::LabelPlacement placement = {}
) {
	TPort* port = createOutputCentered<TPort>(pos, module, outputId);
	moduleWidget->addOutput(port);

	if (panel && labelText && labelText[0]) {
		panel->addObjLabel(port, labelText, placement);
	}

	return port;
}

static inline PortWidget* addConnector(
	ModuleWidget* moduleWidget,
	SubPanel::LabeledPanel* panel,
	Module* module,
	const ConnectorSpec& c
) {
	if (!moduleWidget || !module || c.id < 0) {
		return nullptr;
	}

	PortWidget* port = nullptr;
	ConnectorTheme theme = resolveTheme(c);

	if (c.type == ConnectorType::Input) {
		switch (theme) {
			case ConnectorTheme::Silver:
				port = createInputCentered<SilverPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Red:
				port = createInputCentered<RedPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Blue:
				port = createInputCentered<BluePort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Black:
				port = createInputCentered<BlackPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Purple:
				port = createInputCentered<PurplePort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Pink:
				port = createInputCentered<PinkPort>(c.pos, module, c.id);
				break;
		}
		if (port) {
			moduleWidget->addInput(port);
		}
	}
	else {
		switch (theme) {
			case ConnectorTheme::Silver:
				port = createOutputCentered<SilverPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Red:
				port = createOutputCentered<RedPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Blue:
				port = createOutputCentered<BluePort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Black:
				port = createOutputCentered<BlackPort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Purple:
				port = createOutputCentered<PurplePort>(c.pos, module, c.id);
				break;
			case ConnectorTheme::Pink:
				port = createOutputCentered<PinkPort>(c.pos, module, c.id);
				break;
		}
		if (port) {
			moduleWidget->addOutput(port);
		}
	}

	if (panel && port && c.label && c.label[0]) {
		panel->addObjLabel(port, c.label, c.placement);
	}

	return port;
}

} // namespace SubControls

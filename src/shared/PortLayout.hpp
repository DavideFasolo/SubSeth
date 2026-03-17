#pragma once

#include "../plugin.hpp"
#include "SubPorts.hpp"
#include <vector>

enum class PortStyle {
	Silver,
	Blue,
	Red,
	Black
};

struct InputPortSpec {
	int inputId;
	PortStyle style;
};

struct OutputPortSpec {
	int outputId;
	PortStyle style;
};

struct ZigZagPortLayout {
	int count;
	float centerX;
	float topY;
	float dx;
	float dy;
};

static inline std::vector<Vec> makeZigZagPortCenters(
	int count,
	float centerX,
	float topY,
	float dx,
	float dy
) {
	std::vector<Vec> pts;
	pts.reserve(count);

	const float leftX  = centerX - dx * 0.5f;
	const float rightX = centerX + dx * 0.5f;

	for (int i = 0; i < count; ++i) {
		const float x = (i % 2 == 0) ? leftX : rightX;
		const float y = topY + i * dy;
		pts.emplace_back(x, y);
	}

	return pts;
}

static inline std::vector<Vec> makeZigZagPortCenters(const ZigZagPortLayout& layout) {
	return makeZigZagPortCenters(
		layout.count,
		layout.centerX,
		layout.topY,
		layout.dx,
		layout.dy
	);
}

static inline void addInputByStyle(
	ModuleWidget* w,
	Module* module,
	Vec pos,
	int inputId,
	PortStyle style
) {
	switch (style) {
		case PortStyle::Silver:
			w->addInput(createInputCentered<SilverPort>(pos, module, inputId));
			break;
		case PortStyle::Blue:
			w->addInput(createInputCentered<BluePort>(pos, module, inputId));
			break;
		case PortStyle::Red:
			w->addInput(createInputCentered<RedPort>(pos, module, inputId));
			break;
		case PortStyle::Black:
			w->addInput(createInputCentered<BlackPort>(pos, module, inputId));
			break;
	}
}

static inline void addOutputByStyle(
	ModuleWidget* w,
	Module* module,
	Vec pos,
	int outputId,
	PortStyle style
) {
	switch (style) {
		case PortStyle::Silver:
			w->addOutput(createOutputCentered<SilverPort>(pos, module, outputId));
			break;
		case PortStyle::Blue:
			w->addOutput(createOutputCentered<BluePort>(pos, module, outputId));
			break;
		case PortStyle::Red:
			w->addOutput(createOutputCentered<RedPort>(pos, module, outputId));
			break;
		case PortStyle::Black:
			w->addOutput(createOutputCentered<BlackPort>(pos, module, outputId));
			break;
	}
}

static inline void addInputZigZagGroup(
	ModuleWidget* w,
	Module* module,
	const std::vector<InputPortSpec>& specs,
	float centerX,
	float topY,
	float dx,
	float dy
) {
	auto pts = makeZigZagPortCenters((int) specs.size(), centerX, topY, dx, dy);
	for (int i = 0; i < (int) specs.size(); ++i) {
		addInputByStyle(w, module, pts[i], specs[i].inputId, specs[i].style);
	}
}

static inline void addInputZigZagGroup(
	ModuleWidget* w,
	Module* module,
	const std::vector<InputPortSpec>& specs,
	const ZigZagPortLayout& layout
) {
	auto pts = makeZigZagPortCenters(layout);
	for (int i = 0; i < (int) specs.size(); ++i) {
		addInputByStyle(w, module, pts[i], specs[i].inputId, specs[i].style);
	}
}

static inline void addOutputZigZagGroup(
	ModuleWidget* w,
	Module* module,
	const std::vector<OutputPortSpec>& specs,
	float centerX,
	float topY,
	float dx,
	float dy
) {
	auto pts = makeZigZagPortCenters((int) specs.size(), centerX, topY, dx, dy);
	for (int i = 0; i < (int) specs.size(); ++i) {
		addOutputByStyle(w, module, pts[i], specs[i].outputId, specs[i].style);
	}
}

static inline void addOutputZigZagGroup(
	ModuleWidget* w,
	Module* module,
	const std::vector<OutputPortSpec>& specs,
	const ZigZagPortLayout& layout
) {
	auto pts = makeZigZagPortCenters(layout);
	for (int i = 0; i < (int) specs.size(); ++i) {
		addOutputByStyle(w, module, pts[i], specs[i].outputId, specs[i].style);
	}
}

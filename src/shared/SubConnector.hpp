#pragma once

#include "../plugin.hpp"
#include "SubPanel.hpp"
#include <vector>

enum class ConnectorTheme {
	Silver,
	Red,
	Blue,
	Black,
	Purple,
	Pink
};

enum class ConnectorSignal {
	Mono,
	Poly
};

enum class ConnectorType {
	Input,
	Output
};

struct ConnectorSpec {
	ConnectorSignal signal;
	ConnectorType type;

	int id;          // enum/input/output id reale del modulo
	int numberId;    // numero umano: 1, 2, 3...

	Vec pos;         // posizione sul pannello

	const char* configName; // nome per configInput/configOutput
	const char* label;      // label opzionale sul pannello

	SubPanel::LabelPlacement placement;

	ConnectorSpec()
		: signal(ConnectorSignal::Mono),
		  type(ConnectorType::Input),
		  id(-1),
		  numberId(0),
		  pos(Vec()),
		  configName(nullptr),
		  label(nullptr),
		  placement() {
	}

	ConnectorSpec(
		ConnectorSignal signal_,
		ConnectorType type_,
		int id_,
		int numberId_,
		Vec pos_,
		const char* configName_ = nullptr,
		const char* label_ = nullptr,
		SubPanel::LabelPlacement placement_ = SubPanel::LabelPlacement()
	)
		: signal(signal_),
		  type(type_),
		  id(id_),
		  numberId(numberId_),
		  pos(pos_),
		  configName(configName_),
		  label(label_),
		  placement(placement_) {
	}
};

static inline ConnectorTheme resolveTheme(const ConnectorSpec& c) {
	switch (c.signal) {
		case ConnectorSignal::Mono:
			return ConnectorTheme::Silver;
		case ConnectorSignal::Poly:
			return ConnectorTheme::Purple;
	}
	return ConnectorTheme::Silver;
}

static inline ConnectorSpec makeMonoInput(
	int id,
	int numberId,
	Vec pos,
	const char* configName = nullptr,
	const char* label = nullptr,
	SubPanel::LabelPlacement placement = SubPanel::LabelPlacement()
) {
	return ConnectorSpec(
		ConnectorSignal::Mono,
		ConnectorType::Input,
		id,
		numberId,
		pos,
		configName,
		label,
		placement
	);
}

static inline ConnectorSpec makeMonoOutput(
	int id,
	int numberId,
	Vec pos,
	const char* configName = nullptr,
	const char* label = nullptr,
	SubPanel::LabelPlacement placement = SubPanel::LabelPlacement()
) {
	return ConnectorSpec(
		ConnectorSignal::Mono,
		ConnectorType::Output,
		id,
		numberId,
		pos,
		configName,
		label,
		placement
	);
}

static inline ConnectorSpec makePolyInput(
	int id,
	int numberId,
	Vec pos,
	const char* configName = nullptr,
	const char* label = nullptr,
	SubPanel::LabelPlacement placement = SubPanel::LabelPlacement()
) {
	return ConnectorSpec(
		ConnectorSignal::Poly,
		ConnectorType::Input,
		id,
		numberId,
		pos,
		configName,
		label,
		placement
	);
}

static inline ConnectorSpec makePolyOutput(
	int id,
	int numberId,
	Vec pos,
	const char* configName = nullptr,
	const char* label = nullptr,
	SubPanel::LabelPlacement placement = SubPanel::LabelPlacement()
) {
	return ConnectorSpec(
		ConnectorSignal::Poly,
		ConnectorType::Output,
		id,
		numberId,
		pos,
		configName,
		label,
		placement
	);
}


template <typename F>
static inline std::vector<ConnectorSpec> makeConnectorSpecs(int count, F makeOne) {
	std::vector<ConnectorSpec> specs;
	specs.reserve(count);

	for (int i = 0; i < count; ++i) {
		specs.push_back(makeOne(i));
	}

	return specs;
}

static inline std::vector<ConnectorSpec> makeMonoInputSpecsFromPoints(
	const std::vector<Vec>& pts,
	int firstId,
	int firstNumber = 1
) {
	return makeConnectorSpecs((int) pts.size(), [&](int i) {
		return makeMonoInput(firstId + i, firstNumber + i, pts[i]);
	});
}

namespace SubModule {

static inline std::string defaultConnectorName(const ConnectorSpec& c) {
	return string::f(
		"%s %d",
		c.type == ConnectorType::Input ? "Input" : "Output",
		c.numberId
	);
}

static inline void configConnector(Module* module, const ConnectorSpec& c) {
	if (!module || c.id < 0) {
		return;
	}

	std::string name;
	if (c.configName && c.configName[0]) {
		name = c.configName;
	}
	else {
		name = defaultConnectorName(c);
	}

	if (c.type == ConnectorType::Input) {
		module->configInput(c.id, name);
	}
	else {
		module->configOutput(c.id, name);
	}
}

} // namespace SubModule

#pragma once

#include "../plugin.hpp"
#include "SubButton.hpp"

#include <cmath>

namespace SubKnob {

using Accent = SubButton::Accent;

static constexpr float kPi = 3.14159265358979323846f;

static inline float lerpf(float a, float b, float t) {
	return a + (b - a) * SubButton::clampf(t, 0.f, 1.f);
}

struct RectValueLight : app::ModuleLightWidget {
	float angle = 0.f;

	RectValueLight(NVGcolor baseColor) {
		addBaseColor(baseColor);
		bgColor = nvgRGBA(0, 0, 0, 0);
		borderColor = nvgRGBA(0, 0, 0, 0);
		box.size = Vec(1.5f, 6.0f);
	}

	void drawBackground(const DrawArgs& args) override {
		(void) args;
	}

	void drawLight(const DrawArgs& args) override {
		(void) args;
		// Corpo della light VCV invisibile: resta solo l'halo.
	}

	void drawHalo(const DrawArgs& args) override {
		if (color.a <= 0.f) {
			return;
		}

		const float cx = box.size.x * 0.5f;
		const float cy = box.size.y * 0.5f;

		nvgSave(args.vg);
		nvgTranslate(args.vg, cx, cy);
		nvgRotate(args.vg, angle);
		nvgTranslate(args.vg, -cx, -cy);

		// Alone puro, senza corpo centrale visibile.
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, 21.0f);
		nvgFillPaint(args.vg, nvgRadialGradient(
			args.vg,
			cx, cy,
			0.15f,
			21.0f,
			nvgRGBAf(color.r, color.g, color.b, 0.36f),
			nvgRGBAf(color.r, color.g, color.b, 0.00f)
		));
		nvgFill(args.vg);

		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, 10.8f);
		nvgFillPaint(args.vg, nvgRadialGradient(
			args.vg,
			cx, cy,
			0.05f,
			10.8f,
			nvgRGBAf(color.r, color.g, color.b, 0.24f),
			nvgRGBAf(color.r, color.g, color.b, 0.00f)
		));
		nvgFill(args.vg);

		nvgRestore(args.vg);
	}
};

struct Knob : app::Knob {
	Accent accent = Accent::Cyan;

	// Geometria principale
	float outerRadius = 20.0f;
	float ringThickness = 1.5f;

	// Posizione LED
	float ledOrbitRadius = 12.0f;

	// Geometria LED grafico
	float ledBodyWidth = 1.2f;
	float ledBodyHeight = 5.0f;
	float ledBodyCorner = 0.40f;
	float ledCoreX = 0.5f;
	float ledCoreY = 0.75f;
	float ledCoreWidth = 0.5f;
	float ledCoreHeight = 4.5f;
	float ledCoreCorner = 0.25f;

	// Gradiente anello
	float ringGradientRadiusFactor = 0.8f;
	float ringGradientShiftFactor = 0.95f;

	int lightId = -1;
	RectValueLight* valueLight = nullptr;

	Knob() {
		box.size = Vec(43.2f, 43.2f);
		minAngle = -0.83f * kPi;
		maxAngle =  0.83f * kPi;
		snap = false;

		valueLight = new RectValueLight(SubButton::accentColor(accent));
		addChild(valueLight);
	}

	float value01() {
		ParamQuantity* pq = getParamQuantity();
		if (!pq) {
			return 0.f;
		}

		const float lo = pq->getMinValue();
		const float hi = pq->getMaxValue();
		if (!(hi > lo)) {
			return 0.f;
		}

		return SubButton::clampf((pq->getValue() - lo) / (hi - lo), 0.f, 1.f);
	}

	float valueAngle() {
		return lerpf(minAngle, maxAngle, value01());
	}

	Vec knobCenter() const {
		return Vec(box.size.x * 0.5f, box.size.y * 0.5f);
	}

	float ledTheta() {
		return valueAngle() - kPi * 0.5f;
	}

	Vec ledCenter() {
		const Vec c = knobCenter();
		const float theta = ledTheta();
		return Vec(
			c.x + std::cos(theta) * ledOrbitRadius,
			c.y + std::sin(theta) * ledOrbitRadius
		);
	}

	void step() override {
		app::Knob::step();

		if (!valueLight) {
			return;
		}

		valueLight->module = module;
		valueLight->firstLightId = lightId;

		const Vec lc = ledCenter();

		valueLight->box.pos = Vec(
			lc.x - valueLight->box.size.x * 0.5f,
			lc.y - valueLight->box.size.y * 0.5f
		);

		// Il LED eredita la rotazione del raggio centro -> LED.
		// Base del rettangolo verticale, quindi +pi/2 per allinearlo al raggio.
		valueLight->angle = ledTheta() + kPi * 0.5f;
	}

	void drawLedBody(const DrawArgs& args, float cx, float cy) {
		(void) cx;
		(void) cy;

		const NVGcolor led = SubButton::accentColor(accent);
		const Vec lc = ledCenter();
		const float rot = valueAngle();

		nvgSave(args.vg);
		nvgTranslate(args.vg, lc.x, lc.y);
		nvgRotate(args.vg, rot);
		nvgTranslate(args.vg, -ledBodyWidth * 0.5f, -ledBodyHeight * 0.5f);

		// Corpo LED grafico
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0.f, 0.f, ledBodyWidth, ledBodyHeight, ledBodyCorner);
		nvgFillColor(args.vg, led);
		nvgFill(args.vg);

		// Core chiaro interno
		nvgBeginPath(args.vg);
		nvgRoundedRect(
			args.vg,
			ledCoreX,
			ledCoreY,
			ledCoreWidth,
			ledCoreHeight,
			ledCoreCorner
		);
		nvgFillColor(args.vg, nvgRGBA(255, 255, 255, 235));
		nvgFill(args.vg);

		nvgRestore(args.vg);
	}

	void draw(const DrawArgs& args) override {
		nvgSave(args.vg);

		const float cx = box.size.x * 0.5f;
		const float cy = box.size.y * 0.5f;
		const float innerRadius = outerRadius - ringThickness;

		// Anello esterno con gradiente lineare lungo la diagonale alto-sx -> basso-dx,
		// con asse traslato verso il lato chiaro.
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, outerRadius);
		nvgCircle(args.vg, cx, cy, innerRadius);
		nvgPathWinding(args.vg, NVG_HOLE);

		const float d = outerRadius * ringGradientRadiusFactor;
		const float shift = d * ringGradientShiftFactor;

		nvgFillPaint(args.vg, nvgLinearGradient(
			args.vg,
			cx - d - shift, cy - d - shift,
			cx + d - shift, cy + d - shift,
			nvgRGB(0xEE, 0xEE, 0xEE),
			nvgRGB(0x00, 0x00, 0x00)
		));
		nvgFill(args.vg);

		// Disco interno piatto, black matte.
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, innerRadius);
		nvgFillColor(args.vg, nvgRGB(0x00, 0x00, 0x00));
		nvgFill(args.vg);

		// LED grafico separato dalla light VCV
		drawLedBody(args, cx, cy);

		nvgRestore(args.vg);
	}
};

template <Accent TAccent>
struct ThemedKnob : Knob {
	ThemedKnob() : Knob() {
		accent = TAccent;

		if (valueLight) {
			valueLight->baseColors.clear();
			valueLight->addBaseColor(SubButton::accentColor(accent));
		}
	}
};

using RedKnob    = ThemedKnob<Accent::Red>;
using AmberKnob  = ThemedKnob<Accent::Amber>;
using GreenKnob  = ThemedKnob<Accent::Green>;
using BlueKnob   = ThemedKnob<Accent::Blue>;
using PurpleKnob = ThemedKnob<Accent::Purple>;
using CyanKnob   = ThemedKnob<Accent::Cyan>;
using PinkKnob   = ThemedKnob<Accent::Pink>;
using DefaultKnob = CyanKnob;

} // namespace SubKnob

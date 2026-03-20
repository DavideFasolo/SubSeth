#pragma once

#include "../plugin.hpp"
#include "SubTheme.hpp"

/*
 * SubSeth button component
 * - design based on latest approved concept
 * - black outer corona
 * - accent middle ring always visible, matte, even when OFF
 * - OFF state: neutral cap
 * - ON state: accent cap + halo
 * - shallow convex profile, almost flat
 * - usable as latch or momentary through app::Switch::momentary
 */

namespace SubButton {

enum class Accent { Red, Amber, Green, Blue, Purple, Cyan, Pink };

static inline float clampf(float x, float lo, float hi) {
	return x < lo ? lo : (x > hi ? hi : x);
}

static inline NVGcolor rgba8(int r, int g, int b, int a = 255) {
	return nvgRGBA((unsigned char) r, (unsigned char) g, (unsigned char) b, (unsigned char) a);
}

static inline NVGcolor withAlpha(NVGcolor c, float a) {
	c.a = clampf(a, 0.f, 1.f);
	return c;
}

static inline NVGcolor mix(NVGcolor a, NVGcolor b, float t) {
	t = clampf(t, 0.f, 1.f);
	NVGcolor out;
	out.r = a.r * (1.f - t) + b.r * t;
	out.g = a.g * (1.f - t) + b.g * t;
	out.b = a.b * (1.f - t) + b.b * t;
	out.a = a.a * (1.f - t) + b.a * t;
	return out;
}

static inline NVGcolor accentColor(Accent accent) {
	switch (accent) {
		case Accent::Red:    return nvgRGB(0xF3, 0x12, 0x00);
		case Accent::Amber:  return nvgRGB(0xFF, 0xB4, 0x37);
		case Accent::Green:  return nvgRGB(0x00, 0xB5, 0x6E);
		case Accent::Blue:   return nvgRGB(0x36, 0x95, 0xEF);
		case Accent::Purple: return nvgRGB(0x8B, 0x4A, 0xDE);
		case Accent::Cyan: return nvgRGB(0x00, 0xD8, 0xFF); case Accent::Pink:   return nvgRGB(0xFF, 0xAE, 0xC9);
	}
	return nvgRGB(0x8B, 0x4A, 0xDE);
}



static inline NVGcolor labelColor(Accent accent) {
	switch (accent) {
		case Accent::Red:    return nvgRGB(0x80, 0x80, 0x80);
		case Accent::Amber:  return nvgRGB(0xFF, 0xFF, 0xFF);
		case Accent::Green:  return nvgRGB(0xFF, 0x99, 0x41);
		case Accent::Blue:   return nvgRGB(0x80, 0x36, 0x10);
		case Accent::Purple: return nvgRGB(0x42, 0xFF, 0x01);
		case Accent::Cyan: return nvgRGB(0xFF, 0x7A, 0x00); case Accent::Pink:   return nvgRGB(0xFF, 0x37, 0x80);
	}
	return nvgRGB(0x42, 0xFF, 0x01);
}
static inline NVGcolor offCapLight() { return nvgRGB(0xDD, 0xDE, 0xE1); }
static inline NVGcolor offCapDark()  { return nvgRGB(0x92, 0x94, 0x98); }
static inline NVGcolor offCapEdge()  { return nvgRGB(0xB7, 0xB9, 0xBE); }

static inline NVGcolor coronaBlackOuter()  { return nvgRGB(0x07, 0x07, 0x08); }
static inline NVGcolor coronaBlackInner()  { return nvgRGB(0x13, 0x13, 0x15); }
static inline NVGcolor cavityBlack()       { return nvgRGB(0x09, 0x09, 0x0A); }
static inline NVGcolor cavityEdge()        { return nvgRGB(0x22, 0x22, 0x26); }

struct Button : app::Switch {
	Accent accent = Accent::Purple;

	// Visual tuning
	float outerRadius = 8.48f;
	float outerBlackThickness = 0.34f;
	float accentRingThickness = 0.30f;
	float cavityInset = 0.36f;
	float capInset = 0.58f;

	Button() {
		box.size = Vec(12.75f, 12.75f);
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
		return clampf((pq->getValue() - lo) / (hi - lo), 0.f, 1.f);
	}

	bool isOn() {
		return value01() > 0.5f;
	}

	void drawRing(const DrawArgs& args, float cx, float cy, float rOuter, float thickness, NVGcolor color) {
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, rOuter);
		nvgCircle(args.vg, cx, cy, rOuter - thickness);
		nvgPathWinding(args.vg, NVG_HOLE);
		nvgFillColor(args.vg, color);
		nvgFill(args.vg);
	}

	void drawCircleFill(const DrawArgs& args, float cx, float cy, float r, NVGcolor color) {
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, r);
		nvgFillColor(args.vg, color);
		nvgFill(args.vg);
	}

	void drawCircleStroke(const DrawArgs& args, float cx, float cy, float r, float width, NVGcolor color) {
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy, r);
		nvgStrokeWidth(args.vg, width);
		nvgStrokeColor(args.vg, color);
		nvgStroke(args.vg);
	}

	void draw(const DrawArgs& args) override {
		nvgSave(args.vg);

		const float s = box.size.x;
		const float cx = s * 0.5f;
		const float cy = s * 0.5f;

		const bool on = isOn();
		const float onf = on ? 1.f : 0.f;

		const NVGcolor accentBase = accentColor(accent);
		const NVGcolor accentMatte = mix(accentBase, coronaBlackOuter(), 0.22f);
		const NVGcolor accentBright = mix(accentBase, nvgRGB(0xFF, 0xFF, 0xFF), 0.22f);
		const NVGcolor accentDark = mix(accentBase, nvgRGB(0x00, 0x00, 0x00), 0.28f);

		const float shadowYOffset = 0.64f;
		const float capYOffset = 0.14f + onf * 0.17f;   // almost flat, minimal travel

		const float rOuter = outerRadius;
		const float rOuterBlackInner = rOuter - outerBlackThickness;
		const float rAccentOuter = rOuterBlackInner - 0.28f;
		const float rAccentInner = rAccentOuter - accentRingThickness;
		const float rCavity = rAccentInner - cavityInset;
		const float rCap = rCavity - capInset;

		// Outer shadow, very soft and shallow
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy + shadowYOffset, rOuter + 0.26f);
		nvgFillPaint(args.vg, nvgRadialGradient(
			args.vg,
			cx, cy + shadowYOffset,
			rOuter - 1.35f,
			rOuter + 1.95f,
			withAlpha(nvgRGB(0x00, 0x00, 0x00), 0.18f),
			withAlpha(nvgRGB(0x00, 0x00, 0x00), 0.f)
		));
		nvgFill(args.vg);

		// Halo only when ON
		if (on) {
			nvgBeginPath(args.vg);
			nvgCircle(args.vg, cx, cy, rOuter + 2.1f);
			nvgFillPaint(args.vg, nvgRadialGradient(
				args.vg,
				cx, cy,
				rOuter - 0.6f,
				rOuter + 4.9f,
				withAlpha(accentBase, 0.26f),
				withAlpha(accentBase, 0.f)
			));
			nvgFill(args.vg);
		}

		// Black outer corona
		drawRing(args, cx, cy, rOuter, outerBlackThickness, coronaBlackOuter());

		// Subtle glossy hint on black corona
		nvgBeginPath(args.vg);
		nvgArc(args.vg, cx, cy, rOuter - 0.22f, 3.8f, 5.45f, NVG_CW);
		nvgStrokeWidth(args.vg, 0.6f);
		nvgStrokeColor(args.vg, withAlpha(coronaBlackInner(), 0.95f));
		nvgStroke(args.vg);

		// Accent ring, matte, always visible also when OFF
		drawRing(args, cx, cy, rAccentOuter, accentRingThickness, accentMatte);

		// Slight accent edge definition
		drawCircleStroke(args, cx, cy, rAccentOuter - 0.06f, 0.19f, withAlpha(accentBright, 0.24f));
		drawCircleStroke(args, cx, cy, rAccentInner + 0.045f, 0.19f, withAlpha(accentDark, 0.34f));

		// Inner cavity
		drawCircleFill(args, cx, cy, rCavity, cavityBlack());
		drawCircleStroke(args, cx, cy, rCavity + 0.04f, 0.26f, withAlpha(cavityEdge(), 0.75f));

		// Cap shadow: tiny, just enough to separate it from cavity
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy + capYOffset + 0.22f, rCap + 0.22f);
		nvgFillPaint(args.vg, nvgRadialGradient(
			args.vg,
			cx, cy + capYOffset + 0.19f,
			rCap - 1.12f,
			rCap + 1.35f,
			withAlpha(nvgRGB(0x00, 0x00, 0x00), 0.12f),
			withAlpha(nvgRGB(0x00, 0x00, 0x00), 0.f)
		));
		nvgFill(args.vg);

		// Cap body
		NVGcolor capTop = on ? mix(accentBase, nvgRGB(0xFF, 0xFF, 0xFF), 0.32f) : offCapLight();
		NVGcolor capBottom = on ? mix(accentBase, nvgRGB(0x00, 0x00, 0x00), 0.33f) : offCapDark();

		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy + capYOffset, rCap);
		nvgFillPaint(args.vg, nvgLinearGradient(
			args.vg,
			cx - rCap * 0.55f, cy + capYOffset - rCap * 0.75f,
			cx + rCap * 0.60f, cy + capYOffset + rCap * 0.80f,
			capTop,
			capBottom
		));
		nvgFill(args.vg);

		// Very shallow spherical hint
		nvgBeginPath(args.vg);
		nvgCircle(args.vg, cx, cy + capYOffset, rCap);
		nvgFillPaint(args.vg, nvgRadialGradient(
			args.vg,
			cx - rCap * 0.30f,
			cy + capYOffset - rCap * 0.34f,
			0.f,
			rCap * 1.05f,
			withAlpha(nvgRGB(0xFF, 0xFF, 0xFF), on ? 0.18f : 0.12f),
			withAlpha(nvgRGB(0xFF, 0xFF, 0xFF), 0.f)
		));
		nvgFill(args.vg);


		// Small top sheen, flatter than a full dome
		nvgBeginPath(args.vg);
		nvgEllipse(args.vg, cx - rCap * 0.18f, cy + capYOffset - rCap * 0.28f, rCap * 0.42f, rCap * 0.20f);
		nvgFillPaint(args.vg, nvgLinearGradient(
			args.vg,
			cx - rCap * 0.50f, cy + capYOffset - rCap * 0.45f,
			cx + rCap * 0.15f, cy + capYOffset - rCap * 0.05f,
			withAlpha(nvgRGB(0xFF, 0xFF, 0xFF), on ? 0.17f : 0.11f),
			withAlpha(nvgRGB(0xFF, 0xFF, 0xFF), 0.f)
		));
		nvgFill(args.vg);

		// Cap outline
		drawCircleStroke(args, cx, cy + capYOffset, rCap, 0.68f, on ? withAlpha(accentBright, 0.55f) : withAlpha(offCapEdge(), 0.95f));

		nvgRestore(args.vg);
	}
};

template <Accent TAccent, bool TMomentary = false>
struct ThemedButton : Button {
	ThemedButton() : Button() {
		accent = TAccent;
		momentary = TMomentary;
	}
};


template <Accent TAccent>
struct AccentModuleLight : app::ModuleLightWidget {
	AccentModuleLight() {
		// Luce volutamente più grande del bottone, così si percepisce come halo reale di Rack
		box.size = Vec(13.f, 13.f);
		addBaseColor(accentColor(TAccent));
	}
};

using RedLight    = AccentModuleLight<Accent::Red>;
using AmberLight  = AccentModuleLight<Accent::Amber>;
using GreenLight  = AccentModuleLight<Accent::Green>;
using BlueLight   = AccentModuleLight<Accent::Blue>;
using PurpleLight = AccentModuleLight<Accent::Purple>;
using CyanLight = AccentModuleLight<Accent::Cyan>; using PinkLight   = AccentModuleLight<Accent::Pink>;

template <typename TButton, typename TLight>
static inline TButton* addLightedParamCentered(
	app::ModuleWidget* moduleWidget,
	Vec pos,
	engine::Module* module,
	int paramId,
	int lightId
) {
	// Prima la luce, poi il bottone davanti
	auto* light = createLightCentered<TLight>(pos, module, lightId);
	moduleWidget->addChild(light);

	TButton* button = createParamCentered<TButton>(pos, module, paramId);
	moduleWidget->addParam(button);
	return button;
}

// Latch variants
using RedLatch    = ThemedButton<Accent::Red, false>;
using AmberLatch  = ThemedButton<Accent::Amber, false>;
using GreenLatch  = ThemedButton<Accent::Green, false>;
using BlueLatch   = ThemedButton<Accent::Blue, false>;
using PurpleLatch = ThemedButton<Accent::Purple, false>;
using CyanLatch = ThemedButton<Accent::Cyan, false>; using PinkLatch   = ThemedButton<Accent::Pink, false>;

// Momentary variants
using RedMomentary    = ThemedButton<Accent::Red, true>;
using AmberMomentary  = ThemedButton<Accent::Amber, true>;
using GreenMomentary  = ThemedButton<Accent::Green, true>;
using BlueMomentary   = ThemedButton<Accent::Blue, true>;
using PurpleMomentary = ThemedButton<Accent::Purple, true>;
using CyanMomentary = ThemedButton<Accent::Cyan, true>; using PinkMomentary   = ThemedButton<Accent::Pink, true>;

} // namespace SubButton

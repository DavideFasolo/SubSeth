#pragma once

#include "../plugin.hpp"
#include "SubDraw.hpp"
#include "SubFonts.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace SubLabel {

static constexpr float WIDTH = 104.f;
static constexpr float HEIGHT = 26.f;
static constexpr float CORNER_RADIUS = 5.f;
static constexpr float DEFAULT_FONT_SIZE = 18.f;
static constexpr float H_PADDING = 7.f;

static inline NVGcolor defaultTextColor() {
	// TD-410-like cyan
	return nvgRGB(0x27, 0xD1, 0xFF);
}

static inline NVGcolor backgroundColor() {
	return nvgRGB(0x00, 0x00, 0x00);
}

static inline int colorByte(float c) {
	int v = (int) std::lround(c * 255.f);
	return std::max(0, std::min(255, v));
}

static inline bool sameColor(NVGcolor a, NVGcolor b) {
	return colorByte(a.r) == colorByte(b.r)
		&& colorByte(a.g) == colorByte(b.g)
		&& colorByte(a.b) == colorByte(b.b)
		&& colorByte(a.a) == colorByte(b.a);
}

struct State {
	std::string text;
	NVGcolor color = defaultTextColor();

	json_t* toJson() const {
		json_t* rootJ = json_object();
		json_object_set_new(rootJ, "text", json_string(text.c_str()));
		json_object_set_new(rootJ, "r", json_integer(colorByte(color.r)));
		json_object_set_new(rootJ, "g", json_integer(colorByte(color.g)));
		json_object_set_new(rootJ, "b", json_integer(colorByte(color.b)));
		json_object_set_new(rootJ, "a", json_integer(colorByte(color.a)));
		return rootJ;
	}

	void fromJson(json_t* rootJ) {
		if (!rootJ) {
			return;
		}

		json_t* textJ = json_object_get(rootJ, "text");
		if (json_is_string(textJ)) {
			text = json_string_value(textJ);
		}

		int r = colorByte(color.r);
		int g = colorByte(color.g);
		int b = colorByte(color.b);
		int a = colorByte(color.a);

		json_t* rJ = json_object_get(rootJ, "r");
		json_t* gJ = json_object_get(rootJ, "g");
		json_t* bJ = json_object_get(rootJ, "b");
		json_t* aJ = json_object_get(rootJ, "a");

		if (json_is_integer(rJ)) r = (int) json_integer_value(rJ);
		if (json_is_integer(gJ)) g = (int) json_integer_value(gJ);
		if (json_is_integer(bJ)) b = (int) json_integer_value(bJ);
		if (json_is_integer(aJ)) a = (int) json_integer_value(aJ);

		color = nvgRGBA(
			(uint8_t) std::max(0, std::min(255, r)),
			(uint8_t) std::max(0, std::min(255, g)),
			(uint8_t) std::max(0, std::min(255, b)),
			(uint8_t) std::max(0, std::min(255, a))
		);
	}
};

struct Config {
	SubDraw::Justify justify = SubDraw::Justify::Right;
	float fontSize = DEFAULT_FONT_SIZE;
};

struct Field : ui::TextField {
	State* state = nullptr;
	Config config;
	std::string fontPath;
	NVGcolor color = defaultTextColor();

	Field(State* state_, Config config_ = {})
		: state(state_), config(config_) {
		box.size = Vec(WIDTH, HEIGHT);
		multiline = false;
		placeholder = "";
		text = state ? state->text : std::string();
		color = state ? state->color : defaultTextColor();
		fontPath = asset::plugin(
			pluginInstance,
			"res/fonts/IBMPlexMono/IBMPlexMono-Regular.ttf"
		);
	}

	bool isFocused() const {
		return APP && APP->event && APP->event->selectedWidget == this;
	}

	void step() override {
		ui::TextField::step();

		if (!state) {
			return;
		}

		if (!isFocused() && text != state->text) {
			setText(state->text);
		}

		color = state->color;
	}

	float innerLeft() const {
		return H_PADDING;
	}

	float innerRight() const {
		return box.size.x - H_PADDING;
	}

	float innerWidth() const {
		return innerRight() - innerLeft();
	}

	std::shared_ptr<Font> getFont() const {
		return APP->window->loadFont(fontPath);
	}

	std::string displayText() const {
		if (password) {
			return std::string(text.size(), '*');
		}
		return text;
	}

	void setupFont(NVGcontext* vg) const {
		auto font = getFont();
		if (!font) {
			return;
		}

		nvgFontFaceId(vg, font->handle);
		nvgFontSize(vg, config.fontSize);
		nvgTextLetterSpacing(vg, 0.f);
		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	}

	float measureTextWidth(NVGcontext* vg, const std::string& s) const {
		if (s.empty()) {
			return 0.f;
		}

		float bounds[4] = {};
		nvgTextBounds(vg, 0.f, 0.f, s.c_str(), nullptr, bounds);
		return bounds[2] - bounds[0];
	}

	float computeTextX(NVGcontext* vg, const std::string& s) const {
		float w = measureTextWidth(vg, s);

		switch (config.justify) {
			case SubDraw::Justify::Left:
				return innerLeft();
			case SubDraw::Justify::Center:
				return innerLeft() + (innerWidth() - w) * 0.5f;
			case SubDraw::Justify::Right:
			default:
				return innerRight() - w;
		}
	}

	std::vector<NVGglyphPosition> glyphPositions(
		NVGcontext* vg,
		const std::string& s,
		float x,
		float y
	) const {
		std::vector<NVGglyphPosition> glyphs;
		if (s.empty()) {
			return glyphs;
		}

		glyphs.resize(s.size());
		int count = nvgTextGlyphPositions(
			vg,
			x,
			y,
			s.c_str(),
			nullptr,
			glyphs.data(),
			(int) glyphs.size()
		);
		glyphs.resize(std::max(0, count));
		return glyphs;
	}

	float cursorXForIndex(
		const std::string& s,
		const std::vector<NVGglyphPosition>& glyphs,
		float textX,
		float textWidth,
		int index
	) const {
		if (index <= 0 || glyphs.empty()) {
			return textX;
		}

		for (const auto& g : glyphs) {
			int bytePos = (int) (g.str - s.c_str());
			if (bytePos >= index) {
				return g.x;
			}
		}

		return textX + textWidth;
	}

	void setCurrentColor(NVGcolor newColor) {
		color = newColor;
		if (state) {
			state->color = newColor;
		}
	}

	void onButton(const ButtonEvent& e) override {
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
			createContextMenu();
			e.consume(this);
			return;
		}

		ui::TextField::onButton(e);
	}

	void onChange(const ChangeEvent& e) override {
		if (state) {
			state->text = text;
		}
		Widget::onChange(e);
	}

	void draw(const DrawArgs& args) override {
		auto font = getFont();
		if (!font) {
			return;
		}

		const std::string s = displayText();
		const float y = box.size.y * 0.5f;

		nvgSave(args.vg);
		nvgScissor(args.vg, 0.f, 0.f, box.size.x, box.size.y);

		setupFont(args.vg);
		const float textWidth = measureTextWidth(args.vg, s);
		const float textX = computeTextX(args.vg, s);
		const auto glyphs = glyphPositions(args.vg, s, textX, y);

		if (isFocused() && selection != cursor && !s.empty()) {
			int begin = std::min(cursor, selection);
			int end = std::max(cursor, selection);
			float x0 = cursorXForIndex(s, glyphs, textX, textWidth, begin);
			float x1 = cursorXForIndex(s, glyphs, textX, textWidth, end);

			nvgBeginPath(args.vg);
			nvgRect(args.vg, x0, 4.f, x1 - x0, box.size.y - 8.f);
			NVGcolor hi = color;
			hi.a = 0.30f;
			nvgFillColor(args.vg, hi);
			nvgFill(args.vg);
		}

		nvgFillColor(args.vg, color);
		nvgText(args.vg, textX, y, s.c_str(), nullptr);

		if (isFocused()) {
			float cx = cursorXForIndex(s, glyphs, textX, textWidth, cursor);
			nvgBeginPath(args.vg);
			nvgRect(args.vg, cx, 4.f, 1.f, box.size.y - 8.f);
			nvgFillColor(args.vg, color);
			nvgFill(args.vg);
		}

		nvgResetScissor(args.vg);
		nvgRestore(args.vg);
	}

	int getTextPosition(math::Vec mousePos) override {
		auto font = getFont();
		if (!font) {
			return 0;
		}

		const std::string s = displayText();
		if (s.empty()) {
			return 0;
		}

		setupFont(font->vg);
		const float y = box.size.y * 0.5f;
		const float textX = computeTextX(font->vg, s);
		const auto glyphs = glyphPositions(font->vg, s, textX, y);

		if (glyphs.empty() || mousePos.x <= textX) {
			return 0;
		}

		for (const auto& g : glyphs) {
			float mid = (g.minx + g.maxx) * 0.5f;
			if (mousePos.x < mid) {
				return (int) (g.str - s.c_str());
			}
		}

		return (int) s.size();
	}

	struct ColorItem : ui::ColorDotMenuItem {
		Field* field = nullptr;
		NVGcolor choice = color::BLACK_TRANSPARENT;

		void onAction(const event::Action& e) override {
			if (field) {
				field->setCurrentColor(choice);
			}
			ui::MenuItem::onAction(e);
		}

		void step() override {
			ui::ColorDotMenuItem::step();
			rightText = (field && sameColor(field->color, choice)) ? "✔" : "";
		}
	};

	void addColorItem(ui::Menu* menu, const char* name, NVGcolor c) {
		auto* item = new ColorItem();
		item->text = name;
		item->field = this;
		item->choice = c;
		item->color = c;
		menu->addChild(item);
	}

	void createContextMenu() {
		ui::Menu* menu = createMenu();

		auto* label = new ui::MenuLabel();
		label->text = "Label color";
		menu->addChild(label);

		addColorItem(menu, "Cyan", nvgRGB(0x27, 0xD1, 0xFF));
		addColorItem(menu, "White", nvgRGB(0xFF, 0xFF, 0xFF));
		addColorItem(menu, "Green", nvgRGB(0x5B, 0xFF, 0x7A));
		addColorItem(menu, "Yellow", nvgRGB(0xFF, 0xE1, 0x4D));
		addColorItem(menu, "Orange", nvgRGB(0xFF, 0xA1, 0x2D));
		addColorItem(menu, "Red", nvgRGB(0xFF, 0x5A, 0x5A));
		addColorItem(menu, "Magenta", nvgRGB(0xFF, 0x5E, 0xF1));
		addColorItem(menu, "Blue", nvgRGB(0x66, 0xA8, 0xFF));
	}
};

struct Display : Widget {
	State* state = nullptr;
	Config config;
	Field* field = nullptr;

	Display(State* state_, Config config_ = {})
		: state(state_), config(config_) {
		box.size = Vec(WIDTH, HEIGHT);
		field = new Field(state, config);
		field->box.pos = Vec(0.f, 0.f);
		field->box.size = box.size;
		addChild(field);
	}

	void draw(const DrawArgs& args) override {
		nvgBeginPath(args.vg);
		nvgRoundedRect(args.vg, 0.f, 0.f, box.size.x, box.size.y, CORNER_RADIUS);
		nvgFillColor(args.vg, backgroundColor());
		nvgFill(args.vg);
		Widget::draw(args);
	}
};

static inline Display* createLabelDisplay(
	Vec pos,
	State* state,
	Config config = {}
) {
	auto* w = new Display(state, config);
	w->box.pos = pos;
	return w;
}

static inline Display* createLabelDisplay(
	float x,
	float y,
	State* state,
	Config config = {}
) {
	return createLabelDisplay(Vec(x, y), state, config);
}

static inline Display* createLabelDisplay(
	Vec pos,
	State* state,
	SubDraw::Justify justify
) {
	Config config;
	config.justify = justify;
	return createLabelDisplay(pos, state, config);
}

static inline Display* createLabelDisplay(
	float x,
	float y,
	State* state,
	SubDraw::Justify justify
) {
	return createLabelDisplay(Vec(x, y), state, justify);
}

} // namespace SubLabel

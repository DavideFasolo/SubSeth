/**************************************************************
*
* Portions of this file are derived from SubmarineFree
* Copyright © 2018 David O'Rourke
* Source: https://github.com/david-c14/SubmarineFree
*
* Based on src/shared/Port.cpp from SubmarineFree.
* Flat-style support intentionally omitted for SubSeth.
*
**************************************************************/

#include "SubPorts.hpp"

void SilverPort::draw(const DrawArgs& args) {
	nvgSave(args.vg);
	const float radius = box.size.x / 2.0f;

	// Shadow
	nvgBeginPath(args.vg);
	nvgCircle(args.vg, radius, radius * 1.2f, radius);
	nvgFillColor(args.vg, nvgRGBAf(0.f, 0.f, 0.f, 0.15f));
	nvgFill(args.vg);

	// Socket opening
	nvgBeginPath(args.vg);
	nvgRect(args.vg, 6.f, 6.f, 13.f, 13.f);
	nvgFillColor(args.vg, nvgRGB(0x00, 0x00, 0x00));
	nvgFill(args.vg);

	nvgBeginPath(args.vg);
	nvgRect(args.vg, 10.f, 6.f, 5.f, 13.f);
	nvgFillPaint(args.vg,
		nvgLinearGradient(args.vg, radius, 19.f, radius, radius,
			nvgRGB(0x60, 0x60, 0x60), nvgRGB(0x00, 0x00, 0x00)));
	nvgFill(args.vg);

	nvgLineJoin(args.vg, NVG_BEVEL);

	// Port body
	nvgBeginPath(args.vg);
	nvgCircle(args.vg, radius, radius, 7.f);
	nvgStrokeWidth(args.vg, 4.f);
	nvgStrokePaint(args.vg,
		nvgRadialGradient(args.vg, radius + 0.3f, radius + 1.f, 0.f, 9.f,
			nvgRGB(0x20, 0x20, 0x20), col));
	nvgStroke(args.vg);

	// Outer rim
	nvgBeginPath(args.vg);
	nvgMoveTo(args.vg, 0.665568f, 9.993499f);
	nvgBezierTo(args.vg, 1.849660f, 4.402603f, 6.784906f, 0.403117f, 12.499817f, 0.403117f);
	nvgBezierTo(args.vg, 18.214727f, 0.403117f, 23.149975f, 4.402601f, 24.334068f, 9.993498f);
	nvgBezierTo(args.vg, 23.438292f, 9.993016f, 22.610352f, 10.470624f, 22.162313f, 11.246303f);
	nvgBezierTo(args.vg, 21.714275f, 12.021981f, 21.714256f, 12.977802f, 22.162262f, 13.753500f);
	nvgBezierTo(args.vg, 22.610268f, 14.529198f, 23.438189f, 15.006838f, 24.333965f, 15.006392f);
	nvgBezierTo(args.vg, 23.149975f, 20.597290f, 18.214729f, 24.596775f, 12.499819f, 24.596775f);
	nvgBezierTo(args.vg, 6.784908f, 24.596775f, 1.849660f, 20.597288f, 0.665567f, 15.006392f);
	nvgBezierTo(args.vg, 2.049333f, 15.005648f, 3.170693f, 13.883659f, 3.170665f, 12.499894f);
	nvgBezierTo(args.vg, 3.170636f, 11.116129f, 2.049230f, 9.994187f, 0.665464f, 9.993499f);
	nvgBezierTo(args.vg, 0.665499f, 9.993499f, 0.665533f, 9.993499f, 0.665568f, 9.993499f);
	
	nvgPathWinding(args.vg, NVG_SOLID);
	nvgClosePath(args.vg);
	nvgCircle(args.vg, radius, radius, 8.0f);
	nvgPathWinding(args.vg, NVG_HOLE);
	nvgStrokeWidth(args.vg, 0.80645f);

	if (type == engine::Port::OUTPUT) {
		nvgFillPaint(args.vg,
			nvgRadialGradient(args.vg, radius + 0.3f, radius + 1.f, 1.f, 12.f,
				col, nvgRGB(0x3f, 0x3f, 0x3f)));
		nvgStrokeColor(args.vg, nvgRGB(0x36, 0x36, 0x36));
	}
	else {
		nvgFillPaint(args.vg,
			nvgRadialGradient(args.vg, radius + 0.3f, radius + 1.f, 1.f, 12.f,
				col, nvgRGB(0xff, 0xff, 0xff)));
		nvgStrokeColor(args.vg, nvgRGB(0x66, 0x66, 0x66));
	}

	nvgFill(args.vg);
	nvgStroke(args.vg);
	nvgRestore(args.vg);
}

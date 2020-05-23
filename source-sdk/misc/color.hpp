#pragma once
#include <cstdint>
#include <d3d9.h>

struct color {
	int a, r, g, b;
	color() = default;
	color(int r, int g, int b, int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	color(uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}
	color from_uint(uint32_t uint) {
		return color(uint);
	}
	D3DCOLOR from_color(color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}

	void random_color(int iTick)
	{
		this->r = sin(3.f * iTick + 0.f) * 127 + 128;
		this->g = sin(3.f * iTick + ((2.f * M_PI) / 3)) * 127 + 128;
		this->b = sin(3.f * iTick + ((4.f * M_PI) / 3)) * 127 + 128;
		this->a = 255;
		// decrease 0.3f if you want it smoother
	}

	static color black(int a = 255) { return { 0, 0, 0, a }; }
	static color white(int a = 255) { return { 255, 255, 255, a }; }
	static color red(int   a = 255) { return { 255, 0, 0, a }; }
	static color green(int a = 255) { return { 0, 255, 0, a }; }
	static color blue(int  a = 255) { return { 0, 0, 255, a }; }


};



#pragma once
#include "../../../dependencies/common_includes.hpp"
#include <vector>
struct paint_kit {
	int id;
	std::string name;
	bool operator < (const paint_kit& other) {
		return name < other.name;
	}
};

class c_kit_parser {
public:
	void setup() noexcept;
};

extern std::vector<paint_kit> parser_skins;
extern std::vector<paint_kit> parser_gloves;
extern c_kit_parser kit_parser;
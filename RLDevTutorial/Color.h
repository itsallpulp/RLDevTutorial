#pragma once
#pragma once

#include "Types.h"


typedef struct Color {
	int r, g, b;
};


extern std::map<char, Color> colors;

static void LoadColors(std::string filepath)
{
	json::value data;
	std::ifstream inf(filepath);
	std::string s = "";
	json::stream_parser p;
	json::error_code ec;

	while (std::getline(inf, s))
	{
		p.write(s, ec);
		if (ec)
		{
			return;
		}
	}

	data = p.release();

	json::object d = data.as_object();

	for (auto it = d.begin(); it != d.end(); ++it)
	{
		colors[it->key().at(0)] = { json::value_to<int>(it->value().as_array().at(0)), json::value_to<int>(it->value().as_array().at(1)), json::value_to<int>(it->value().as_array().at(2)) };
	}

}

static Color Darker(Color c)
{
	return { std::max(c.r - 35, 0), std::max(c.g - 35, 0), std::max(c.b - 35, 0) };
}
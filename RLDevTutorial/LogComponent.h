#pragma once
#include "Component.h"
class LogComponent : public Component {
	public:
	LogComponent();
	std::vector<std::string> logs;
};
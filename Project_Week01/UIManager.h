#pragma once
#include "UIFrame.h"
#include <Memory>

class UIManager
{
private:
	std::vector<std::unique_ptr<UIFrame>> UIFrameList;
public:
	UIManager();
	~UIManager();

	UIFrame& CreateFrame(const std::string& title);

	void Render();
};

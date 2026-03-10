#pragma once
#include "UIFrame.h"

class UIManager
{
private:
	std::vector<UIFrame> UIFrameList;
public:
	UIManager();
	~UIManager();

	void AddFrame(const UIFrame& frame);

	void Render();
};

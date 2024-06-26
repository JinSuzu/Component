#pragma once
#include "../Component.h"

class Cp_Counter
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;

private:
	int m_max = 0;

	std::list<std::weak_ptr<class Cp_SquarePolygon>>m_drawList;
};

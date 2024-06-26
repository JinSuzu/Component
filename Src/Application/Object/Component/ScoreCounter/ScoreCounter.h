#pragma once
#include "../Component.h"

class Cp_ScoreCounter
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;

private:

	std::list<std::weak_ptr<class Cp_SquarePolygon>>m_drawList;
};

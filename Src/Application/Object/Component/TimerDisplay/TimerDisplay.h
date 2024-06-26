#pragma once
#include "../Component.h"

class Cp_TimerDisplay
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	std::list<std::weak_ptr<class Cp_SquarePolygon>>m_drawList;
	std::string m_sceneTag = "Game";
};

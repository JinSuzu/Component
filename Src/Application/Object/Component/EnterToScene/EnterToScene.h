#pragma once
#include "../Component.h"

class Cp_EnterToScene
	:public Component
{
public:
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

private:
	std::string m_nextScene;
};

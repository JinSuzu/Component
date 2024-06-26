#pragma once
#include "../Component.h"

class Cp_SceneChanger
	:public Component
{
public:
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	std::string m_nextSceneName;
	std::string m_sePath;
};

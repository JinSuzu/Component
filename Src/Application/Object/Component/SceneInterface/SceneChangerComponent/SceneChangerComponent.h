#pragma once

class SceneChangerComponent
	:public Component
{
public:
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

private:
	std::string m_nextSceneName;
	std::string m_sePath;
};

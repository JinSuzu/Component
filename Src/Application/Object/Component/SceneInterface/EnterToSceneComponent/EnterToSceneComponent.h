#pragma once

class EnterToSceneComponent
	:public Component
{
public:
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

private:
	std::string m_nextScene;
};

#pragma once
#include "../Component.h"

class Cp_SceneActive
	:public Component
{
public:
	~Cp_SceneActive();

	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

private:
	std::string m_sceneName;

	std::string m_sceneBGMPath;
	std::shared_ptr<KdSoundInstance>m_sceneBGM;

	bool m_flg = false;
};

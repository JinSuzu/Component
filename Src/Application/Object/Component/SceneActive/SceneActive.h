﻿#pragma once
#include "../Component.h"

class Cp_SceneActive
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	std::string m_sceneName;

	std::string m_sceneBGMPath;

	bool m_flg = false;
};

#pragma once
#include "../Component.h"

class Cp_Camera
	:public Component 
{
public:
	void Start()override; 
	void ImGuiUpdate()override;

	void PostUpdateContents(); 

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;
private:
	std::shared_ptr<std::function<void()>>		m_preDraw;
	std::shared_ptr<CameraManager::CameraPack>	m_cameraPack;
};
#pragma once

class CameraComponent
	:public Component 
{
public:
	void Awake()override; 
	void Start()override; 
	void ImGuiUpdate()override;

	void PostUpdateContents(); 

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	std::shared_ptr<CameraManager::CameraPack> m_cameraPack;
};
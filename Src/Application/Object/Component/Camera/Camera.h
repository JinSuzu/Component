#pragma once
#include "../Component.h"

class Cp_Camera
	:public Component 
{
public:
	void Start()override; 
	void ImGuiUpdate()override;

	void PreDraw(); 
	void PostUpdateContents(); 

	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;
private:
	std::unique_ptr<KdCamera> m_camera;

	int m_priority = 0;
};
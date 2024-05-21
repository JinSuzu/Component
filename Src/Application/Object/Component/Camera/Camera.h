#pragma once
#include "../Component.h"

class Cp_Camera
	:public Component 
{
public:
	void Start()override; 
	void PreDraw(); 
	//void InitJson()override;
	//nlohmann::json GetJson()override;
private:
	std::unique_ptr<KdCamera> m_camera;
};
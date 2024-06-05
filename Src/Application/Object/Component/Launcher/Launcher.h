#pragma once
#include "../Component.h"

class Cp_Launcher
	:public Component 
{
public:
	void Start()override;
	void PreUpdateContents()override;

	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;
private:
	std::string m_bulletPath = "RPG-7_Bullet";
	std::shared_ptr<GameObject> m_bullet;

	int m_capacity = 1;
};
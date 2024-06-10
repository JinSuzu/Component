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
	enum State
	{
		Loaded,
		UnLoad,
	};
	State		m_state			  = UnLoad;
	std::string m_loadedModelPath = "RPG-7";
	std::string m_unLoadModelPath = "RPG-7_Launcher";
	std::shared_ptr<KdModelData> m_launcherModel[2];
	std::weak_ptr<class Cp_ModelData> m_draw;
	
	std::string m_bulletPath = "RPG-7_Bullet";
	std::shared_ptr<GameObject> m_bullet;

	int m_capacity = 1;
};
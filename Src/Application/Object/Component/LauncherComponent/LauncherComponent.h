#pragma once


class LauncherComponent
	:public Component 
{
public:
	void Start()override;
	void PreUpdateContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;
private:
	enum State
	{
		Loaded,
		UnLoad,
	};
	State		m_state			  = UnLoad;
	std::string m_loadedModelPath = "RPG-7";
	std::string m_unLoadModelPath = "RPG-7_Launcher";
	std::shared_ptr<class KdModelData> m_launcherModel[2];
	std::weak_ptr<class ModelDataComponent> m_draw;
	
	std::shared_ptr<class CreateObject> m_bullet;

	//弾数
	int m_bulletNum = 1;
	int m_bulletMax = 1;
	
	//発射間隔係
	bool m_bShotStandby    = false;
	int  m_shotInterval    = 3;
	int  m_shotIntervalCnt = 0;

	//リロード係
	bool m_bReloading    = false;
	int  m_reloadTime    = 10;
	int  m_reloadTimeCnt = 0;
};
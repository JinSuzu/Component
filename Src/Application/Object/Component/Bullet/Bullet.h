#pragma once
#include "../Component.h"

class Cp_Bullet
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

	void Destroy()override;
private:
	void Shot();
	//初期位置
	Math::Vector3 m_startPos;

	//加速時間
	int m_accelerationTime = 60;
	int m_accelerationTimeCnt = 0;

	//射程
	float m_shotRange = 10;
	Math::Vector3 m_startPoint;

	//着弾処理
	std::string m_landingPath = "";
	std::shared_ptr<GameObject> m_landingObject;

	float m_speedPow = 0.1f;
	std::weak_ptr<class Cp_Rigidbody>m_rigid;
	std::weak_ptr<class Cp_Transform>m_trans;
};

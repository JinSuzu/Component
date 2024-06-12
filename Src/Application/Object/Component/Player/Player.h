#pragma once
#include "../Component.h"

class Cp_Player
	:public Component
{
public:
	void Start()override;

	void PreUpdateContents()override;
	void PostUpdateContents()override;

	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	std::weak_ptr<class Cp_Rigidbody>m_rigid;

	bool  m_jumpFlg = false;
	float m_jumpPow = 1.0f;
	
	//加速時間
	int m_accelerationTime = 60;
	int m_accelerationTimeCnt = 0;
};

#pragma once
#include "../Component.h"

class Cp_MoveLimit
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

private:
	//射程
	float m_shotRange = 10;
	Math::Vector3 m_startPoint;
	std::weak_ptr<class Cp_Transform> m_trans;
};

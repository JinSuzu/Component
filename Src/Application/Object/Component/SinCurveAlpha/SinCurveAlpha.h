#pragma once
#include "../Component.h"

class Cp_SinCurveAlpha
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override; 
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override; 
	nlohmann::json GetJson()override;

private:
	std::weak_ptr<class Cp_SquarePolygon>m_wpDraw;
	float m_angle		= 0;

	float m_addPow		= 1;
	float m_addAngle	= 0.0f;
};
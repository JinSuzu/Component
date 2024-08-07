#pragma once


class SinCurveAlphaComponent
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override; 
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override; 
	nlohmann::json Serialize()override;

private:
	std::weak_ptr<class SquarePolygonComponent>m_wpDraw;
	float m_angle		= 0;

	float m_addPow		= 1;
	float m_addAngle	= 0.0f;
};
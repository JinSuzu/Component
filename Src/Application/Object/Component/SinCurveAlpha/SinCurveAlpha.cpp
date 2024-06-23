#include "SinCurveAlpha.h"
#include "../../Game/GameObject.h"
#include "../SquarePolygon/SquarePolygon.h"

void Cp_SinCurveAlpha::Start()
{
	m_wpDraw = m_owner.lock()->GetComponent<Cp_SquarePolygon>();
}

void Cp_SinCurveAlpha::PreUpdateContents()
{
	if (m_wpDraw.expired())return;

	m_angle += m_addPow;
	m_angle -= 360 * (m_angle >= 360);
	m_angle += 360 * (m_angle < 0);

	float alpha = abs(sin(DirectX::XMConvertToRadians(m_angle)));
	alpha += m_addAngle;
	if (alpha > 1)m_angle += 90;

	Math::Color color = m_wpDraw.lock()->GetColor();
	color.w = alpha;
	m_wpDraw.lock()->SetColor(color);
}

void Cp_SinCurveAlpha::ImGuiUpdate()
{
	ImGui::DragFloat("addPow", &m_addPow);
	ImGui::DragFloat("addAngle", &m_addAngle);
}

void Cp_SinCurveAlpha::InitJson()
{
	m_addAngle = m_jsonData["addAngle"];
	m_addPow = m_jsonData["addPow"];
}

nlohmann::json Cp_SinCurveAlpha::GetJson()
{
	m_jsonData["addAngle"] = m_addAngle;
	m_jsonData["addPow"] = m_addPow;
	return m_jsonData;
}

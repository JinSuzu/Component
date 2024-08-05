#include "SinCurveAlphaComponent.h"
#include "../../../Game/GameObject.h"

void SinCurveAlphaComponent::Start()
{
	m_wpDraw = m_owner.lock()->GetComponent<SquarePolygonComponent>();
}

void SinCurveAlphaComponent::PreUpdateContents()
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

void SinCurveAlphaComponent::ImGuiUpdate()
{
	ImGui::DragFloat("addPow", &m_addPow);
	ImGui::DragFloat("addAngle", &m_addAngle);
}

void SinCurveAlphaComponent::LoadJson(nlohmann::json _json)
{
	m_addAngle = _json["addAngle"];
	m_addPow   = _json["addPow"];
}

nlohmann::json SinCurveAlphaComponent::GetJson()
{
	nlohmann::json json;
	json["addAngle"] = m_addAngle;
	json["addPow"] = m_addPow;
	return json;
}

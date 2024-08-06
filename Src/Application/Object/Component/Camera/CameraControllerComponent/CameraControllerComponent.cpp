#include "CameraControllerComponent.h"
#include "../../../Game/GameObject.h"

#include "../../../../main.h"

void CameraControllerComponent::PreUpdateContents()
{
	const Math::Vector2& EffectShaft
		= Application::Instance().GetMouseDelta();

	Math::Vector3 rota = m_trans.lock()->GetLocalRotation();

	if (m_effectShaft & (UINT)Shaft::X) 
	{
		rota.y += EffectShaft.x;
	}
	
	if (m_effectShaft & (UINT)Shaft::Y) 
	{
		rota.x -= EffectShaft.y;
	}

	m_trans.lock()->SetRotation(rota);
}

void CameraControllerComponent::ImGuiUpdate()
{
	if (ImGui::Button("EffectShaft"))ImGui::OpenPopup("EffectShaft");
	if (ImGui::BeginPopup("EffectShaft"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<Shaft>(m_effectShaft);
		ImGui::EndPopup();
	}
}

nlohmann::json CameraControllerComponent::GetJson()
{
	nlohmann::json json;
	json["EffectShaft"] = m_effectShaft;
	return json;
}

void CameraControllerComponent::LoadJson(nlohmann::json _json)
{
	m_effectShaft = _json["EffectShaft"];
}

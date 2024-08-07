#include "AddRotaionComponent.h"
#include "../../../Game/GameObject.h"

#include "../../../../main.h"

void AddRotationComponent::Start()
{

}


void AddRotationComponent::UpdateContents()
{
	auto trans = m_owner.lock()->GetTransform();
	Math::Vector3 rota = trans.lock()->GetLocalRotation();

	if (m_addType == AddType::Normal)rota += m_addPow;

	if (m_addType == AddType::FollowMouse)
	{

		Math::Vector2 nowMouseMove = Application::Instance().GetMouse() - m_mouseMove;
	
		POINT test{ 640,360 };
		ClientToScreen(Application::Instance().GetWindowHandle(), &test);
		if (!KernelEngine::is_Debugging())SetCursorPos(test.x, test.y);
		m_mouseMove = Application::Instance().GetMouse();


		if (KernelEngine::is_Debugging() && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))return;
		nowMouseMove = nowMouseMove * m_addPow;
		rota += {-nowMouseMove.y, nowMouseMove.x, 0};
	}

	trans.lock()->SetLocalRotation(rota);
}

void AddRotationComponent::ImGuiUpdate()
{
	ImGui::DragFloat3("addPow", &m_addPow.x);

	bool bNormal = m_addType & AddType::Normal;
	if (ImGui::Checkbox("Normal", &bNormal))m_addType = AddType::Normal; ImGui::SameLine();
	bool bFollowMouse = m_addType & AddType::FollowMouse;
	if (ImGui::Checkbox("FollowMouse", &bFollowMouse))m_addType = AddType::FollowMouse;
}

void AddRotationComponent::LoadJson(nlohmann::json _json)
{
	m_addPow = Utility::JsonHelper::InPutVec3(_json["addPow"]);
	m_addType = _json["addType"];
}
nlohmann::json AddRotationComponent::Serialize()
{
	nlohmann::json json;
	json["addPow"] = Utility::JsonHelper::OutPutVec3(m_addPow);
	json["addType"] = m_addType;
	return json;
}

void AddRotationComponent::SetActive(bool _flg)
{
	bool nowFlg = m_bActive;
	Component::SetActive(_flg);
	if (nowFlg == _flg)return;
	if (_flg && !KernelEngine::is_Debugging() && m_addType == AddType::FollowMouse)
	{
		POINT test{ 640,360 };
		ClientToScreen(Application::Instance().GetWindowHandle(), &test);
		SetCursorPos(test.x, test.y);
	}
}

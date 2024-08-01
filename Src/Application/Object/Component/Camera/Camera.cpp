#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Camera::Start()
{
	m_cameraPack = CameraManager::Instance().CreateCameraPack();
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_Camera::ImGuiUpdate()
{
	ImGui::InputInt("Priority", &m_cameraPack->priority);
}

void Cp_Camera::PostUpdateContents()
{
	m_cameraPack->matrix = m_trans.lock()->GetMatrix();
	CameraManager::Instance().RegisterCamera(m_cameraPack);
}

void Cp_Camera::LoadJson(nlohmann::json _json)
{
	m_cameraPack->priority = _json["Priority"];
}

nlohmann::json Cp_Camera::GetJson()
{
	nlohmann::json _json;
	_json["Priority"] = m_cameraPack->priority;
	return _json;
}

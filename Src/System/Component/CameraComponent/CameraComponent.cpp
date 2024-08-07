#include "CameraComponent.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../TransformComponent/TransformComponent.h"

void CameraComponent::Awake()
{
	m_cameraPack = std::make_shared<CameraManager::CameraPack>();
}

void CameraComponent::Start()
{
}

void CameraComponent::ImGuiUpdate()
{
	ImGui::InputInt("Priority", &m_cameraPack->priority);
}

void CameraComponent::PostUpdateContents()
{
	m_cameraPack->matrix = m_trans.lock()->GetMatrix();
	CameraManager::Instance().RegisterCamera(m_cameraPack);
}

void CameraComponent::LoadJson(nlohmann::json _json)
{
	m_cameraPack->priority = _json["Priority"];
}

nlohmann::json CameraComponent::Serialize()
{
	nlohmann::json _json;
	_json["Priority"] = m_cameraPack->priority;
	return _json;
}

#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"

void Cp_Camera::Start()
{
	m_cameraPack = std::make_shared<CameraManager::CameraPack>();
	m_trans = m_owner.lock()->GetTransform();
	//m_preDraw = std::make_shared<std::function<void()>>([&]() {PreDraw(); });
	//RenderManager::Instance().AddPreDraw(m_preDraw);
	//m_owner.lock()->SetCamera(WeakThisPtr(this));
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

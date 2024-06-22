#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../../../RenderManger/RenderManger.h"

void Cp_Camera::Start()
{
	m_camera = std::make_unique<KdCamera>();
	/*m_preDraw = std::make_shared<std::function<void()>>([&]() {PreDraw(); });
	RenderManager::Instance().AddPreDraw(m_preDraw);
	m_owner.lock()->SetCamera(WeakThisPtr(this));*/
}

void Cp_Camera::ImGuiUpdate()
{
	ImGui::InputInt("Priority", &m_priority);
}

void Cp_Camera::PreDraw()
{
	if (m_bActive)
	{
		this;
		m_camera->SetCameraMatrix(m_owner.lock()->GetTransform().lock()->GetMatrix());
		m_camera->SetToShader();
	}
}

void Cp_Camera::PostUpdateContents()
{
}

void Cp_Camera::InitJson()
{
	if (m_jsonData["Priority"].is_number())m_priority = m_jsonData["Priority"];
	RenderManager::Instance().AddCamera(m_priority, WeakThisPtr(this));
}

nlohmann::json Cp_Camera::GetJson()
{
	m_jsonData["Priority"] = m_priority;
	return m_jsonData;
}

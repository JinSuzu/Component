﻿#include "Camera.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../../../RenderManger/RenderManger.h"

void Cp_Camera::Start()
{
	m_camera = std::make_shared<KdCamera>();
	//m_preDraw = std::make_shared<std::function<void()>>([&]() {PreDraw(); });
	//RenderManager::Instance().AddPreDraw(m_preDraw);
	//m_owner.lock()->SetCamera(WeakThisPtr(this));
}

void Cp_Camera::ImGuiUpdate()
{
	ImGui::InputInt("Priority", &m_priority);
}

void Cp_Camera::PreDraw()
{
	if (m_bActive && m_owner.lock()->GetActive())
	{
		m_camera->SetCameraMatrix(m_owner.lock()->GetTransform().lock()->GetMatrix());
		m_camera->SetToShader();
	}
}

void Cp_Camera::PostUpdateContents()
{
}

void Cp_Camera::LoadJson(nlohmann::json _json)
{
	m_priority = _json["Priority"];
	RenderManager::Instance().AddCamera(m_priority, WeakThisPtr(this));
}

nlohmann::json Cp_Camera::GetJson()
{
	nlohmann::json _json;
	_json["Priority"] = m_priority;
	return _json;
}

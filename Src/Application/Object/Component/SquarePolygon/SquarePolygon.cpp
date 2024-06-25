﻿#include "SquarePolygon.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Camera/Camera.h"
#include "../../../AssetManager/AssetManager.h"
#include "../../../Utility/Animation2D/Animation2D.h"
#include "../../../RenderManger/RenderManger.h"
#include "../../../main.h"

void Cp_SquarePolygon::Draw3D(UINT _type)
{
	if (m_owner.lock()->GetHideFlg())return;
	if (!Application::Instance().GetBuildFlg() && !m_bActive)return;
	if (m_drawType & _type)
	{
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_trans.lock()->GetMatrix(), m_color);
	}
}

void Cp_SquarePolygon::Start()
{
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon("chara/chara");
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) {Draw3D(_type); });
	RenderManager::Instance().AddDraw3D(m_draw3D);
}

void Cp_SquarePolygon::PreUpdateContents()
{
	m_squarePolygon->SetSplit(m_split);
	m_animation->PreUpdate(m_squarePolygon.get());
}

void Cp_SquarePolygon::PostUpdateContents()
{
	if (m_cameraFocus)
	{
		m_trans.lock()->SetRotation(RenderManager::Instance().GetCamera().lock()->GetOwner().lock()->GetTransform().lock()->GetRotation());
	}
}

void Cp_SquarePolygon::ImGuiUpdate()
{
	ImGui::InputText("Path", &m_path);
	if (ImGui::SameLine(); ImGui::Button("Load"))m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		ImGuiCheckBoxBit("Lit", m_drawType, DrawType::Lit);
		ImGuiCheckBoxBit("UnLit", m_drawType, DrawType::UnLit);
		ImGuiCheckBoxBit("Bright", m_drawType, DrawType::Bright);
		ImGuiCheckBoxBit("UI", m_drawType, DrawType::UI);
		ImGuiCheckBoxBit("DepthOfShadow", m_drawType, DrawType::DepthOfShadow);

		ImGui::EndPopup();
	}

	ImGui::Checkbox("CameraFocus", &m_cameraFocus);

	ImGui::InputInt2("Split w/h", m_split);
	m_animation->ImGuiUpdate();
}

void Cp_SquarePolygon::InitJson()
{
	m_path = m_jsonData["path"];
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path);
	m_drawType = m_jsonData["drawType"];

	m_split[0] = m_jsonData["splitW"];
	m_split[1] = m_jsonData["splitH"];

	m_animation->SetJson(m_jsonData["Animation"]);

	if (m_jsonData["CameraFocus"].is_boolean())m_cameraFocus = m_jsonData["CameraFocus"];
}

nlohmann::json Cp_SquarePolygon::GetJson()
{
	m_jsonData["path"] = m_path;
	m_jsonData["drawType"] = m_drawType;

	m_jsonData["splitW"] = m_split[0];
	m_jsonData["splitH"] = m_split[1];

	m_jsonData["Animation"] = m_animation->GetJson();

	m_jsonData["CameraFocus"] = m_cameraFocus;
	return m_jsonData;
}

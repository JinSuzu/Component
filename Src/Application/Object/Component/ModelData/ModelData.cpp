#include "ModelData.h"
#include "../../Game/GameObject.h"
#include "../../../AssetManager/AssetManager.h"
#include "../../../RenderManger/RenderManger.h"
#include "../Transform/Transform.h"

void Cp_ModelData::Draw3D(UINT _type)
{
	if (m_drawType & _type)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_trans.lock()->GetMatrix());
	}
}

void Cp_ModelData::Start()
{
	m_modelData = AssetManager::Instance().GetModelData("RPG-7");
	m_trans = m_owner.lock()->GetTransform();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) { Draw3D(_type); });
	RenderManager::Instance().AddDraw3D(m_draw3D);
}

void Cp_ModelData::ImGuiUpdate()
{
	ImGui::InputText("Path", &m_path);
	if (ImGui::SameLine(); ImGui::Button("Load"))
	{
		m_modelData = AssetManager::Instance().GetModelData(m_path);
	}

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
}

void Cp_ModelData::InitJson()
{
	m_drawType  = m_jsonData["drawType"];
	m_path      = m_jsonData["path"];
	m_modelData = AssetManager::Instance().GetModelData(m_path);
}

nlohmann::json Cp_ModelData::GetJson()
{
	m_jsonData["path"]     = m_path;
	m_jsonData["drawType"] = m_drawType;
	return m_jsonData;
}

#include "ModelData.h"
#include "../Transform/Transform.h"
#include "../../Game/GameObject.h"

#include "../../../AssetManager/AssetManager.h"
#include "../../../RenderManger/RenderManger.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"

void Cp_ModelData::Draw3D(UINT _type)
{
	if (m_owner.lock()->GetHideFlg())return;
	if (m_drawType & _type)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelData, m_trans.lock()->GetMatrix());
	}
}

void Cp_ModelData::Start()
{
	m_modelData = AssetManager::Instance().GetModelData("Asset/Data/Model/earth/earth.gltf");
	m_trans = m_owner.lock()->GetTransform();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) { Draw3D(_type); });
	RenderManager::Instance().AddDraw3D(m_draw3D);
}

void Cp_ModelData::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelData(m_modelData, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		MyImGui::CheckBoxAllBit<DrawType>(m_drawType);

		ImGui::EndPopup();
	}
}

void Cp_ModelData::LoadJson(nlohmann::json _json)
{
	m_drawType  = _json["drawType"];
	m_path      = _json["path"];
	m_modelData = AssetManager::Instance().GetModelData(m_path);
}

nlohmann::json Cp_ModelData::GetJson()
{
	nlohmann::json json;
	json["path"]     = m_path;
	json["drawType"] = m_drawType;
	return json;
}

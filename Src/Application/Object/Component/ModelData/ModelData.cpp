#include "ModelData.h"
#include "../Transform/Transform.h"
#include "../../Game/GameObject.h"

void Cp_ModelData::Start()
{
	m_trans = m_owner.lock()->GetTransform();

	m_modelDataPack = std::make_shared<RenderManager::ModelDataPack>();
	m_modelDataPack->modelData = AssetManager::Instance().GetModelData("Asset/Data/Model/earth/earth.gltf");
	m_modelDataPack->mat = &m_trans.lock()->WorkMatrix();
}



void Cp_ModelData::UpdateRenderContents()
{
	if (m_modelDataPack->modelData == nullptr)return;
	if (!m_owner.lock()->GetHideFlg()) 
	{
		RenderManager::Instance().AddDraw3D(m_modelDataPack);
	}
}

void Cp_ModelData::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelData(m_modelDataPack->modelData, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<RenderManager::DrawType>(m_modelDataPack->drawType);
		ImGui::EndPopup();
	}
}

void Cp_ModelData::LoadJson(nlohmann::json _json)
{
	m_modelDataPack->drawType	= _json["drawType"];
	m_path						= _json["path"];
	m_modelDataPack->modelData	= AssetManager::Instance().GetModelData(m_path);
}

nlohmann::json Cp_ModelData::GetJson()
{
	nlohmann::json json;
	json["path"]     = m_path;
	json["drawType"] = m_modelDataPack->drawType;
	return json;
}
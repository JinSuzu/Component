#include "ModelDataComponent.h"
#include "../../../../Application/Object/Game/GameObject.h"

void ModelDataComponent::Start()
{
	m_trans = m_owner.lock()->GetTransform();

	m_modelDataPack = std::make_shared<RenderManager::ModelDataPack>();
	m_modelDataPack->modelData = AssetManager::Instance().GetModelData("Asset/Data/Model/earth/earth.gltf");
	m_modelDataPack->mat = &m_trans.lock()->WorkMatrix();
}



void ModelDataComponent::UpdateRenderContents()
{
	if (m_modelDataPack->modelData == nullptr)return;
	if (!m_owner.lock()->GetHideFlg()) 
	{
		RenderManager::Instance().AddDraw3D(m_modelDataPack);
	}
}

void ModelDataComponent::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelData(m_modelDataPack->modelData, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<RenderManager::DrawType>(m_modelDataPack->drawType);
		ImGui::EndPopup();
	}
}

void ModelDataComponent::LoadJson(nlohmann::json _json)
{
	m_modelDataPack->drawType	= _json["drawType"];
	m_path						= _json["path"];
	m_modelDataPack->modelData	= AssetManager::Instance().GetModelData(m_path);
}

nlohmann::json ModelDataComponent::GetJson()
{
	nlohmann::json json;
	json["path"]     = m_path;
	json["drawType"] = m_modelDataPack->drawType;
	return json;
}
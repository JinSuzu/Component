#include "ModelWorkComponent.h"
#include "../../../../Application/Object/Game/GameObject.h"

void ModelWorkComponent::Awake()
{
	m_modelWorkPack =std::make_shared<RenderManager::ModelWorkPack>();
	m_modelWorkPack->modelWork = AssetManager::Instance().GetModelWork("Asset/Data/Model/earth/earth.gltf");
}

void ModelWorkComponent::Start()
{
	m_modelWorkPack->mat = &m_trans.lock()->WorkMatrix();
	if (m_modelWorkPack->modelWork)m_modelAnimator = std::make_shared<KdAnimator>();
}

void ModelWorkComponent::PreUpdateContents()
{
	if (!m_modelWorkPack->modelWork)return;
	if (!m_modelAnimator)return;

	if (m_modelAnimator->IsAnimationEnd())
	{
		m_modelAnimator->SetAnimation(m_modelWorkPack->modelWork->GetAnimation(m_animeName.empty() ? "" : m_animeName));
	}

	m_modelWorkPack->modelWork->GetAnimation(1);
	m_modelAnimator->AdvanceTime(m_modelWorkPack->modelWork->WorkNodes());
	m_modelWorkPack->modelWork->CalcNodeMatrices();
}

void ModelWorkComponent::UpdateRenderContents()
{
	if (!m_owner.lock()->GetHideFlg()) 
	{
		RenderManager::Instance().AddDraw3D(m_modelWorkPack);
	}
}

void ModelWorkComponent::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelWork(m_modelWorkPack->modelWork, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<RenderManager::DrawType>(m_modelWorkPack->drawType);

		ImGui::EndPopup();
	}
}

void ModelWorkComponent::LoadJson(nlohmann::json _json)
{
	m_modelWorkPack->drawType = _json["drawType"];
	m_path = _json["path"];
	m_modelWorkPack->modelWork = AssetManager::Instance().GetModelWork(m_path);
}

nlohmann::json ModelWorkComponent::Serialize()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["drawType"] = m_modelWorkPack->drawType;
	return json;
}

#include "ModelWork.h"
#include "../Transform/Transform.h"
#include "../../Game/GameObject.h"

void Cp_ModelWork::Start()
{
	m_modelWorkPack = std::make_shared<ModelWorkPack>();
	m_modelWorkPack->modelWork = AssetManager::Instance().GetModelWork("Asset/Data/Model/earth/earth.gltf");
	if (m_modelWorkPack->modelWork)m_modelAnimator = std::make_shared<KdAnimator>();
	m_trans = m_owner.lock()->GetTransform();
}

void Cp_ModelWork::PreUpdateContents()
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

void Cp_ModelWork::PostUpdateContents()
{
	if (!m_owner.lock()->GetHideFlg()) 
	{
		m_modelWorkPack->mat = m_trans.lock()->GetMatrix();
		RenderManager::Instance().AddDraw3D(m_modelWorkPack);
	}
}

void Cp_ModelWork::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelWork(m_modelWorkPack->modelWork, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<DrawType>(m_modelWorkPack->drawType);

		ImGui::EndPopup();
	}
}

void Cp_ModelWork::LoadJson(nlohmann::json _json)
{
	m_modelWorkPack->drawType = _json["drawType"];
	m_path = _json["path"];
	m_modelWorkPack->modelWork = AssetManager::Instance().GetModelWork(m_path);
}

nlohmann::json Cp_ModelWork::GetJson()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["drawType"] = m_modelWorkPack->drawType;
	return json;
}

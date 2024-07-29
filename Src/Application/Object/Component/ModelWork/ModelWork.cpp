#include "ModelWork.h"
#include "../Transform/Transform.h"
#include "../../Game/GameObject.h"

#include "../../../AssetManager/AssetManager.h"
#include "../../../RenderManger/RenderManger.h"

void Cp_ModelWork::Draw3D(UINT _type)
{
	if (m_owner.lock()->GetHideFlg())return;
	if (!m_modelWork)return;
	if (m_drawType & _type)
	{
		KdShaderManager::Instance().m_StandardShader.DrawModel(*m_modelWork, m_trans.lock()->GetMatrix());
	}
}

void Cp_ModelWork::Start()
{
	m_modelWork = AssetManager::Instance().GetModelWork("Asset/Data/Model/earth/earth.gltf");
	if (m_modelWork)m_modelAnimator = std::make_shared<KdAnimator>();
	m_trans = m_owner.lock()->GetTransform();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) { Draw3D(_type); });
	RenderManager::Instance().AddDraw3D(m_draw3D);

	m_drawType = (UINT)DrawType::Lit;
}

void Cp_ModelWork::PreUpdateContents()
{
	if (!m_modelWork)return;
	if (!m_modelAnimator)return;

	if (m_modelAnimator->IsAnimationEnd())
	{
		m_modelAnimator->SetAnimation(m_modelWork->GetAnimation(m_animeName.empty() ? "" : m_animeName));
	}

	m_modelWork->GetAnimation(1);
	m_modelAnimator->AdvanceTime(m_modelWork->WorkNodes());
	m_modelWork->CalcNodeMatrices();

}

void Cp_ModelWork::ImGuiUpdate()
{
	AssetManager::Instance().SelectModelWork(m_modelWork, m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<DrawType>(m_drawType);

		ImGui::EndPopup();
	}
}

void Cp_ModelWork::LoadJson(nlohmann::json _json)
{
	m_drawType = _json["drawType"];
	m_path = _json["path"];
	m_modelWork = AssetManager::Instance().GetModelWork(m_path);
}

nlohmann::json Cp_ModelWork::GetJson()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["drawType"] = m_drawType;
	return json;
}

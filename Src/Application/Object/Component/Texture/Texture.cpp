#include "Texture.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../../../Utility/Timer.h"
#include "../../../Utility/Animation2D/Animation2D.h"
#include "Texture.h"

void Cp_Texture::Start()
{
	m_texPack = std::make_shared<TexturePack>();

	m_texPack->tex  = AssetManager::Instance().GetTexture("Asset/Textures/chara/block.png");
	m_texPack->rect = { 0,0,64,64 };
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();
	KernelEngine::SetStopComponent(WeakThisPtr(this));
}

void Cp_Texture::PreUpdateContents()
{
	m_animation->PreUpdate(m_texPack->rect);
}

void Cp_Texture::PostUpdateContents()
{
	m_texPack->mat = m_trans.lock()->GetMatrix();
	if(!m_owner.lock()->GetHideFlg())RenderManager::Instance().AddDraw2D(m_texPack);
}

void Cp_Texture::ImGuiUpdate()
{
	if (AssetManager::Instance().SelectTexture(m_texPack->tex, m_path))m_texPack->rect = { 0,0,(LONG)m_texPack->tex->GetInfo().Width,(LONG)m_texPack->tex->GetInfo().Height};

	ImGui::DragFloat4("Rect", (float*) & m_texPack->rect.x);
	m_animation->ImGuiUpdate();
}

void Cp_Texture::LoadJson(nlohmann::json _json)
{
	m_path = _json["path"];
	m_texPack->rect = Utility::JsonHelper::InputRect(_json["rect"]);
	m_texPack->tex = AssetManager::Instance().GetTexture(m_path);

	m_animation->SetJson(_json["animation"]);
}

nlohmann::json Cp_Texture::GetJson()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["rect"] = Utility::JsonHelper::OutPutRect(m_texPack->rect);

	json["animation"] = m_animation->GetJson();
	return json;
}
#include "TextureComponent.h"
#include "../../../../Application/Object/Game/GameObject.h"
#include "../../../../Application/Utility/Animation2D/Animation2D.h"
#include "../../../../Application/Utility/Timer.h"

void TextureComponent::Awake()
{
	m_texPack = std::make_shared<RenderManager::TexturePack>();
	m_animation = std::make_shared<Animation2D>();
	m_texPack->rect = { 0,0,64,64 };
}

void TextureComponent::Start()
{
	m_texPack->mat = &m_trans.lock()->WorkMatrix();
}

void TextureComponent::PreUpdateContents()
{
	m_animation->PreUpdate(m_texPack->rect);
}

void TextureComponent::PostUpdateContents()
{

}

void TextureComponent::UpdateRenderContents()
{
	if (m_owner.lock()->GetHideFlg())
	{
		RenderManager::Instance().AddDraw2D(m_texPack);
	}
}

void TextureComponent::ImGuiUpdate()
{
	if (AssetManager::Instance().SelectTexture(m_texPack->tex, m_path))m_texPack->rect = { 0,0,(LONG)m_texPack->tex->GetInfo().Width,(LONG)m_texPack->tex->GetInfo().Height };

	ImGui::DragFloat4("Rect", (float*)&m_texPack->rect.x);
	m_animation->ImGuiUpdate();
}

void TextureComponent::LoadJson(nlohmann::json _json)
{
	m_path = _json["path"];
	m_texPack->rect = Utility::JsonHelper::InputRect(_json["rect"]);
	m_texPack->tex = AssetManager::Instance().GetTexture(m_path);

	m_animation->SetJson(_json["animation"]);
}

nlohmann::json TextureComponent::Serialize()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["rect"] = Utility::JsonHelper::OutPutRect(m_texPack->rect);

	json["animation"] = m_animation->Serialize();
	return json;
}

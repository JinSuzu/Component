#include "Texture.h"
#include "../../Game/GameObject.h"
#include "../../../AssetManager/AssetManager.h"
#include "../Transform/Transform.h"
#include "../../../Utility/Timer.h"
#include "../../../Utility/Animation2D/Animation2D.h"
#include "../../../RenderManger/RenderManger.h"
#include "Texture.h"

void Cp_Texture::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_trans.lock()->GetMatrix());

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_tex, 0, 0, m_rect.width, m_rect.height, &m_rect);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void Cp_Texture::Start()
{
	m_tex   = AssetManager::Instance().GetKdTexture("chara/chara");
	m_rect  = { 0,0,64,64 };
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();

	m_draw2D = std::make_shared<std::function<void()>>([&]() {DrawSprite();});
	RenderManager::Instance().AddDraw2D(m_draw2D);
}

void Cp_Texture::PreUpdateContents()
{
	m_animation->PreUpdate(m_rect);
}

void Cp_Texture::ImGuiUpdate()
{
	ImGui::InputText("Path", &m_path);
	if (ImGui::SameLine();ImGui::Button("Load"))
	{
		m_tex = AssetManager::Instance().GetKdTexture(m_path);
		m_rect = { 0,0,(LONG)m_tex->GetInfo().Width,(LONG)m_tex->GetInfo().Height };
	}

	float value[4] = { (float)m_rect.x,(float)m_rect.y ,(float)m_rect.width ,(float)m_rect.height };
	ImGui::DragFloat4("Rect", value);
	m_rect = Math::Rectangle((long)value[0], (long)value[1], (long)value[2], (long)value[3]);

	m_animation->ImGuiUpdate();
}

void Cp_Texture::InitJson()
{
	m_path = m_jsonData["path"] ;
	m_rect = JsonToRect(m_jsonData["rect"]);
	m_tex = AssetManager::Instance().GetKdTexture(m_path);

	m_animation->SetJson(m_jsonData["animation"]);
}
nlohmann::json Cp_Texture::GetJson()
{
	nlohmann::json json;
	m_jsonData["path"] = m_path;
	m_jsonData["rect"] = RectToJson(m_rect);

	m_jsonData["animation"] = m_animation->GetJson();
	return m_jsonData;
}
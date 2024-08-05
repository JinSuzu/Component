﻿#pragma once
#include "../Component.h"

class Cp_Texture
	:public Component
{
public:
	void Start()override;

	void PreUpdateContents()override;
	void PostUpdateContents()override;

	void UpdateRenderContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdTexture>GetTexture() { return m_texPack->tex; };
private:
	std::shared_ptr<RenderManager::TexturePack> m_texPack;
	std::string									m_path;

	std::shared_ptr<class Animation2D>			m_animation;
};

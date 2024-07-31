#pragma once
#include "../Component.h"

class Cp_Texture
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdTexture>GetTexture() { return m_texPack->tex; };
private:
	std::string					m_path;

	std::shared_ptr<class Animation2D> m_animation;

	struct TexturePack
		:RenderManager::Asset2DPackBase
	{
		void Draw()override 
		{
			KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
			KdShaderManager::Instance().m_spriteShader.DrawTex(tex, 0, 0, rect.width, rect.height, &rect);
			KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
		}

		Math::Matrix				mat = Math::Matrix::Identity;
		Math::Rectangle				rect;
		std::shared_ptr<KdTexture>	tex;
	};

	std::shared_ptr<TexturePack> m_texPack;
};

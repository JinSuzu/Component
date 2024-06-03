#pragma once
#include "../Component.h"

class Cp_Texture
	:public Component
{
public:
	void DrawSprite();

	void Start()override;
	void PreUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:
	std::string					m_path;
	std::shared_ptr<KdTexture>	m_tex;
	Math::Rectangle				m_rect;

	std::weak_ptr<class Cp_Transform> m_trans;

	std::shared_ptr<class Animation2D> m_animation;
	std::shared_ptr<std::function<void()>> m_draw2D;
};

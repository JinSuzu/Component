#pragma once
#include "../Component.h"

class Cp_ModelWork
	:public Component
{
public:
	void Draw3D(UINT _type);

	void Start()override;
	void PreUpdateContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdModelWork> GetModelWork() { return m_modelWork; }
	void SetModelWork(std::shared_ptr<KdModelWork> _data) { m_modelWork = _data; }

	void SetColor(Math::Color _color) { m_color = _color; }
	Math::Color GetColor() const { return m_color; }

private:
	UINT								m_drawType;

	std::string							m_path;
	std::shared_ptr<KdModelWork>		m_modelWork;
	std::shared_ptr<KdAnimator>			m_modelAnimator;

	Math::Color							m_color = kWhiteColor;

	std::shared_ptr<std::function<void(UINT)>> m_draw3D;

	std::string m_animeName;
};

#pragma once

class ModelWorkComponent
	:public Component
{
public:
	void Awake()override;
	void Start()override;
	void PreUpdateContents()override;

	void UpdateRenderContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json Serialize()override;

	std::weak_ptr<KdModelWork> GetModelWork() { return m_modelWorkPack->modelWork; }
	void SetModelWork(std::shared_ptr<KdModelWork> _data) { m_modelWorkPack->modelWork = _data; }

	void SetColor(Math::Color _color) { m_modelWorkPack->color = _color; }
	Math::Color GetColor() const { return m_modelWorkPack->color; }

private:
	std::string							m_path;
	std::shared_ptr<KdAnimator>			m_modelAnimator;
	std::string m_animeName;

	std::shared_ptr<RenderManager::ModelWorkPack> m_modelWorkPack;
};

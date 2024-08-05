#pragma once

class ModelDataComponent
	:public Component
{
public:
	void Start()override;

	void UpdateRenderContents()override;

	void ImGuiUpdate()override;


	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdModelData> GetModelData() { return m_modelDataPack->modelData; }
	void SetModelData(std::shared_ptr<KdModelData> _data) { m_modelDataPack->modelData = _data; }

	void SetColor(Math::Color _color) { m_modelDataPack->color = _color; }
	Math::Color GetColor() const { return m_modelDataPack->color; }

private:
	

	std::shared_ptr<RenderManager::ModelDataPack>	m_modelDataPack;
	std::string										m_path;
};

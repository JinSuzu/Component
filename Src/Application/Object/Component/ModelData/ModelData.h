#pragma once
#include "../Component.h"

class Cp_ModelData
	:public Component
{
public:
	void Start()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;


	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdModelData> GetModelData() { return m_modelDataPack->modelData; }
	void SetModelData(std::shared_ptr<KdModelData> _data) { m_modelDataPack->modelData = _data; }

	void SetColor(Math::Color _color) { m_modelDataPack->color = _color; }
	Math::Color GetColor() const { return m_modelDataPack->color; }

private:
	struct ModelDataPack
		:RenderManager::Asset3DPackBase 
	{
		void Draw(UINT _type)override 
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawModel(*modelData, mat);
			}
		}

		std::shared_ptr<KdModelData>		modelData;
		Math::Matrix						mat;
		Math::Color							color = kWhiteColor;
	};

	std::shared_ptr<ModelDataPack>	m_modelDataPack;
	std::string						m_path;
};

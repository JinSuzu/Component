﻿#pragma once
#include "../Component.h"

class Cp_ModelWork
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void PostUpdateContents()override;

	void ImGuiUpdate()override;
	void LoadJson(nlohmann::json _json)override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdModelWork> GetModelWork() { return m_modelWorkPack->modelWork; }
	void SetModelWork(std::shared_ptr<KdModelWork> _data) { m_modelWorkPack->modelWork = _data; }

	void SetColor(Math::Color _color) { m_modelWorkPack->color = _color; }
	Math::Color GetColor() const { return m_modelWorkPack->color; }

private:
	std::string							m_path;
	std::shared_ptr<KdAnimator>			m_modelAnimator;
	std::string m_animeName;

	struct ModelWorkPack
		:RenderManager::Asset3DPackBase
	{
		void Draw(UINT _type)override
		{
			if (drawType & _type)
			{
				KdShaderManager::Instance().m_StandardShader.DrawModel(*modelWork, mat);
			}
		}

		std::shared_ptr<KdModelWork>		modelWork;
		Math::Matrix						mat;
		Math::Color							color = kWhiteColor;
	};

	std::shared_ptr<ModelWorkPack> m_modelWorkPack;
};
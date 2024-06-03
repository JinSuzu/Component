﻿#pragma once
#include "../Component.h"

class Cp_ModelData
	:public Component
{
public:
	void Draw3D(UINT _type);
	void Start()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

	std::weak_ptr<KdModelData>GetModelData() { return m_modelData; }
private:
	UINT								m_drawType;

	std::string							m_path;
	std::shared_ptr<KdModelData>		m_modelData;

	std::weak_ptr<class Cp_Transform>	m_trans;
	std::shared_ptr<std::function<void(UINT)>> m_draw3D;
};

﻿#pragma once

class _Base
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void UpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	nlohmann::json GetJson()override;
	void LoadJson(nlohmann::json _json)override;

private:

};

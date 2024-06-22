#pragma once
#include "../Component.h"

class _Base
	:public Component
{
public:
	void Start()override;
	void PreUpdateContents()override;
	void UpdateContents()override;
	void PostUpdateContents()override;
	void ImGuiUpdate()override;
	void InitJson()override;
	nlohmann::json GetJson()override;

private:

};

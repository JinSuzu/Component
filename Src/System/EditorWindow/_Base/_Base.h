#pragma once
#include "../Base/EditorWindowBase.h"

class _Base
	:public EditorWindowBase
{
public:
	void Update()override;
	void LoadJson()override;
	nlohmann::json SaveJson()override;
};

#pragma once
#include "../../Object.h"

class EditorWindowBase
	:public Object
{
public:
	virtual void Update() = 0;
	virtual void LoadJson() {}
	virtual nlohmann::json SaveJson() { return "";}

	virtual void SetOwner(class Editor* _editor) final { m_owner = _editor; }
protected:
	Editor* m_owner = nullptr;
};

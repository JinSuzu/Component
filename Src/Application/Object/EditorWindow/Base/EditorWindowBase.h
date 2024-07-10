#pragma once
#include "../../Object.h"

class EditorWindowBase
	:public Object
{
public:
	virtual void Draw() final;
	virtual void Update() {};
	virtual void LoadJson() {}
	virtual nlohmann::json SaveJson() { return ""; }

	virtual void SetOwner(class Editor* _editor) final { m_owner = _editor; }
	virtual void SetName(std::string _str) { m_name = _str; }
protected:
	Editor* m_owner = nullptr;
	std::string m_name = "EditorWindow";
};

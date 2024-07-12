#pragma once
#include "../../Object.h"

class EditorWindowBase
	:public Object
{
public:
	virtual void Update() final;

	virtual void LoadJson() {}
	virtual nlohmann::json SaveJson() { return ""; }

	virtual void SetOwner(class Editor* _editor) final { m_owner = _editor; }
	virtual void SetName(std::string _str) { m_name = _str; }
protected:
	virtual void UpdateContents() {};
	virtual void PostUpdate() {}

	Editor* m_owner = nullptr;
	std::string m_name = "EditorWindow";
};

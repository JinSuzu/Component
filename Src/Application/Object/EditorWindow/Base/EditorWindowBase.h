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

	virtual void ConfigLoad()final;
	virtual void ConfigSave()final;
	
protected:
	virtual void UpdateContents() {};
	virtual void PostUpdate() {}
	virtual void ConfigLoadContents(nlohmann::json& _json) {_json;}
	virtual void ConfigSaveContents(nlohmann::json& _json) {_json;}

	Editor* m_owner = nullptr;
	std::string m_name = "EditorWindow";
};

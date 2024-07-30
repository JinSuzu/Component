#pragma once

class EditorWindowBase
	:public Object
{
public:
	virtual void Update()final;

	virtual void LoadJson() {}
	virtual nlohmann::json SaveJson() { return ""; }

	virtual void SetOwner(class Editor* _editor) final { m_owner = _editor; }
	virtual void SetName(std::string _str) { m_name = _str; }

	virtual void ConfigLoad()final;
	virtual void ConfigSave()final;
	
	virtual void Init();
	virtual void Release();
protected:
	struct ChildWindowOptionSet
	{
		std::function<void()>before;
		std::function<void()>after;
	};

	ChildWindowOptionSet m_beginChildOption;
	ChildWindowOptionSet m_endChildOption;

	Editor* m_owner = nullptr;
	std::string m_name = "EditorWindow";

	virtual void UpdateContents() {};
	virtual void ConfigLoadContents(nlohmann::json& _json) {_json;}
	virtual void ConfigSaveContents(nlohmann::json& _json) {_json;}
};

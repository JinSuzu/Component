#pragma once
#include "../Object/Game/Manager/GameObjectManager.h"

enum SceneID
{
	Title,
	Game,
	Result,
	Max
};

class SceneBase
{
public:
	SceneBase()
		:id()
		, m_bLoad()
	{}
	~SceneBase() {}

	virtual void Draw2D();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Init() 
	{
		id = PickName(typeid(*this).name());
		m_objectMgr.Init();
	};
	virtual void Load()final;
	virtual void Release() final;
	virtual void ImGuiUpdate();

protected:
	std::string id;
	GameObjectManager m_objectMgr;

	bool m_bLoad;
	virtual void ReleaseContent() {};
	virtual void LoadContent() {};
private:
	nlohmann::json m_jsonData;
};
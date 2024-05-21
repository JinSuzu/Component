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
		, m_bLoad(false)
	{}
	~SceneBase() {}

	virtual void PreDraw();
	virtual void Draw();
	virtual void DrawSprite();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Init() {};
	virtual void Load()final;
	virtual void Release() final;
	virtual void ImGuiUpdate();

protected:
	std::string id;
	Math::Matrix				m_world;

	GameObjectManager m_objectMgr;

	bool m_bLoad;
	virtual void ReleaseContent() {};
	virtual void LoadContent() {};

};
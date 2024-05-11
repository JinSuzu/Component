#pragma once

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
	virtual void Init() { id = typeid(*this).name(); };
	virtual void Load()final;
	virtual void Release() final;
	virtual void ImGuiUpdate();

protected:
	std::string id;

	bool m_bLoad;
	virtual void ReleaseContent() {};
	virtual void LoadContent() {};
};
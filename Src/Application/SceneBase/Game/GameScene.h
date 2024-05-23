#pragma once
#include "../SceneBase.h"
class GameScene :
    public SceneBase
{
public:
    GameScene()
	{
		m_name = "Game";
	}
    ~GameScene() {}

	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

    void Init()override;
	void LoadContent()override;
private:
};


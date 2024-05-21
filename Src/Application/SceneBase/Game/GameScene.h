#pragma once
#include "../SceneBase.h"
class C_GameScene :
    public SceneBase
{
public:
    C_GameScene()
	{
		id = "Game";
	}
    ~C_GameScene() {}

    void PreDraw()override;

	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

    void Init()override;
	void LoadContent()override;
private:
};


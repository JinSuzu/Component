#pragma once
#include "../SceneBase.h"
class C_ResultScene :
    public SceneBase
{
public:
    C_ResultScene(){}
    ~C_ResultScene() {}

    void Draw2D()override;
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;
    void Init()override;
private:
};


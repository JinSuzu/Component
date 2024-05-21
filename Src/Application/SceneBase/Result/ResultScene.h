#pragma once
#include "../SceneBase.h"
class C_ResultScene :
    public SceneBase
{
public:
    C_ResultScene()
	{
		id = "Result";
	}
    ~C_ResultScene() {}

    void Draw()override;
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;
    void Init()override;
private:
};


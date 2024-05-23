#include "GameScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"
#include "../../Object/Component/Component.h"

void GameScene::PreUpdate()
{
	SceneBase::PreUpdate();
}

void GameScene::Update()
{
	SceneBase::Update();
}

void GameScene::PostUpdate()
{
	SceneBase::PostUpdate();
}

void GameScene::Init()
{
	SceneBase::Init();
}

void GameScene::LoadContent()
{
	/*auto obj1 = GameObjectManager::Instance().CreateObject("Test");
	GameObjectManager::Instance().CreateObject("TestC")
		->SetParent(obj1);*/
}

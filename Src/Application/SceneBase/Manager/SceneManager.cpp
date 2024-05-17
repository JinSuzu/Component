#include "SceneManager.h"
#include "../SceneBase.h"	
#include "../Titile/TitleScene.h"
#include "../Game/GameScene.h"
#include "../Result/ResultScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"

#include "../../main.h"

void SceneManager::Draw()
{
	if (m_scene.get() == nullptr)return;
	m_scene->Draw2D();
}
void SceneManager::PreUpdate()
{
	if (m_scene.get() == nullptr)return;
	m_scene->PreUpdate();
}
void SceneManager::Update()
{
	if (m_scene.get() == nullptr)return;
	m_scene->Update();
}

void SceneManager::PostUpdate()
{
	if (m_scene.get() == nullptr)return;
	m_scene->PostUpdate();
}

void SceneManager::Init()
{
	m_geneSceneList.push_back([=]() {return std::make_shared<C_TitleScene>(); });
	m_geneSceneList.push_back([=]() {return std::make_shared<C_GameScene>(); });
	m_geneSceneList.push_back([=]() {return std::make_shared<C_ResultScene>(); });

	m_nowSceneNum = SceneID::Game;
	auto newScene = m_geneSceneList[m_nowSceneNum]();
	newScene->Init();
	GameObjectManager::Instance().Init();
	newScene->Load();
	m_scene = newScene;
}

void SceneManager::ImGuiUpdate()
{
	if (m_scene == nullptr)return;
	m_scene->ImGuiUpdate();
}

void SceneManager::ShiftScene(SceneID _toSceneNum)
{
	if (m_nowSceneNum == _toSceneNum)return;

	//Release前処理
	auto temp = m_geneSceneList[m_nowSceneNum]();
	temp->Init();
	m_scene->Release();

	//Release後処理
	m_nowSceneNum = _toSceneNum;
	
	GameObjectManager::Instance().Init();
	bool flg = true;
	auto Fn = [temp]() {temp->Load(); };

	std::thread mask(&SceneManager::DrawLoad, this, std::ref(flg));
	std::thread load(Fn);

	load.join();
	flg = false;
	mask.join();

	m_scene = temp;
}

void SceneManager::DrawLoad(bool& flg)
{
	while (flg)
	{
		continue;
		static auto load =
		{
			GameObjectManager::Instance().CreateObject("Back",false) ,
			GameObjectManager::Instance().CreateObject("Load",false) ,
			GameObjectManager::Instance().CreateObject("Loading",false)
		};


		for (auto&& it : load)it->Update();
		Application::Instance().KdBeginDraw(false);
		{
			KdShaderManager::Instance().m_spriteShader.Begin();
			{
				for (auto&& it : load)it->Draw();
			}
			KdShaderManager::Instance().m_spriteShader.End();
		}
		Application::Instance().KdPostDraw();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

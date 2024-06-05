#include "SceneManager.h"
#include "../SceneBase.h"	
#include "../Titile/TitleScene.h"
#include "../Game/GameScene.h"
#include "../Result/ResultScene.h"
#include "../../Object/Game/GameObject.h"
#include "../../Object/Game/Manager/GameObjectManager.h"

#include "../../main.h"

#define MAKESCENE(NAME)\
class NAME##Scene :public SceneBase { public: NAME##Scene(){m_name = #NAME;} };\
m_registerScene[SceneID::##NAME] = ([=]() {return std::make_shared<NAME##Scene>(); });

void SceneManager::PreUpdate()
{
	if (m_scene == nullptr)return;
	m_scene->PreUpdate();
}
void SceneManager::Update()
{
	if (m_scene == nullptr)return;
	m_scene->Update();
}

void SceneManager::PostUpdate()
{
	if (m_scene == nullptr)return;
	m_scene->PostUpdate();
}

void SceneManager::Init()
{
	m_registerScene[SceneID::Title]		= ([=]() {return std::make_shared<TitleScene>(); });
	m_registerScene[SceneID::Game]		= ([=]() {return std::make_shared<GameScene>(); });
	m_registerScene[SceneID::Result]	= ([=]() {return std::make_shared<ResultScene>(); });
	MAKESCENE(Kurosaki)
	MAKESCENE(Yamamoto)
	MAKESCENE(Motoori)

	m_nowSceneNum = SceneID::Game;
	auto newScene = m_registerScene[m_nowSceneNum]();
	m_scene = newScene;
	newScene->Init();
	newScene->Load();
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
	auto temp = m_registerScene[_toSceneNum]();
	temp->Init();
	m_scene->Release();

	//Release後処理
	m_nowSceneNum = _toSceneNum;
	m_scene = temp;
	
	//bool flg = true;
	auto Fn = [temp]() {temp->Load(); };
	Fn();
	/*std::thread mask(&SceneManager::DrawLoad, this, std::ref(flg));
	std::thread load(Fn);

	load.join();
	flg = false;
	mask.join();*/

}

void SceneManager::DrawLoad(bool& flg)
{
	while (flg)
	{
		/*
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
		*/
	}
}

void SceneManager::ReLoad()
{
	m_scene->Release();
	m_scene->Load();
}

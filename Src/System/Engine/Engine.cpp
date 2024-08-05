#include "Engine.h"
#include "../../Application/Object/Component/AllComponentIncluder.h"
#include "../../Application/Object/Component/Component.h"
#include "../../Application/Object/Game/GameObject.h"
#include "../../Application/Object/Component/Transform/Transform.h"
#include "../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../Application/Utility/Timer.h"

#include "../EditorWindow/DebugLog/DebugLog.h"

namespace KernelEngine
{
	enum FlowState
	{
		None,
		Playing,
		Stopping,
		Building
	};

	static int							flowState = FlowState::Building;
	bool								Debugging = true;

	void Draw()
	{
		RenderManager::Instance().BeginDraw();
		{
			RenderManager::Instance().Draw();
			RenderManager::Instance().PostDraw();
			RenderManager::Instance().PreDraw();
			Editor::Instance().ImGuiDraw();
		}
		RenderManager::Instance().EndDraw();
	}
	void Update()
	{
		SceneManager::Instance().PushScene();

		//Begin
		{
			// 入力状況の更新
			KdInputManager::Instance().Update();

			// 空間環境の更新
			KdShaderManager::Instance().WorkAmbientController().Update();
		}

		//Update
		{
			Timer::Instance().PreUpdate();

			if (is_Playing())
			{
				GameObjectManager::Instance().PreUpdate();
				GameObjectManager::Instance().Update();
				GameObjectManager::Instance().PostUpdate();
			}
			else
			{
				//ゲーム停止時の座標更新
				std::list<std::shared_ptr<GameObject>>::iterator gIt = GameObjectManager::Instance().WorkObjectList().begin();
				std::list<std::shared_ptr<GameObject>>::iterator gEnd = GameObjectManager::Instance().WorkObjectList().end();
				while (gIt != gEnd)
				{
					if ((*gIt)->GetDestroy())
					{
						gIt = GameObjectManager::Instance().WorkObjectList().erase(gIt);
						continue;
					}
					(*gIt)->GetTransform().lock()->MatrixUpdata();
					gIt++;
				}

			}

			Editor::Instance().ImGuiUpdate();

			GameObjectManager::Instance().UpdateRender();
		}

		//End
		{
			// 3DSoundListnerの行列を更新
			KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());
		}
	}

	void Init()
	{
		Editor::Instance().Init();
		CameraManager::Instance().Init();
		SceneManager::Instance().Init();

		RegisterComponentInit();
	}
	void Release()
	{
		SceneManager::Instance().Release();
		Editor::Instance().Release();
	}

	void Start()
	{
		SceneManager::Instance().LoadScene(SceneManager::Instance().GetNowSceneName());
		flowState = FlowState::Playing;
		ShowCursor(flowState == FlowState::Playing);
	}
	void Stop()
	{
		flowState = FlowState::Stopping;
		ShowCursor(flowState == FlowState::Playing);
	}
	void End()
	{
		SceneManager::Instance().LoadScene(SceneManager::Instance().GetNowSceneName());
		flowState = FlowState::Building;
		ShowCursor(flowState == FlowState::Playing);
	}

	void AddLog(const char* fmt, ...)
	{
		Editor::Instance().GetDebugLog().lock()->AddLog(fmt);
	}

	const bool  is_Playing()
	{
		return flowState == FlowState::Playing;
	}
	const bool  is_Building()
	{
		return flowState == FlowState::Building;
	}
	const bool is_Stopping()
	{
		return flowState == FlowState::Stopping;
	}
	const bool& is_Debugging()
	{
		return Debugging;
	}

	void StartButton()
	{
		bool playing = is_Playing();
		if (ImGui::Checkbox("Start", &playing))
		{
			if (playing)
			{
				Start();
			}
			else
			{
				End();
			}
		}
	}
	void StopButton()
	{
		bool playing = is_Playing();
		bool stoped = is_Stopping();
		if (!stoped && !playing)return;
		if (ImGui::Checkbox("stop", &stoped))
		{
			if (stoped)
			{
				flowState = FlowState::Stopping;
			}
			else
			{
				flowState = FlowState::Playing;
			}
		}
	}
	void SetDebugging(const bool& _debugging)
	{
		Debugging = _debugging;
	}
}
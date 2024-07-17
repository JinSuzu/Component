#include "RenderManger.h"
#include "../Object/Component/Texture/Texture.h"
#include "../Object/Component/Camera/Camera.h"
#include "../Object/Game/GameObject.h"
#include "../main.h"

void RenderManager::BeginDraw()
{
	if (Application::Instance().GetDebugFlg())
	{
		m_rtp.ClearTexture();
		m_rtc.ChangeRenderTarget(m_rtp);
	}
	KdDirect3D::Instance().ClearBackBuffer();
	KdShaderManager::Instance().WorkAmbientController().Draw();
	KdShaderManager::Instance().m_postProcessShader.Draw();
}
void RenderManager::PreDraw()
{
	std::map<int, std::list<std::weak_ptr<Cp_Camera>>>::iterator camera = m_cameraMap.begin();
	while (camera != m_cameraMap.end())
	{
		std::list <std::weak_ptr<class Cp_Camera>>::iterator it = camera->second.begin();
		while (it != camera->second.end())
		{
			if (it->lock())
			{
				if (it->lock()->GetActive() && it->lock()->GetOwner().lock()->GetActive())
				{
					it->lock()->PreDraw();
					return;
				}
				it++;
			}
			else
			{
				it = camera->second.erase(it);
			}
		}
		camera++;
	}
}
void RenderManager::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		std::list<std::weak_ptr<std::function<void(UINT)>>>::iterator draw3D = m_draw3DList.begin();
		while (draw3D != m_draw3DList.end())
		{
			if (draw3D->expired())
			{
				draw3D = m_draw3DList.erase(draw3D);
				continue;
			}

			(*draw3D->lock())((UINT)DrawType::DepthOfShadow);
			draw3D++;
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())((UINT)DrawType::Lit);
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())((UINT)DrawType::UnLit);
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())((UINT)DrawType::Bright);
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}
void RenderManager::PostDraw()
{
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();
	DrawSprite();
	DrawDebug();
	m_rtc.UndoRenderTarget();
}
void RenderManager::EndDraw()
{
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}

void RenderManager::AddCamera(int _priority, std::weak_ptr<class Cp_Camera> _camera)
{
	std::map<int, std::list<std::weak_ptr<class Cp_Camera>>>::iterator map = m_cameraMap.find(_priority);
	if (map == m_cameraMap.end())
	{
		m_cameraMap[_priority] = std::list<std::weak_ptr<class Cp_Camera>>();
	}
	m_cameraMap[_priority].push_back(_camera);
}

std::weak_ptr<Cp_Camera> RenderManager::GetCamera()
{
	std::map<int, std::list<std::weak_ptr<class Cp_Camera>>>::iterator camera = m_cameraMap.begin();
	while (camera != m_cameraMap.end())
	{
		std::list <std::weak_ptr<class Cp_Camera>>::iterator it = camera->second.begin();
		while (it != camera->second.end())
		{
			if (it->lock())
			{
				if (it->lock()->GetActive() && it->lock()->GetOwner().lock()->GetActive())
				{
					return *it;
				}
				it++;
			}
			else
			{
				it = camera->second.erase(it);
			}
		}
		camera++;
	}

	return std::weak_ptr<class Cp_Camera>();

}

void RenderManager::Init()
{
	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	/*
	if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}
	*/

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (KdDirect3D::Instance().Init(Application::Instance().GetWindowHandle(), 1280, 720, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(Application::Instance().GetWindowHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
	}

	// フルスクリーン設定
	if (bFullScreen) {
		HRESULT hr;

		hr = KdDirect3D::Instance().SetFullscreenState(TRUE, 0);
		if (FAILED(hr))
		{
			MessageBoxA(Application::Instance().GetWindowHandle(), "フルスクリーン設定失敗", "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		}
	}

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();

	DebugViewResize(1280, 720);
}

void RenderManager::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		std::list<std::weak_ptr<std::function<void()>>>::iterator draw2D = m_draw2DList.begin();
		while (draw2D != m_draw2DList.end())
		{
			if (draw2D->expired())
			{
				draw2D = m_draw2DList.erase(draw2D);
				continue;
			}

			(*draw2D->lock())();
			draw2D++;
		}
	}
	KdShaderManager::Instance().m_spriteShader.End();
}
void RenderManager::DrawDebug()
{
	if (!Application::Instance().GetDebugFlg())return;
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		std::list<std::weak_ptr<std::function<void()>>>::iterator drawDebug = m_drawDebugList.begin();
		while (drawDebug != m_drawDebugList.end())
		{
			if (drawDebug->expired())
			{
				drawDebug = m_drawDebugList.erase(drawDebug);
				continue;
			}

			(*drawDebug->lock())();
			drawDebug++;
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();
}
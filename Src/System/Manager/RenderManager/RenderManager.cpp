#include "RenderManager.h"
#include "../../../Application/Object/Component/Texture/Texture.h"
#include "../../../Application/Object/Component/Camera/Camera.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/main.h"

void RenderManager::BeginDraw()
{
	if (KernelEngine::is_Debugging())
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
	std::list<std::weak_ptr<std::function<void()>>>::iterator preDraw = m_preDrawList.begin();
	while (preDraw != m_preDrawList.end())
	{
		if (preDraw->expired())
		{
			preDraw = m_preDrawList.erase(preDraw);
			continue;
		}

		(*preDraw->lock())();
		preDraw++;
	}
	
	CameraManager::Instance().CameraToShader();
}
void RenderManager::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		for (auto& draw3D : m_draw3DList)draw3D.lock()->Draw((UINT)DrawType::DepthOfShadow);
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		for (auto& draw3D : m_draw3DList)draw3D.lock()->Draw((UINT)DrawType::Lit);
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& draw3D : m_draw3DList)draw3D.lock()->Draw((UINT)DrawType::UnLit);
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& draw3D : m_draw3DList)draw3D.lock()->Draw((UINT)DrawType::Bright);
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
	m_draw2DList.clear();
	m_draw3DList.clear();
	KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);
}

void RenderManager::Init(int w, int h)
{
	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();
	DebugViewResize(w, h);
}

void RenderManager::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		for (auto& draw2D : m_draw2DList)draw2D.lock()->Draw();
	}
	KdShaderManager::Instance().m_spriteShader.End();
}
void RenderManager::DrawDebug()
{
	if (!KernelEngine::is_Debugging())return;
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
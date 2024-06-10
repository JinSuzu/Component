#include "RenderManger.h"
#include "../Object/Component/Texture/Texture.h"

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

			(*draw3D->lock())(DrawType::DepthOfShadow);
			draw3D++;
		}
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())(DrawType::Lit);
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())(DrawType::UnLit);
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		for (auto& draw3D : m_draw3DList)(*draw3D.lock())(DrawType::Bright);
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
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

std::shared_ptr<KdTexture> RenderManager::CreateBackBuffer()
{
	RenderTarget render;
	render.BeginRenderTarget();
	{
		PreDraw();
		Draw();
		DrawSprite();
	}
	render.EndRenderTarget();
	return render.GetResult();
}

void RenderTarget::BeginRenderTarget()
{
	bCallBegin = true;
	tex = std::make_shared<KdTexture>();
	tex->CreateRenderTarget(1280, 720);
	tex->ClearRenderTarget();
	tex->SetRenderTarget();
}

void RenderTarget::EndRenderTarget()
{
	bCallEnd = true;
	KdDirect3D::Instance().SetBackBuffer();
}
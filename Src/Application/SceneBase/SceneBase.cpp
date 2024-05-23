﻿#include "SceneBase.h"
#include "../Object/Game/Manager/GameObjectManager.h"

void SceneBase::PreDraw()
{
	m_objectMgr.PreDraw();
}

void SceneBase::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginGenerateDepthMapFromLight();
	{
		m_objectMgr.GenerateDepthMapFromLight();
	}
	KdShaderManager::Instance().m_StandardShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginLit();
	{
		m_objectMgr.DrawLit();
	}
	KdShaderManager::Instance().m_StandardShader.EndLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_StandardShader.BeginUnLit();
	{
		m_objectMgr.DrawUnLit();
	}
	KdShaderManager::Instance().m_StandardShader.EndUnLit();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
		m_objectMgr.DrawBright();
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

void SceneBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		m_objectMgr.DrawSprite();
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

void SceneBase::PreUpdate()
{
	m_objectMgr.PreUpdate();
}

void SceneBase::Update()
{
	m_objectMgr.Update();
}

void SceneBase::PostUpdate()
{
	m_objectMgr.PostUpdate();
}

void SceneBase::Load()
{
	if (m_bLoad)return;//ロード済みreturn
	m_bLoad = true;
	m_objectMgr.Load("Scene/" + m_name);
	LoadContent();
}

void SceneBase::Release()
{
	if (!m_bLoad)return;//リリース済みreturn
	m_bLoad = false;
	m_objectMgr.Release("Scene/" + m_name);
	ReleaseContent();
}

void SceneBase::ImGuiUpdate()
{
	ImGui::Text(m_name.c_str());
	ImGui::BeginChild("ObjectWindow"); 
	{
		m_objectMgr.ImGuiUpdate();
	}
	ImGui::EndChild();
}

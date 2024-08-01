#include "GameScreen.h"

#include "../../../Application/Object/Component/Camera/Camera.h"

#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Component/BuildCamera/BuildCamera.h"
#include "../../../Application/Object/Component/Transform/Transform.h"

#include "../../../Application/main.h"

#include "../../Manager/EditorWindowManager/EditorWindowManager.h"

void GameScreen::Init()
{
		//ゲームビューの枠組み子ウィンドウのbegin処理
	m_beginChildOption.before
		= []()
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
		};
	m_beginChildOption.after	//ゲームビューの枠組み子ウィンドウのbegin処理
		= [&]()
		{
			ImVec2 windowSize(1280.0f, 720.0f);
			m_imageSize = ImGui::GetContentRegionAvail() / windowSize;
			m_imageSize = windowSize * (m_imageSize.x < m_imageSize.y ? m_imageSize.x : m_imageSize.y); //ゲームSceneのサイズ決定

			Utility::ImGuiHelper::SetCenterCursorPos(m_imageSize);

			ImGui::BeginChild(std::to_string((int)this).c_str(), m_imageSize, ImGuiChildFlags_Border);
			ImGui::PopStyleVar(1);																		//計算したサイズでギズモ用の子ウィンドウを生成

			if (m_cameraController.expired())return;
			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))											//子ウィンドウに対してドラックしたか
			{
				m_cameraController.lock()->AngleShift();
			}
		};

	m_endChildOption.before		//ゲームビューの枠組み子ウィンドウのend処理
		= [&]()
		{
			ImGui::EndChild();
			if (m_cameraController.expired())return;
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
				{
					m_cameraController.lock()->HoverMove();
				}
			}
		};
}

void GameScreen::UpdateContents()
{
	/*
	if (ImGui::SameLine(); ImGui::Button("cameraSetUp"))ImGui::OpenPopup("camera");
	if (ImGui::BeginPopup("camera"))
	{
		if (m_buildCamera)GameObjectManager::Instance().ImGuiGameObject(m_buildCamera);
		ImGui::EndPopup();
	}

	*/
	if (!m_buildCamera && KernelEngine::is_Building())
	{
		m_buildCamera = GameObjectManager::CreateObject(std::string("BuildCamera"), std::weak_ptr<GameObject>(), false);
		m_buildCamera->AddComponent("Camera");
		m_cameraController = std::static_pointer_cast<Cp_BuildCamera>(m_buildCamera->AddComponent("BuildCamera"));
	}
	else if (!KernelEngine::is_Building())m_buildCamera = nullptr;

	//ゲームビュー表示
	Utility::ImGuiHelper::ImageWindowCenter(RenderManager::Instance().GetDebugView().m_RTTexture->WorkSRView(), m_imageSize);

	//ビルドカメラ
	if (m_buildCamera)
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			m_cameraController.lock()->AngleShift();
		}
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
			{
				m_cameraController.lock()->HoverMove();
			}
		}

		m_buildCamera->PreUpdate();
		m_buildCamera->Update();
		m_buildCamera->PostUpdate();
	}

	//============================================
	// ギズモ処理
	//============================================
	std::weak_ptr<GameObject> editObject = Editor::Instance().GetEditObject();
	if (editObject.expired())return;

	std::weak_ptr<Cp_Transform> trans = editObject.lock()->GetTransform();
	std::weak_ptr<Cp_Transform> parentTrans = trans.lock()->GetParent();
	Utility::ImGuizmoHelper::TransformPack resultPack;
	//ギズモ更新処理
	bool edited = Utility::ImGuizmoHelper::Update
	(
		CameraManager::Instance().GetCamera().lock()->GetCameraMatrix().Invert(),
		CameraManager::Instance().GetCamera().lock()->GetProjMatrix(),
		trans.lock()->GetMatrix(),
		(parentTrans.expired() ? Math::Matrix::Identity : parentTrans.lock()->GetMatrix()),
		m_zmoPreation,
		resultPack
	);
	if (edited)
	{
		std::weak_ptr<Cp_Transform> trans = Editor::Instance().GetEditObject().lock()->GetTransform();
		trans.lock()->SetPosition(resultPack.position);
		trans.lock()->SetRotation(resultPack.rotation);
		trans.lock()->SetScale(resultPack.scale);
	}

	//ギズモの編集対象の変更
	ImGui::Begin("OprerationChanger");
	{
		Utility::ImGuizmoHelper::OprerationChanger(m_zmoPreation);
	}
	ImGui::End();
}
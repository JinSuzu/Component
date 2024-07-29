#include "GameScreen.h"

#include "../../../Application/Object/Component/Camera/Camera.h"

#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Component/BuildCamera/BuildCamera.h"
#include "../../../Application/Object/Component/Transform/Transform.h"

#include "../../../Application/RenderManger/RenderManger.h"
#include "../../../Application/main.h"

#include "../../Manager/EditorWindowManager/EditorWindowManager.h"

void GameScreen::UpdateContents()
{
	/*
	bool buildChange = Utility::ImGuiHelper::ButtonWindowCenter(Application::Instance().GetBuildFlg() ? "StartRun" : "StartBuild");
	buildChange |= GetAsyncKeyState(Application::Instance().GetBuildFlg() ? VK_F5 : VK_ESCAPE) & 0x8000;
	if (buildChange)
	{
		//SceneManager::Instance().ReLoad();
		Application::Instance().TurnBuildFlg();
	}

	if (ImGui::SameLine(); ImGui::Button("cameraSetUp"))ImGui::OpenPopup("camera");
	if (ImGui::BeginPopup("camera"))
	{
		if (m_buildCamera)GameObjectManager::Instance().ImGuiGameObject(m_buildCamera);
		ImGui::EndPopup();
	}

	*/
	if (!m_buildCamera && Application::Instance().GetBuildFlg())
	{
		m_buildCamera = GameObjectManager::CreateObject(std::string("BuildCamera"), std::weak_ptr<GameObject>(), false);
		m_buildCamera->AddComponent("Camera");
		m_cameraController = std::static_pointer_cast<Cp_BuildCamera>(m_buildCamera->AddComponent("BuildCamera"));
	}
	else if (!Application::Instance().GetBuildFlg())m_buildCamera = nullptr;
	ID3D11ShaderResourceView* rtv = RenderManager::Instance().GetDebugView().m_RTTexture->WorkSRView();
	Utility::ImGuiHelper::ImageWindowCenter(rtv, m_imageSize);
	if (m_cameraController.lock())
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
		{
			m_cameraController.lock()->AngleShift();
		}

	if (Application::Instance().GetEditor().lock()->GetEditObject().lock())
	{
		Math::Matrix resultMat;
		std::weak_ptr<KdCamera> camera = RenderManager::Instance().GetCamera().lock()->GetCamera();
		//ギズモ更新処理
		bool edited = Utility::ImGuizmoHelper::Update
		(
			camera.lock()->GetCameraMatrix().Invert(),
			camera.lock()->GetProjMatrix(),
			Application::Instance().GetEditor().lock()->GetEditObject().lock()->GetTransform().lock()->GetMatrix(),
			m_zmoPreation,
			resultMat
		);
		if (edited)
		{
			Application::Instance().GetEditor().lock()->GetEditObject().lock()->GetTransform().lock()->SetPosition(resultMat.Translation());
		}

	}
	if (m_cameraController.lock())
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
			{
				m_cameraController.lock()->HoverMove();
			}
		}

	//ギズモの編集対象の変更
	ImGui::Begin("OprerationChanger");
	{
		Utility::ImGuizmoHelper::OprerationChanger(m_zmoPreation);
	}
	ImGui::End();
}

void GameScreen::BeginChildOption()
{
	// 画面中央に固定するためのオフセット計算
	ImVec2 imagePos = ((ImGui::GetContentRegionAvail() - m_imageSize) * 0.5f);

	// 位置を設定
	ImGui::SetCursorPos(imagePos);
	ImVec2 windowSize(1280.0f, 720.0f);
	m_imageSize = ImGui::GetContentRegionAvail() / windowSize;
	m_imageSize = windowSize * (m_imageSize.x < m_imageSize.y ? m_imageSize.x : m_imageSize.y);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.5,0.5));
	ImGui::BeginChild(std::to_string((int)this).c_str(), m_imageSize,ImGuiChildFlags_Border);
	ImGui::PopStyleVar(1);
	if (m_cameraController.expired())return;
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		m_cameraController.lock()->AngleShift();
	}
}

void GameScreen::EndChildOption()
{
	EditorWindowBase::EndChildOption();
	if (m_cameraController.expired())return;
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
		{
			m_cameraController.lock()->HoverMove();
		}
	}
}

void GameScreen::Screen(ImVec2* _resultSize)
{
	ImVec2 windowSize(1280.0f, 720.0f);
	m_imageSize = ImGui::GetContentRegionAvail() / windowSize;
	m_imageSize = windowSize * (m_imageSize.x < m_imageSize.y ? m_imageSize.x : m_imageSize.y);
}
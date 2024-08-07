#include "GameScreen.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Component/Camera/BuildCameraComponent/BuildCameraComponent.h"
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
	if (!m_buildCamera && KernelEngine::is_Building())
	{
		m_buildCamera = GameObjectManager::CreateObject(std::string("BuildCamera"), std::weak_ptr<GameObject>(), false);
		m_buildCamera->AddComponent<CameraComponent>();
		m_cameraController = std::static_pointer_cast<BuildCameraComponent>(m_buildCamera->AddComponent<BuildCameraComponent>());
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
	std::weak_ptr<TransformComponent> trans = editObject.lock()->GetTransform();
	Math::Matrix transform = trans.lock()->GetMatrix();
	//ギズモ更新処理
	bool edited = Utility::ImGuizmoHelper::Update
	(
		CameraManager::Instance().GetCamera().lock()->GetCameraMatrix().Invert(),
		CameraManager::Instance().GetCamera().lock()->GetProjMatrix(),
		transform,
		m_zmoPreation,
		m_zmoMode
	);
	if (edited)
	{
		std::weak_ptr<TransformComponent> parentTrans = trans.lock()->GetParent();
		if (!parentTrans.expired())transform *= parentTrans.lock()->GetMatrix().Invert();

		Math::Vector3 pos, rotation, scale;
		Math::Quaternion quaternion;
		//ImGuizmo::DecomposeMatrixToComponents();
		transform.Decompose(scale, quaternion, pos);

		rotation = Utility::MathHelper::ToDegrees(quaternion);

		trans.lock()->SetLoaclPosition(pos);
		trans.lock()->SetLocalRotation(rotation);
		trans.lock()->SetLocalScale(scale);
	}

	//ギズモの編集対象の変更
	ImGui::Begin("zmoOption");
	{
		Utility::ImGuizmoHelper::OprerationChanger(m_zmoPreation);
		Utility::ImGuizmoHelper::ModeChanger(m_zmoMode);
	}
	ImGui::End();
}
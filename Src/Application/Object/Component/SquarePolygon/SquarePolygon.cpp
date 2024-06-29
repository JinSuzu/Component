#include "SquarePolygon.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Camera/Camera.h"

#include "../../../AssetManager/AssetManager.h"
#include "../../../ImGuiHelper/ImGuiHelper.h"
#include "../../../Utility/Animation2D/Animation2D.h"
#include "../../../RenderManger/RenderManger.h"
#include "../../../main.h"
void Cp_SquarePolygon::Draw3D(UINT _type)
{
	if (m_owner.lock()->GetHideFlg())return;
	if (!Application::Instance().GetBuildFlg() && !m_bActive)return;
	if (m_drawType & _type)
	{
		Animation2D::AnimeFrame frame = m_animation->GetAnimeFrame();
		m_squarePolygon->SetUVRect(frame.w, frame.h);
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_squarePolygon, m_trans.lock()->GetMatrix(Math::Matrix::CreateTranslation(m_offsetPos)),m_color);
	}
}

void Cp_SquarePolygon::Start()
{
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon("chara/chara");
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) {Draw3D(_type); });
	RenderManager::Instance().AddDraw3D(m_draw3D);
}

void Cp_SquarePolygon::PreUpdateContents()
{
	m_squarePolygon->SetSplit(m_split);
	m_animation->PreUpdate(m_squarePolygon.get());
}

void Cp_SquarePolygon::PostUpdateContents()
{
	if (m_cameraFocus)
	{
		m_trans.lock()->SetRotation(RenderManager::Instance().GetCamera().lock()->GetOwner().lock()->GetTransform().lock()->GetRotation());
	}
}

void Cp_SquarePolygon::ImGuiUpdate()
{
	MyImGui::SelectSquarePolygon(m_squarePolygon,m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		MyImGui::CheckBoxAllBit<DrawType>(m_drawType);
		ImGui::EndPopup();
	}

	ImGui::DragFloat3("OffsetPos", &m_offsetPos.x);
	ImGui::Checkbox("CameraFocus", &m_cameraFocus);

	ImGui::InputInt2("Split w/h", m_split);
	m_animation->ImGuiUpdate();
}

void Cp_SquarePolygon::InitJson()
{
	m_path = m_jsonData["path"];
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path);
	m_drawType = m_jsonData["drawType"];

	m_split[0] = m_jsonData["splitW"];
	m_split[1] = m_jsonData["splitH"];

	m_animation->SetJson(m_jsonData["Animation"]);

	if (m_jsonData["OffsetPos"].is_object())m_offsetPos = MyJson::InPutVec3(m_jsonData["OffsetPos"]);
	if (m_jsonData["CameraFocus"].is_boolean())m_cameraFocus = m_jsonData["CameraFocus"];
}

nlohmann::json Cp_SquarePolygon::GetJson()
{
	m_jsonData["path"] = m_path;
	m_jsonData["drawType"] = m_drawType;

	m_jsonData["splitW"] = m_split[0];
	m_jsonData["splitH"] = m_split[1];

	m_jsonData["Animation"] = m_animation->GetJson();

	m_jsonData["OffsetPos"] = MyJson::OutPutVec3(m_offsetPos);
	m_jsonData["CameraFocus"] = m_cameraFocus;
	return m_jsonData;
}

void Cp_SquarePolygon::SetAnimeName(std::string _name)
{
	m_animation->ChangeAnime(_name);
}

#include "SquarePolygon.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Camera/Camera.h"

#include "../../../AssetManager/AssetManager.h"
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
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path = "Asset/Textures/chara/block.png");
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();

	m_draw3D = std::make_shared<std::function<void(UINT)>>([&](UINT _type) {Draw3D(_type); });
	m_drawType = (UINT)DrawType::Lit;
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
		std::weak_ptr<Cp_Camera> camera = RenderManager::Instance().GetCamera();
		if(camera.lock())m_trans.lock()->SetRotation(camera.lock()->GetOwner().lock()->GetTransform().lock()->GetRotation());
	}
}

void Cp_SquarePolygon::ImGuiUpdate()
{
	AssetManager::Instance().SelectSquarePolygon(m_squarePolygon,m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<DrawType>(m_drawType);
		ImGui::EndPopup();
	}

	ImGui::DragFloat3("OffsetPos", &m_offsetPos.x);
	ImGui::Checkbox("CameraFocus", &m_cameraFocus);

	ImGui::InputInt2("Split w/h", m_split);
	m_animation->ImGuiUpdate();
}



void Cp_SquarePolygon::LoadJson(nlohmann::json _json)
{
	m_path = _json["path"];
	m_squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path);
	m_drawType = _json["drawType"];

	m_split[0] = _json["splitW"];
	m_split[1] = _json["splitH"];

	m_animation->SetJson(_json["Animation"]);

	if (_json["OffsetPos"].is_object())m_offsetPos = Utility::JsonHelper::InPutVec3(_json["OffsetPos"]);
	if (_json["CameraFocus"].is_boolean())m_cameraFocus = _json["CameraFocus"];
}

nlohmann::json Cp_SquarePolygon::GetJson()
{
	nlohmann::json json;
	json["path"] = m_path;
	json["drawType"] = m_drawType;

	json["splitW"] = m_split[0];
	json["splitH"] = m_split[1];

	json["Animation"] = m_animation->GetJson();

	json["OffsetPos"] = Utility::JsonHelper::OutPutVec3(m_offsetPos);
	json["CameraFocus"] = m_cameraFocus;
	return json;
}

void Cp_SquarePolygon::SetAnimeName(std::string _name)
{
	m_animation->ChangeAnime(_name);
}

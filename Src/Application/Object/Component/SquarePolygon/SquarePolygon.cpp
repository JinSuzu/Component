#include "SquarePolygon.h"
#include "../../Game/GameObject.h"
#include "../Transform/Transform.h"
#include "../Camera/Camera.h"

#include "../../../Utility/Animation2D/Animation2D.h"
#include "../../../main.h"

void Cp_SquarePolygon::Start()
{
	m_squarePolygonPack->squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path = "Asset/Textures/chara/block.png");
	m_trans = m_owner.lock()->GetTransform();
	m_animation = std::make_shared<Animation2D>();

	m_squarePolygonPack->drawType = (UINT)DrawType::Lit;
	KernelEngine::SetStopComponent(WeakThisPtr(this));
}

void Cp_SquarePolygon::PreUpdateContents()
{
	m_squarePolygonPack->squarePolygon->SetSplit(m_split);
	m_animation->PreUpdate(m_squarePolygonPack->squarePolygon.get());
}

void Cp_SquarePolygon::PostUpdateContents()
{
	if (m_cameraFocus)
	{
		m_trans.lock()->SetRotation(CameraManager::Instance().GetCamera().lock()->GetCameraMatrix().ToEuler());
	}

	Animation2D::AnimeFrame frame = m_animation->GetAnimeFrame();
	m_squarePolygonPack->squarePolygon->SetUVRect(frame.w, frame.h);

	m_squarePolygonPack->mat = m_owner.lock()->GetTransform().lock()->GetMatrix();

	if (!m_owner.lock()->GetHideFlg())RenderManager::Instance().AddDraw3D(m_squarePolygonPack);
}

void Cp_SquarePolygon::ImGuiUpdate()
{
	AssetManager::Instance().SelectSquarePolygon(m_squarePolygonPack->squarePolygon,m_path);

	if (ImGui::Button("DrawType"))ImGui::OpenPopup("Types");
	if (ImGui::BeginPopup("Types"))
	{
		Utility::ImGuiHelper::CheckBoxAllBit<DrawType>(m_squarePolygonPack->drawType);
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
	m_squarePolygonPack->squarePolygon = AssetManager::Instance().GetSquarePolygon(m_path);
	m_squarePolygonPack->drawType = _json["drawType"];

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
	json["drawType"] = m_squarePolygonPack->drawType;

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

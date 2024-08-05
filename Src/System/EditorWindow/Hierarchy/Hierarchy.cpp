#include "Hierarchy.h"
#include "../Prefab/Prefab.h"
#include "../../../Application/Object/Game/GameObject.h"
#include "../../../Application/Object/Game/Manager/GameObjectManager.h"
#include "../../../Application/object/Game/Manager/GameObjectManager.h"
#include "../../../Application/main.h"

void Hierarchy::UpdateContents()
{
	m_openedOption = false;
	for (auto& obj : GameObjectManager::Instance().GetObjectList())if (obj->GetParent().expired())ImGuiGameObject(obj);
}

void Hierarchy::GameObjectOption(std::weak_ptr<GameObject> _parent)
{
	if (m_openedOption)return;
	std::string ID = "GameObjectOption" + std::to_string((int)_parent.lock().get());
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))ImGui::OpenPopup(ID.c_str());
	if (ImGui::BeginPopup(ID.c_str()))
	{
		m_openedOption = true;
		if (_parent.lock())
		{
			static std::string path;
			ImGui::InputText("##Path", &path);
			if (ImGui::SameLine(); ImGui::Button("Save"))
			{
				nlohmann::json json = nlohmann::json::array();
				json.push_back(_parent.lock()->OutPutFamilyJson());
				Utility::JsonHelper::OutputJson(json, path);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Remove"))_parent.lock()->Destroy();
		}
		if (ImGui::Button("Create"))GameObjectManager::CreateObject(std::string(), _parent);
		ImGui::EndPopup();
	}
}

void Hierarchy::Init()
{
	//いちアイテムに対してのドラックドロップ処理
	m_dragDrop = std::make_shared<Utility::ImGuiHelper::DragDrop<std::weak_ptr<GameObject>>>();
	m_dragDrop->source.push_back([](std::weak_ptr<GameObject> _obj) {return MyDragDrop::SourceGameObjectData(_obj); });		//ゲームオブジェクトの保存/親子関係の移動用
	m_dragDrop->target.push_back([](std::weak_ptr<GameObject> _obj) {return MyDragDrop::TargetGameObjectData(_obj); });		//ゲームオブジェクトの親子関係の移動用
	m_dragDrop->target.push_back([](std::weak_ptr<GameObject> _obj) {return MyDragDrop::TargetGameObjectDataPath(_obj); });	//ゲームオブジェクトのPrefabからの生成用

	//windowへのドラックドロップ適応
	m_endChildOption.after = [&]()
		{
			std::weak_ptr<GameObject> emplyObj;
			m_dragDrop->CallTarget(emplyObj);
			GameObjectOption();
		};
}

void Hierarchy::ImGuiGameObject(std::weak_ptr<GameObject> _obj, bool _colledSource)
{
	ImGuiTreeNodeFlags treeFlg = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	std::list<std::weak_ptr<GameObject>> childs = _obj.lock()->GetChilds();
	if (childs.empty())treeFlg = ImGuiTreeNodeFlags_Leaf;

	bool flg = ImGui::TreeNodeEx((_obj.lock()->GetName() + "##" + std::to_string(_obj.lock()->GetInstanceID())).c_str(), treeFlg);
	if ((ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))) Editor::Instance().SetEditObject(_obj);
	GameObjectOption(_obj);
	_colledSource |= m_dragDrop->CallSource(_obj);
	if (!_colledSource)m_dragDrop->CallTarget(_obj);

	if (flg)
	{
		int i = 0;
		std::list<std::weak_ptr<GameObject>>::iterator child = childs.begin();
		while (child != childs.end())
		{
			if (child->expired())
			{
				child = childs.erase(child);
				continue;
			}

			ImGuiGameObject(*child, _colledSource);
			child++;
		}
		ImGui::TreePop();
	}
}
#pragma once
#include "../Object.h"

class Component;
class Cp_Transform;
class Cp_Collider;
class Cp_Camera;

enum class ObjectTag 
{
	Untagged,
	Respawn,
	Finish,
	EditorOnly,
	MainCamera,
	Player,
	Enemy,
	GameController,
	Max
};

class GameObject
	:public Object
{
public:
	GameObject() {}
	~GameObject() { Release(); }

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Init(nlohmann::json _json);
	void Release();

	void ImGuiUpdate(int num);
	void ImGuiOpenOption();
	#pragma region Get/SetFunction
	std::string GetName() { return m_name; };
	std::string* WorkName() { return &m_name; };
	void SetName(std::string _name) { m_name = _name; };

	ObjectTag GetTag() const { return m_tag; };
	void SetTag(ObjectTag _tag) { m_tag = _tag; };

	std::weak_ptr<GameObject> GetParent()	{ return m_parent; }
	void SetParent(std::weak_ptr<GameObject> _parent);

	std::list<std::weak_ptr<GameObject>>& GetChilds() { return m_childs; }
	void AddChilds(std::weak_ptr<GameObject> _child)
	{
		_child.lock()->SetParent(WeakThisPtr(this));
		m_childs.push_back(_child); 
	}

	bool GetHideFlg() { return m_bHide; };
	void SetHideFlg(bool _flg) { m_bHide = _flg; };

	virtual void Destroy()override;

	//Json係	
	bool GetAbleSave() const { return m_bSave; }
	void DotSave() { m_bSave = false; }
	void EnableSave() { m_bSave = true; }
	nlohmann::json GetJson();
	nlohmann::json OutPutFamilyJson();
#pragma endregion

	#pragma region ComponentFns
	std::weak_ptr<Cp_Transform>GetTransform() { return m_trans; }
	void SetCamera(std::weak_ptr<Cp_Camera> _camera) { m_camera = _camera; }

	std::shared_ptr<Component> AddComponent(std::string _name, nlohmann::json _json = nlohmann::json());
	std::shared_ptr<Component> AddComponent(UINT _id, nlohmann::json _json = nlohmann::json());
	std::shared_ptr<Component> AddComponent(std::shared_ptr<Component> _add);
	std::list<std::shared_ptr<Component>> AddComponents(unsigned int _id);
	std::list<std::shared_ptr<Component>> AddComponents();

	template<class T>
	std::weak_ptr<T> GetComponent()
	{
		std::shared_ptr<Component> tag = SearchTag(typeid(T).name());
		if (tag == nullptr) return std::weak_ptr<T>();//ケツまで回ってたらナシ
		return std::weak_ptr<T>(std::dynamic_pointer_cast<T>(tag));
	}

	template<class T>
	std::list<std::weak_ptr<T>> GetComponents()
	{
		auto tag = SearchTags(typeid(T).name());
		if (tag.empty()) return std::list<std::weak_ptr<T>>();//ケツまで回ってたらナシ

		std::list<std::weak_ptr<T>>list;
		for (auto& it : tag) list.push_back(std::dynamic_pointer_cast<T>(it));

		return list;
	}

	void ImGuiComponents();

	void SetActive(bool _flg)override;
	bool GetActive()override;
private:
	void ComponentInit(std::shared_ptr<Component>& _addCp, nlohmann::json _json);
public:
#pragma endregion

	std::shared_ptr<GameObject>Initialize(std::weak_ptr<GameObject> _parent);
private:
	std::string									m_name;
	ObjectTag									m_tag = ObjectTag::Untagged;
	bool										m_bHide = false;

	//親子関係＆家族関係
	std::weak_ptr<GameObject>					m_parent;
	std::list<std::weak_ptr<GameObject>>		m_childs;

	//前提コンポども
	std::shared_ptr<Cp_Transform>				m_trans;
	std::weak_ptr<Cp_Camera>					m_camera;

	std::list<std::shared_ptr<Component>>		m_cpList;

	//Json係
	bool										m_bSave = true;
	nlohmann::json								m_jsonData;
private:
	//便利関数
	std::shared_ptr<Component> SearchTag(std::string _tag);
	std::list<std::shared_ptr<Component>> SearchTags(std::string _tag);


};
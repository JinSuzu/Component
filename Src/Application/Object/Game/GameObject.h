#pragma once
#include "../Object.h"

class Component;
class Cp_Transform;
enum ComponentID;

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
	:public std::enable_shared_from_this<GameObject>
	,public Object
{
public:
	GameObject() {}
	~GameObject() { Release(); }

	void Draw();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void Init(std::string _name);
	void ImGuiUpdate();

	std::string GetName() { return m_name; };
	ObjectTag GetTag() const { return m_tag; };
	void SetTag(ObjectTag _tag) { m_tag = _tag; };

	std::weak_ptr<Cp_Transform>GetTransform() { return m_trans; }
	#pragma region ComponentFns
public:
	std::shared_ptr<Component> AddComponent(unsigned int _id);
	std::shared_ptr<Component> AddComponent(Component* _add);
	std::list<std::shared_ptr<Component>> AddComponents(unsigned int _id);
	std::list<std::shared_ptr<Component>> AddComponents();

	template<class T>
	std::weak_ptr<T> GetComponent() 
	{
		std::shared_ptr<Component> tag = SearchTag(typeid(T).name());
		if (tag.get() == nullptr) return std::weak_ptr<T>();//ケツまで回ってたらナシ
		return std::weak_ptr<T>(std::dynamic_pointer_cast<T>(tag));
	}
	
	template<class T>
	std::list<std::weak_ptr<T>> GetComponents() 
	{
		auto tag = SearchTags(typeid(T).name());
		if (tag.empty()) return std::list<std::weak_ptr<T>>();//ケツまで回ってたらナシ
		
		std::list<std::weak_ptr<T>>list;
		for (auto& it : tag) list.push_back(std::dynamic_pointer_cast<T>(tag));
		
		return list;
	}

private:
	void ComponentInit(std::shared_ptr<Component>& _addCp);
	void ComponentInit(std::shared_ptr<Component>& _addCp,nlohmann::json& _json);
public:
#pragma endregion

	std::weak_ptr<GameObject> GetParent() { return m_parent; }
	void SetParent(std::weak_ptr<GameObject> _parent);



	//Json係
	void DotSave() { m_bSave = false; }
	nlohmann::json& GetJson() { return m_jsonData; };

private:
	std::string								m_name;
	ObjectTag								m_tag = ObjectTag::Untagged;

	std::shared_ptr<Cp_Transform>			m_trans;
	std::list<std::shared_ptr<Component>>	m_cpList;

	std::weak_ptr<GameObject>				m_parent;

	//Json係
	bool									m_bSave = true;
	nlohmann::json							m_jsonData;
private:
	//便利関数
	std::shared_ptr<Component> SearchTag(std::string _tag);
	std::list<std::shared_ptr<Component>> SearchTags(std::string _tag);

	void Release();
};
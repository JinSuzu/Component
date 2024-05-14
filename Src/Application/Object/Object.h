﻿#pragma once

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

class Object
	:public std::enable_shared_from_this<Object>
{
public:
	Object();
	~Object()
	{
		Release();
	}

	void Draw();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void Init(std::string _name);

	void ImGuiUpdate();

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

	std::list<std::shared_ptr<Component>>& GetComponentList() { return m_cpList; }

	bool GetActive() const { return m_bActive; }
	void SetActive(bool _active) { m_bActive = _active; }

	std::string GetName() { return m_name; };
	ObjectTag GetTag() const { return m_tag; };

	std::weak_ptr<Cp_Transform>GetspTransform();

	void Destroy()		{ m_bDestroy = false; }
	void SetTag(ObjectTag _tag) { m_tag = _tag; };

	//Fn(ComponentID::'ID')
	void AddComponent(unsigned int _id);
	void AddComponent(Component* _add);

	void AddComponents(unsigned int _id);



	bool CheckID(unsigned int _id) const;
	bool CheckIDs(unsigned int _id)const;

	//Json係
	void DotSave() { m_bSave = false; }
	nlohmann::json& GetJson() { return m_jsonData; };

private:
	bool												m_bDestroy = false;
	bool												m_bActive = true;

	std::string											m_name;
	ObjectTag											m_tag = ObjectTag::Untagged;

	std::shared_ptr<Cp_Transform>						m_trans;
	std::list<std::string>								m_setCompoIDs;
	unsigned int										m_compoID = 0;
	std::list<std::shared_ptr<Component>>				m_cpList;

	//Json係
	bool												m_bSave = true;
	std::string											m_filePath;
	nlohmann::json										m_jsonData;

	//便利関数
	std::shared_ptr<Component> SearchTag(std::string _tag);
	std::list<std::shared_ptr<Component>> SearchTags(std::string _tag);
private:
	void Release();
};
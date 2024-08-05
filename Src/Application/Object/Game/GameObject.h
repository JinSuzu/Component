#pragma once
class Component;

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
	void PreUpdate();
	void Update();
	void PostUpdate();

	void UpdateRender();

	void Init(nlohmann::json _json);

#pragma region Get/SetFunction
	const std::string& GetName() { return m_name; };
	std::string& WorkName() { return m_name; };
	void SetName(std::string _name) { m_name = _name; };

	ObjectTag GetTag() const { return m_tag; };
	void SetTag(ObjectTag _tag) { m_tag = _tag; };

	std::weak_ptr<GameObject> GetParent() { return m_parent; }
	void SetParent(std::weak_ptr<GameObject> _parent) { m_parent = _parent; };
	void SetUpParent(std::weak_ptr<GameObject> _parent, bool _push = true);

	std::list<std::weak_ptr<GameObject>>& GetChilds() { return m_childs; }
	std::list<std::weak_ptr<GameObject>>* WorkChilds() { return &m_childs; }
	void AddChilds(std::weak_ptr<GameObject> _child);

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
	std::weak_ptr<TransformComponent>GetTransform() { return m_trans; }

	template<FromComponent T>
	std::shared_ptr<Component> AddComponent(nlohmann::json _json = nlohmann::json())
	{
		std::shared_ptr<Component> addCp
			= ComponentFactory::Instance().CreateComponent
			(
				Utility::TypeIDHelper::Create<T>().hash_code()
			);
		m_cpList.push_back(addCp);
		addCp->m_owner = WeakThisPtr(this);
		addCp->Start();
		if (!_json.is_null())
		{
			addCp->LoadJson(_json);
		}
		return addCp;
	}
	std::shared_ptr<Component> AddComponent(size_t _id, nlohmann::json _json = nlohmann::json());

	template<FromComponent T>
	std::weak_ptr<T> GetComponent()
	{
		const size_t& myID = Utility::TypeIDHelper::Create<T>().hash_code();
		auto it = std::find_if(
			m_cpList.begin(),
			m_cpList.end(),
			[&](const std::shared_ptr<Component>& component) {
				const size_t& targetID = component->GetID();
				return myID == targetID;
			}
		);

		if (it == m_cpList.end()) return std::weak_ptr<T>();//ケツまで回ってたらナシ
		return std::weak_ptr<T>(std::dynamic_pointer_cast<T>(*it));
	}
	template<FromComponent T>
	std::list<std::weak_ptr<T>> GetComponents()
	{
		const size_t& myID = Utility::TypeIDHelper::Create<T>().hash_code();

		std::list<std::weak_ptr<T>> list;
		for (auto& it : m_cpList)
		{
			const size_t& targetID = it->GetID();
			if (myID == targetID)
			{
				list.push_back(std::dynamic_pointer_cast<T>(it));
			}
		}

		if (list.empty()) return std::list<std::weak_ptr<T>>();//ケツまで回ってたらナシ

		return list;
	}

	void ImGuiComponents();

	void SetActive(bool _flg)override;
	bool GetActive()override;
public:
#pragma endregion

private:
	std::string									m_name = "GameObject";
	ObjectTag									m_tag = ObjectTag::Untagged;
	bool										m_bHide = false;

	//親子関係＆家族関係
	std::weak_ptr<GameObject>					m_parent;
	std::list<std::weak_ptr<GameObject>>		m_childs;
	bool										m_addFamily = false;

	//コンポども
	std::shared_ptr<TransformComponent>					m_trans;
	std::list<std::shared_ptr<Component>>		m_cpList;

	//Json係
	bool										m_bSave = true;
};
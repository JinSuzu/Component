#pragma once
#include "../Object.h"

class GameObject;
enum class ObjectTag;

enum ComponentID
{
	Null					= 0,
	//独立コンポ
	BoxCollision			= 1 << 0,
	Draw					= 1 << 1,
	Rigidbody				= 1 << 2,

	//依存コンポ
	AddRotation				= 1 << 3,
	Controller				= 1 << 4,
	Camera					= 1 << 5,

	MaxID					= 1 << 6,
};

class Component
	:public std::enable_shared_from_this<Component>
	,public Object
{

public:
	Component() {}
	virtual	~Component() {}

	virtual void Start() {}

	virtual void PreUpdate()	final { if (m_bActive && !m_bDestroy)PreUpdateContents(); }
	virtual void Update()		final { if (m_bActive && !m_bDestroy)UpdateContents(); }
	virtual void PostUpdate()	final { if (m_bActive && !m_bDestroy)PostUpdateContents(); }

	virtual void ImGuiUpdate() {}

	virtual std::weak_ptr<GameObject> GetOwner()				final	{ return m_owner; }
	virtual void SetOwner(std::weak_ptr<GameObject> _object)	final	{ m_owner = _object; }


	virtual std::string GetIDName()			final	{ return m_IDName; }
	virtual void SetIDName(std::string _tag)   final	{ m_IDName = _tag; }

	virtual ComponentID GetID() { return m_id; };
	virtual void SetID(unsigned int _id) { m_id = (ComponentID)_id; };

	virtual bool CheckIDName(std::string _tag)	final	{return m_IDName == _tag;}

	//Json係
	virtual void InitJson() {};
	virtual nlohmann::json GetJson();
	virtual void SetJson(nlohmann::json _json) { m_jsonData = _json; }

protected:
	std::weak_ptr<GameObject>				m_owner;

	std::shared_ptr<std::function<void()>>	m_spDraw2D;
	std::shared_ptr<std::function<void()>>	m_spUpdate;

	std::string								m_name;

	std::string								m_IDName;
	ComponentID                             m_id = ComponentID::Null;

	nlohmann::json							m_jsonData;

protected:
	virtual void PreUpdateContents() {}
	virtual void UpdateContents() {}
	virtual void PostUpdateContents() {}
};			

#define PRESET \
void Start()override;								\
void PreUpdateContents()override;					\
void UpdateContents()override;						\
void PostUpdateContents()override;					\
void ImGuiUpdate()override;							\
void InitJson()override;							\
nlohmann::json GetJson()override;

template<class T>
class ComponentFactoryList {
public:
	// シングルトンインスタンスの取得
	static ComponentFactoryList<T>& Instance() {
		static ComponentFactoryList<T> instance;
		return instance;
	}

	void Set(std::weak_ptr<T> _add) {
		list.push_back(_add);
	}

	std::list<std::weak_ptr<T>>& Get() {
		return list;
	}

	// コピーとムーブを禁止
	ComponentFactoryList(const ComponentFactoryList&) = delete;
	ComponentFactoryList& operator=(const ComponentFactoryList&) = delete;
	ComponentFactoryList(ComponentFactoryList&&) = delete;
	ComponentFactoryList& operator=(ComponentFactoryList&&) = delete;

private:
	ComponentFactoryList() {}  
	
	// プライベートコンストラクタ
	std::list<std::weak_ptr<T>> list;
};

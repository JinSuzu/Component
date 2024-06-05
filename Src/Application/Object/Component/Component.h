#pragma once
#include "../Object.h"

class GameObject;
enum class ObjectTag;

class Component
	:public Object
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


	virtual std::string GetIDName()			final	{ return m_idName; }
	virtual void SetIDName(std::string _tag)   final	{ m_idName = _tag; }

	virtual bool CheckIDName(std::string _tag)	final	{return m_idName == _tag;}

	//Json係
	virtual void InitJson() {};
	virtual nlohmann::json GetJson() { return nlohmann::json(); }
	virtual void SetJson(nlohmann::json _json) { m_jsonData = _json; }

protected:
	std::weak_ptr<GameObject>				m_owner;

	std::shared_ptr<std::function<void()>>	m_spDraw2D;
	std::shared_ptr<std::function<void()>>	m_spUpdate;

	std::string								m_name;

	std::string								m_idName;
	UINT									m_id = 0;

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

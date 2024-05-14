#pragma once

class Object;
enum class ObjectTag;

enum ComponentID
{
	//独立コンポ
	BoxCollision			= 1 << 0,
	DrawTex					= 1 << 1,
	Rigidbody				= 1 << 2,
	Transform				= 1 << 3,

	//依存コンポ
	AddRotation				= 1 << 4,

	MaxID					= 1 << 5,
};

class Component
	:public std::enable_shared_from_this<Component>
{
public:
	Component() 
	{}
	virtual	~Component() {}

	virtual void Start() {};

	virtual void Draw() {}

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	virtual void ImGuiUpdate() {}

	virtual std::weak_ptr<Object> GetOwner() final	{ return m_owner; }
	virtual void SetOwner(std::weak_ptr<Object> _object) final	{ m_owner = _object; }


	virtual std::string GetTag()final				{ return m_tag; }
	virtual void SetTag(std::string _tag)   final	{ m_tag = _tag; }

	virtual bool CheckTag(std::string _tag)	final	{return m_tag == _tag;}

	//Josn係
	virtual void InitJson();
	virtual nlohmann::json GetJson() {};

protected:
	std::weak_ptr<Object>					m_owner;
	nlohmann::json							m_jsonData;

	std::shared_ptr<std::function<void()>>	m_spDraw2D;
	std::shared_ptr<std::function<void()>>	m_spUpdate;

	std::string m_tag;
};			

#define PRESET \
void Start()override;	\
void PreUpdate()override;							\
void Update()override;								\
void PostUpdate()override;							\
void ImGuiUpdate()override;							\
void InitJson()override;							\
nlohmann::json GetJson()override;

/*
#define GENCOMPONENTLIST(NAME)													\
class NAME##List																\
{																				\
public:																			\
	void Set(std::weak_ptr<Cp_##NAME> _add) { list.push_back(_add); }			\
	std::list<std::weak_ptr<Cp_##NAME>>& Get() { return list; }					\
private:																		\
	std::list<std::weak_ptr<Cp_##NAME>>list;									\
private:																		\
	NAME##List() {}																\
public:																			\
	static NAME##List& Instance()												\
	{																			\
		static NAME##List inst;													\
		return inst;															\
	}\
};

#define FNTURN(_fnList,arg)				\
auto fn = _fnList.begin();				\
while (fn != _fnList.end())				\
{										\
	if ((*fn).lock().get() == nullptr)	\
	{									\
		fn = _fnList.erase(fn);			\
		continue;						\
	}									\
										\
	(*fn).lock()->operator()(arg);		\
	fn++;								\
}
*/

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

#define GENCOMPONENTLIST(NAME)													\
class NAME##List: public ComponentFactoryList<Cp_##NAME>						\
{																				\
private:																		\
	NAME##List();																\
	virtual ~NAME##List();														\
};

#define FNTURN(_fnList,arg)				\
auto fn = _fnList.begin();				\
while (fn != _fnList.end())				\
{										\
	if ((*fn).lock().get() == nullptr)	\
	{									\
		fn = _fnList.erase(fn);			\
		continue;						\
	}									\
										\
	(*fn).lock()->operator()(arg);		\
	fn++;								\
}

#define COMPONENTLISTINSTANCE(NAME)\
ComponentFactoryList<Cp_##NAME>::Instance()

#define TOARRAY3(v) {v.x,v.y,v.z}


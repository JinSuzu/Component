#pragma once
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

	virtual void UpdateRender()	final { if (!m_bDestroy)UpdateRenderContents(); }

	virtual void ImGuiUpdate() {}

	virtual std::weak_ptr<GameObject> GetOwner()				final { return m_owner; }
	virtual void SetOwner(std::weak_ptr<GameObject> _object)	final { m_owner = _object; }

	virtual const size_t& GetID() { return m_id; }
	virtual void SetID(size_t _id) { m_id = _id; };

	//Json係
	virtual nlohmann::json GetJson() { return nlohmann::json(); }
	virtual void LoadJson(nlohmann::json _json) {}

protected:
	std::weak_ptr<GameObject>				m_owner;
	std::weak_ptr<class Cp_Transform>		m_trans;

	size_t									m_id = 0;

protected:
	virtual void PreUpdateContents() {}
	virtual void UpdateContents() {}
	virtual void PostUpdateContents() {}
	virtual void UpdateRenderContents() {}
};

#define PRESET \
void Start()override;								\
void PreUpdateContents()override;					\
void UpdateContents()override;						\
void PostUpdateContents()override;					\
void ImGuiUpdate()override;							\
void InitJson()override;							\
nlohmann::json GetJson()override;

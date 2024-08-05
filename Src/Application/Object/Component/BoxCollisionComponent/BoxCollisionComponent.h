#pragma once


class TransformComponent;

class BoxCollisionComponent
	:public Component
{
public:
	BoxCollisionComponent() {}
	~BoxCollisionComponent() {}
	
	void Start()override;
	void LoadJson(nlohmann::json _json)override;

	void PreUpdateContents()override;
	void UpdateContents()override;
	void PostUpdateContents()override;

	void ImGuiUpdate()override;

	nlohmann::json GetJson()override;

	bool WithPoint(std::weak_ptr<BoxCollisionComponent> _pos);
	bool WithPoint(Math::Vector3 _pos);

	bool WithSquare(std::weak_ptr<BoxCollisionComponent> _coliSet);	
	
	void PushColliList(enum ObjectTag _add);

	void PushTriggerFn	(std::function<void(enum ObjectTag)> _fn)	{ m_triggerAttach = _fn; }
	void PushHitFn(std::function<void(enum ObjectTag)> _fn)		{ m_hitAttach = _fn; }

	void TurnPostPos(std::weak_ptr<BoxCollisionComponent> _other);
	bool GetTrigger() const { return m_bTrigger; }

	void OnHit() { m_bHit = true; }
private:
	std::weak_ptr<TransformComponent>	m_wpTransform;

	Math::Vector3				m_rad = { 1,1,1 };
	bool						m_bTrigger = false;

	bool						m_bHit = false;
	Math::Vector3				m_postPos;

	std::function<void(enum ObjectTag)> m_triggerAttach;
	std::function<void(enum ObjectTag)> m_hitAttach;

	std::list<enum ObjectTag>m_colliList;

	std::vector<Math::Vector3> GetVertex();
	static std::list<std::weak_ptr<BoxCollisionComponent>>& GetCollisList() 
	{
		static std::list<std::weak_ptr<BoxCollisionComponent>> instance;
		return instance;
	}
};

//ComponentFactoryList<Cp_BoxCollision> BoxCollisionList;
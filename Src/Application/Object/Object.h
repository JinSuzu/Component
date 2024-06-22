#pragma once
class Object;
template<class Type, class This = std::remove_pointer_t<Type>>
concept GameObjectDerrivedConcept =
std::is_base_of_v<std::enable_shared_from_this<Object>, This>
&& requires(Type& value)
{
	typename This::Object;
	value->shared_from_this();
};

class Object
	:public std::enable_shared_from_this<Object>
{
public:
	virtual bool GetActive(){ return m_bActive; }
	virtual void SetActive(bool _active) { m_bActive = _active; }

	virtual void Destroy() { m_bDestroy = true; }
	virtual bool GetDestroy() const { return m_bDestroy; }

	virtual int GetInstanceID() final { return (int)this; }
protected:
	bool m_bDestroy = false;
	bool m_bActive = true;

	template<GameObjectDerrivedConcept Type, class This = std::remove_pointer_t<Type>>
	std::weak_ptr<This> WeakThisPtr(Type)
	{
		static_assert(std::is_base_of_v<std::enable_shared_from_this<Object>, This>, "this type is not inherited from std::enable_shared_from_this<GameObject>");

		return std::static_pointer_cast<This>(shared_from_this());
	}
};

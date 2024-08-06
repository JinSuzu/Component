#pragma once
class Component;

template<typename T>concept FromComponent = std::is_base_of_v<Component, T>;

class ComponentFactory
{
public:
	struct Value
	{
		std::string name;
		std::function<std::shared_ptr<Component>()> creator;
	};

	std::shared_ptr<Component>CreateComponent(size_t _id)
	{
		std::unordered_map<size_t, Value>::iterator it
			= m_registry.find(_id);
		if (it == m_registry.end()) return nullptr;

		return it->second.creator();
	}

	template<FromComponent T>
	void RegisterComponent()
	{
		const type_info& ID = Utility::TypeIDHelper::Create<T>();

		Value value;
		value.creator = [&]()
			{
				std::shared_ptr<T> temp = std::make_shared<T>();
				temp->SetID(ID.hash_code());
				return temp;
			};
		value.name = Utility::StringHelper::PickName(ID.name());
		Utility::StringHelper::DeleteWord(value.name, "Component");

		m_registry[ID.hash_code()] = value;
	}

	const std::unordered_map<size_t, Value>& GetRegistry()
	{
		return m_registry;
	}

private:

	std::unordered_map<size_t, Value> m_registry;

	ComponentFactory() {}
	~ComponentFactory() {}
public:
	static ComponentFactory& Instance()
	{
		static ComponentFactory inst;
		return inst;
	}
};
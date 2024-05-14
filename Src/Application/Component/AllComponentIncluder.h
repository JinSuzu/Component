#pragma once

#include "Component.h"
#include "BoxCollision/BoxCollision.h"
#include "DrawTex/DrawTex.h"
#include "Transform/Transform.h"
#include "Rigidbody/Rigidbody.h"
#include "AddRotation/AddRotaion.h"


#define FNCOMPONENT(Tag)											\
[]()																\
{																	\
	return std::shared_ptr<Cp_##Tag>(new Cp_##Tag);					\
}

static std::size_t ComponentHash(const Component* comp)
{
	return std::hash<std::type_index>()(std::type_index(typeid(*comp)));
}

static std::size_t ComponentHash(Component comp)
{
	return std::hash<std::type_index>()(std::type_index(typeid(comp)));
}

class ComponentMap
{
	std::map<unsigned int, std::function<std::shared_ptr<Component>()>> m_createList =
	{
		//独立コンポ
		{ComponentID::BoxCollision		,FNCOMPONENT(BoxCollision)},
		{ComponentID::DrawTex			,FNCOMPONENT(DrawTex)},
		{ComponentID::Rigidbody			,FNCOMPONENT(Rigidbody)},
		{ComponentID::Transform			,FNCOMPONENT(Transform)},

		//依存コンポ
		{ComponentID::AddRotation		,FNCOMPONENT(AddRotation)},
	};


	std::map<unsigned int, std::string>m_tagMap;
	std::map<std::string, unsigned int>m_bitMap;
	std::unordered_map<std::size_t, unsigned int>m_typeMap;

public:
	std::shared_ptr<Component> createFind(unsigned int _id)	
	{ 
		auto temp = m_createList.find(_id);
		assert(temp != m_createList.end() && "Mapに入ってないよ");
		return temp->second();

	}

	auto bitFind(std::string _tag)			{ return m_bitMap.find(PickName(_tag,'_')); }
	auto bitBegin()							{ return m_bitMap.begin(); }
	auto bitEnd()							{ return m_bitMap.end(); }
	std::string GetTag(unsigned int _id)	{ return m_tagMap.find(_id)->second; }
	int GetCompoNum()						{ return std::log2((double)ComponentID::MaxID); }

	static ComponentMap& Instance()
	{
		static ComponentMap inst;
		return inst;
	}
private:
	ComponentMap() 
	{
		for (auto&& map : m_createList) 
		{
			auto contents = map.second();
			std::string tag = PickName(typeid(*contents.get()).name(),'_');
			m_tagMap[map.first] = tag;
			m_bitMap[tag] = map.first;

			m_typeMap[ComponentHash(map.second().get())] = map.first;
		}
	};

};
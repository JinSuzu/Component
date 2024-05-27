#pragma once

#include "Component.h"
#include "BoxCollision/BoxCollision.h"
#include "Draw/Draw.h"
#include "Rigidbody/Rigidbody.h"
#include "AddRotation/AddRotaion.h"
#include "Controller/Controller.h"
#include "Camera/Camera.h"
#include "TransformLimit/TransformLimit.h"
#include "SquarePolygonAnimetion/SquarePolygonAnimetion.h"


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
		{ComponentID::BoxCollision				,FNCOMPONENT(BoxCollision)},
		{ComponentID::Draw						,FNCOMPONENT(Draw)},
		{ComponentID::Rigidbody					,FNCOMPONENT(Rigidbody)},

		//依存コンポ
		{ComponentID::AddRotation				,FNCOMPONENT(AddRotation)},
		{ComponentID::Controller				,FNCOMPONENT(Controller)},
		{ComponentID::Camera					,FNCOMPONENT(Camera)},
		{ComponentID::TransformLimit			,FNCOMPONENT(TransformLimit)},
		{ComponentID::SquarePolygonAnimetion	,FNCOMPONENT(SquarePolygonAnimetion)}
	};


	std::map<unsigned int, std::string>m_tagMap;
	std::map<std::string, unsigned int>m_bitMap;
	std::map<std::size_t, unsigned int>m_typeMap;

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
	unsigned int ImGuiComponentSet()
	{
		ImGui::SeparatorText("ComponentSet");
		static unsigned int state;
		ImGui::BeginChild("##ComponentSet" , ImVec2(350, 100), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
		{
			if (ImGui::BeginTable("##ComponentSet", 3, ImGuiTableFlags_BordersInnerV))
			{
				for (int i = 0; i < GetCompoNum(); i++)
				{
					bool flg = state & (1 << i);
					ImGui::TableNextColumn();
					ImGui::Checkbox(GetTag(1 << i).c_str(), &flg);

					if (flg)state |= (1 << i);
					if (!flg)state &= ~(1 << i);
				}
				ImGui::EndTable();
			}
		}
		ImGui::EndChild();
		return state;
	}

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
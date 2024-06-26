﻿#pragma once

#include "Component.h"
#include "BoxCollision/BoxCollision.h"

#include "Texture/Texture.h"
#include "ModelData/ModelData.h"
#include "SquarePolygon/SquarePolygon.h"
#include "Rigidbody/Rigidbody.h"

#include "AddRotation/AddRotaion.h"
#include "Controller/Controller.h"
#include "Camera/Camera.h"
#include "TransformLimit/TransformLimit.h"
#include "TransFormInherit/TransFormInherit.h"
#include "Collider/Collider.h"
#include "Launcher/Launcher.h"
#include "Bullet/Bullet.h"
#include "Player/Player.h"
#include "Particle/Particle.h"
#include "HitResult/HitResultBlock/HitResultBlock.h"
#include "HitResult/HitResultReflect/HitResultReflect.h"
#include "HitResult/HitResultScene/HitResultScene.h"
#include "HitResult/HitResultDestroy/HitResultDestroy.h"
#include "SceneActive/SceneActive.h"
#include "SinCurveAlpha/SinCurveAlpha.h"
#include "EnterToScene/EnterToScene.h"
#include "MoveLocus/MoveLocus.h"
#include "MoveLimit/MoveLimit.h"
#include "TimerDisplay/TimerDisplay.h"
#include "SceneChanger/SceneChanger.h"
#include "Counter/Counter.h"
#include "ChildCnt/ChildCnt.h"
#include "ScoreCounter/ScoreCounter.h"


#define FNCOMPONENT(Tag)											\
[]()																\
{																	\
	return std::shared_ptr<Tag>(new Tag);							\
}

class RegisterComponent
{
	std::map<std::string, std::function<std::shared_ptr<Component>()>> m_createMap;
	std::map<UINT, std::string> m_bitNameMap;

	RegisterComponent()
	{
		auto Register = [&](std::function<std::shared_ptr<Component>()> _fn)
			{
				UINT bit = 1 << m_createMap.size();
				m_createMap[PickName(typeid(*_fn().get()).name(), '_')] = _fn;
				m_bitNameMap[bit] = PickName(typeid(*_fn().get()).name(), '_');
			};

		Register(FNCOMPONENT(Cp_Camera));
		Register(FNCOMPONENT(Cp_Collider));
		Register(FNCOMPONENT(Cp_BoxCollision));

		Register(FNCOMPONENT(Cp_Rigidbody));
		{
			Register(FNCOMPONENT(Cp_Bullet));
			Register(FNCOMPONENT(Cp_Controller));
			Register(FNCOMPONENT(Cp_Player));

			//HitResult
			{
				Register(FNCOMPONENT(Cp_HitResultBlock));
				Register(FNCOMPONENT(Cp_HitResultReflect));
				Register(FNCOMPONENT(Cp_HitResultScene));
				Register(FNCOMPONENT(Cp_HitResultDestroy));
				Register(FNCOMPONENT(Cp_MoveLocus));
				Register(FNCOMPONENT(Cp_MoveLimit));
			}
		}

		Register(FNCOMPONENT(Cp_ModelData));
		Register(FNCOMPONENT(Cp_SquarePolygon)); 
		{
			Register(FNCOMPONENT(Cp_SinCurveAlpha));
			Register(FNCOMPONENT(Cp_TimerDisplay));
			Register(FNCOMPONENT(Cp_Counter));
		
		}
		Register(FNCOMPONENT(Cp_Texture));

		Register(FNCOMPONENT(Cp_AddRotation));
		Register(FNCOMPONENT(Cp_Launcher));
		Register(FNCOMPONENT(Cp_Particle));
		Register(FNCOMPONENT(Cp_TransformLimit));
		Register(FNCOMPONENT(Cp_TransFormInherit));

		Register(FNCOMPONENT(Cp_SceneActive));
		Register(FNCOMPONENT(Cp_EnterToScene));
		Register(FNCOMPONENT(Cp_SceneChanger));

		//かなりゴミ
		Register(FNCOMPONENT(Cp_ChildCnt));
		Register(FNCOMPONENT(Cp_ScoreCounter));
	};
public:
	/*UINT NameToBit(std::string _tag) { return m_nameToBitMap.find(PickName(_tag, '_'))->second; }
	std::string GetName(unsigned int _id) { return m_bitToNameMap.find(_id)->second; }*/
	int GetCompoNum() { return m_createMap.size(); }

	std::shared_ptr<Component>CreateComponent(std::string _name)
	{
		auto temp = m_createMap.find(_name);
		assert(temp != m_createMap.end() && "Mapに入ってないよ");

		std::shared_ptr<Component> cmp = temp->second();
		cmp->SetIDName(temp->first);
		return cmp;
	}
	std::shared_ptr<Component>CreateComponent(UINT _name)
	{
		auto temp = m_bitNameMap.find(_name);
		assert(temp != m_bitNameMap.end() && "Mapに入ってないよ");

		std::shared_ptr<Component> cmp = m_createMap[temp->second]();
		cmp->SetIDName(temp->second);
		return cmp;
	}

	unsigned int ImGuiComponentSet()
	{
		ImGui::SeparatorText("ComponentSet");
		static unsigned int state;
		ImGui::BeginChild("##ComponentSet", ImVec2(450, 100), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);
		{
			if (ImGui::BeginTable("##ComponentSet", 3, ImGuiTableFlags_BordersInnerV))
			{
				for (auto& map : m_bitNameMap)
				{
					ImGui::TableNextColumn();
					bool flg = state & map.first;
					ImGui::Checkbox(map.second.c_str(), &flg);

					if (flg)state |= map.first;
					if (!flg)state &= ~map.first;
				}
				ImGui::EndTable();
			}
		}
		ImGui::EndChild();
		return state;
	}
	std::shared_ptr<Component> ImGuiAddComponent()
	{
		if (ImGui::SmallButton("AddComponent"))ImGui::OpenPopup("Components");
		std::shared_ptr<Component> cmp;
		if (!ImGui::BeginPopup("Components"))return cmp;

		ImGui::SeparatorText("Component");
		auto it = m_createMap.begin();

		while (it != m_createMap.end())
		{
			if (ImGui::MenuItem(it->first.c_str()))
			{
				cmp = CreateComponent(it->first);
				break;
			}
			++it;
		}
		ImGui::EndPopup();
		return cmp;
	}


	static RegisterComponent& Instance()
	{
		static RegisterComponent inst;
		return inst;
	}

};
//static std::size_t ComponentHash(const Component* comp)
//{
//	return std::hash<std::type_index>()(std::type_index(typeid(*comp)));
//}
//static std::size_t ComponentHash(Component comp)
//{
//	return std::hash<std::type_index>()(std::type_index(typeid(comp)));
//}
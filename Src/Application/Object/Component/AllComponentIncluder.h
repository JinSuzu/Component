#pragma once

#include "Component.h"
#include "BoxCollision/BoxCollision.h"

#include "Texture/Texture.h"
#include "SquarePolygon/SquarePolygon.h"
#include "ModelData/ModelData.h"
#include "ModelWork/ModelWork.h"

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
#include "BuildCamera/BuildCamera.h"


#define FNCOMPONENT(Tag)											\
[]()																\
{																	\
	return std::shared_ptr<Tag>(new Tag);							\
}

inline void RegisterComponentInit()
{
	ComponentFactory::Instance().RegisterComponent<Cp_Camera>();
	ComponentFactory::Instance().RegisterComponent <Cp_BuildCamera>();
	ComponentFactory::Instance().RegisterComponent <Cp_Collider>();
	ComponentFactory::Instance().RegisterComponent <Cp_BoxCollision>();

	//Regidbody
	ComponentFactory::Instance().RegisterComponent <Cp_Rigidbody>();
	ComponentFactory::Instance().RegisterComponent <Cp_Bullet>();
	ComponentFactory::Instance().RegisterComponent <Cp_Controller>();
	ComponentFactory::Instance().RegisterComponent <Cp_Player>();
	ComponentFactory::Instance().RegisterComponent <Cp_MoveLocus>();
	ComponentFactory::Instance().RegisterComponent <Cp_MoveLimit>();

	//HitResult
	ComponentFactory::Instance().RegisterComponent <Cp_HitResultBlock>();
	ComponentFactory::Instance().RegisterComponent <Cp_HitResultReflect>();
	ComponentFactory::Instance().RegisterComponent <Cp_HitResultScene>();
	ComponentFactory::Instance().RegisterComponent <Cp_HitResultDestroy>();

	//Render
	ComponentFactory::Instance().RegisterComponent <Cp_Texture>();
	ComponentFactory::Instance().RegisterComponent <Cp_SquarePolygon>();
	ComponentFactory::Instance().RegisterComponent <Cp_ModelData>();
	ComponentFactory::Instance().RegisterComponent <Cp_ModelWork>();
	{	
		//Render依存
		ComponentFactory::Instance().RegisterComponent <Cp_TimerDisplay>();
		ComponentFactory::Instance().RegisterComponent <Cp_Counter>();
	}

	ComponentFactory::Instance().RegisterComponent <Cp_SinCurveAlpha>();
	ComponentFactory::Instance().RegisterComponent <Cp_AddRotation>();
	ComponentFactory::Instance().RegisterComponent <Cp_Launcher>();
	ComponentFactory::Instance().RegisterComponent <Cp_TransformLimit>();
	ComponentFactory::Instance().RegisterComponent <Cp_TransFormInherit>();
	ComponentFactory::Instance().RegisterComponent <Cp_SceneActive>();
	ComponentFactory::Instance().RegisterComponent <Cp_EnterToScene>();
	ComponentFactory::Instance().RegisterComponent <Cp_SceneChanger>();

	//かなりゴミ
	ComponentFactory::Instance().RegisterComponent <Cp_ChildCnt>();
	ComponentFactory::Instance().RegisterComponent <Cp_ScoreCounter>();
}
/*
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
*/
//static std::size_t ComponentHash(const Component* comp)
//{
//	return std::hash<std::type_index>()(std::type_index(typeid(*comp)));
//}
//static std::size_t ComponentHash(Component comp)
//{
//	return std::hash<std::type_index>()(std::type_index(typeid(comp)));
//}
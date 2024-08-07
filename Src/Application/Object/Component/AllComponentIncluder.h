#pragma once


#include "Camera/BuildCameraComponent/BuildCameraComponent.h"
#include "Camera/CameraControllerComponent/CameraControllerComponent.h"

#include "CreateObjectComponent/CreateObjectComponent.h"

#include "TrasformInterface/AddRotationComponent/AddRotaionComponent.h"
#include "TrasformInterface/TransFormInheritComponent/TransFormInheritComponent.h"
#include "TrasformInterface/TransformLimitComponent/TransformLimitComponent.h"
#include "TrasformInterface/PositionLimitComponent/PositionLimitComponent.h"
#include "TrasformInterface/RotationLimitComponent/RotationLimitComponent.h"
#include "TrasformInterface/ScaleLimitComponent/ScaleLimitComponent.h"

#include "ChildsInterface/FindChildComponent/FindChildComponent.h"

#include "SceneInterface/EnterToSceneComponent/EnterToSceneComponent.h"
#include "SceneInterface/SceneActiveComponent/SceneActiveComponent.h"
#include "SceneInterface/SceneChangerComponent/SceneChangerComponent.h"

#include "RegidbodyInterface/ControllerComponent/ControllerComponent.h"
#include "RegidbodyInterface/MoveLimitComponent/MoveLimitComponent.h"
#include "RegidbodyInterface/MoveLocusComponent/MoveLocusComponent.h"

#include "RenderInterface/SinCurveAlphaComponent/SinCurveAlphaComponent.h"
#include "RenderInterface/TimerDisplayComponent/TimerDisplayComponent.h"

#include "Counter/ChildCntComponent/ChildCntComponent.h"
#include "Counter/CounterComponent/CounterComponent.h"
#include "Counter/ScoreCounterComponent/ScoreCounterComponent.h"

#include "HitResult/HitResultBlockComponent/HitResultBlockComponent.h"
#include "HitResult/HitResultDestroyComponent/HitResultDestroyComponent.h"
#include "HitResult/HitResultReflectComponent/HitResultReflectComponent.h"
#include "HitResult/HitResultSceneComponent/HitResultSceneComponent.h"

#include "ParticleComponent/ParticleComponent.h"
#include "PlayerComponent/PlayerComponent.h"
#include "BoxCollisionComponent/BoxCollisionComponent.h"
#include "BulletComponent/BulletComponent.h"
#include "LauncherComponent/LauncherComponent.h"

inline void RegisterComponentInit()
{
	ComponentFactory::Instance().RegisterComponent <CameraComponent>();
	{
		ComponentFactory::Instance().RegisterComponent <BuildCameraComponent>();
		ComponentFactory::Instance().RegisterComponent <CameraControllerComponent>();
	}

	ComponentFactory::Instance().RegisterComponent <CreateObjectComponent>();

	ComponentFactory::Instance().RegisterComponent <ColliderComponent>();
	ComponentFactory::Instance().RegisterComponent <BoxCollisionComponent>();

	//トランスフォームいじるやつら
	{
		ComponentFactory::Instance().RegisterComponent <AddRotationComponent>();
		ComponentFactory::Instance().RegisterComponent <TransformLimitComponent>();
		ComponentFactory::Instance().RegisterComponent <PositionLimitComponent>();
		ComponentFactory::Instance().RegisterComponent <RotationLimitComponent>();
		ComponentFactory::Instance().RegisterComponent <ScaleLimitComponent>();
	}

	//Childsいじるやつら
	{
		ComponentFactory::Instance().RegisterComponent <ChildCntComponent>();
	}

	//Regidbodyいじるやつら
	ComponentFactory::Instance().RegisterComponent <RigidbodyComponent>();
	ComponentFactory::Instance().RegisterComponent <BulletComponent>();
	ComponentFactory::Instance().RegisterComponent <ControllerComponent>();
	ComponentFactory::Instance().RegisterComponent <MoveLocusComponent>();
	ComponentFactory::Instance().RegisterComponent <MoveLimitComponent>();

	//Scene管理
	{
		ComponentFactory::Instance().RegisterComponent <SceneActiveComponent>();
		ComponentFactory::Instance().RegisterComponent <EnterToSceneComponent>();
		ComponentFactory::Instance().RegisterComponent <SceneChangerComponent>();
	}

	//HitResult
	ComponentFactory::Instance().RegisterComponent <HitResultBlockComponent>();
	ComponentFactory::Instance().RegisterComponent <HitResultReflectComponent>();
	ComponentFactory::Instance().RegisterComponent <HitResultSceneComponent>();
	ComponentFactory::Instance().RegisterComponent <HitResultDestroyComponent>();

	//Render
	ComponentFactory::Instance().RegisterComponent <TextureComponent>();
	ComponentFactory::Instance().RegisterComponent <SquarePolygonComponent>();
	ComponentFactory::Instance().RegisterComponent <ModelDataComponent>();
	ComponentFactory::Instance().RegisterComponent <ModelWorkComponent>();
	{
		//Render依存
		ComponentFactory::Instance().RegisterComponent <TimerDisplayComponent>();
		ComponentFactory::Instance().RegisterComponent <CounterComponent>();
		ComponentFactory::Instance().RegisterComponent <SinCurveAlphaComponent>();//微妙
	}


	//Player
	{
		ComponentFactory::Instance().RegisterComponent <PlayerComponent>();

	}

	//その他
	{
		ComponentFactory::Instance().RegisterComponent <LauncherComponent>();
	}

	//かなりゴミ
	ComponentFactory::Instance().RegisterComponent <ChildCntComponent>();
	ComponentFactory::Instance().RegisterComponent <ScoreCounterComponent>();
}
#pragma once

class GameObject;
class Component;
class Cp_Collider;


class GameObjectManager
{
public:
	GameObjectManager(){}
	~GameObjectManager() {}

	void PreUpdate();
	void Update();
	void PostUpdate();
	
	void ImGuiUpdate();
	void ImGuiCreateObject();
	static void ImGuiGameObject(std::weak_ptr<GameObject> _object);
	
	void Load(std::string _path);
	void Release(std::string _path);

	void AddColliderList(std::weak_ptr<Cp_Collider> _add) { m_colliderList.push_back(_add); }
	bool RayHit(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool RayHit(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool RayHit(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	static std::shared_ptr<GameObject> CreateObject(std::string _tag,std::weak_ptr<GameObject> _parent = std::weak_ptr<GameObject>(), bool bPush = true);
	static std::shared_ptr<GameObject> CreateObject(nlohmann::json _json, std::weak_ptr<GameObject> _parent = std::weak_ptr<GameObject>(), bool bPush = true);

	void AddObject(std::shared_ptr<GameObject> _add) { m_objectList.push_back(_add); }
private:
	std::list<std::shared_ptr<GameObject>>	m_objectList;
	std::list<std::weak_ptr<Cp_Collider>>	m_colliderList;


	void LoadJson(std::string _path, bool _bOrigin = true);
public:
	static std::string GetGameObjectPath() { return "GameObject/"; };
	static std::string GetGameObjectSetPath() { return "GameObject/Set/"; }


	static std::weak_ptr<GameObject>& EditObject() 
	{
		static std::weak_ptr<GameObject> editObject;
		return editObject;
	};
};

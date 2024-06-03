#pragma once

class GameObject;
class Component;
class Cp_Collider;
enum  ComponentID;


class GameObjectManager
{
public:
	GameObjectManager(){}
	~GameObjectManager() {}

	void PreUpdate();
	void Update();
	void PostUpdate();
	
	void ImGuiUpdate();
	
	void Load(std::string _path);
	void Release(std::string _path);

	void AddColliderList(std::weak_ptr<Cp_Collider> _add) { m_colliderList.push_back(_add); }
	bool RayHit(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool RayHit(const KdCollider::BoxInfo& targetBox, std::list<KdCollider::CollisionResult>* pResults);
	bool RayHit(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	//単品GameObject用Json専
	static std::shared_ptr<GameObject> CreateObject(std::string _tag, bool bPush = true);
	//階層構造のあるGameObject用Json専
	static std::shared_ptr<GameObject> CreateObject(nlohmann::json _json, bool bPush = true);
private:
	std::list<std::shared_ptr<GameObject>>	m_objectList;
	std::list<std::weak_ptr<Cp_Collider>>	m_colliderList;


	void LoadJson(std::string _path, bool _bOrigin = true);
	void ImGuiCreateObject(bool bOrigin = false);
public:
	static std::string GetGameObjectPath() { return "GameObject/"; };
	static std::string GetGameObjectSetPath() { return "GameObject/Set/"; }

	static std::shared_ptr<Component> ToComponent(unsigned int _id);
	static std::string ToTag(unsigned int _id);
	static unsigned int ToID(std::string _tag);

	static void ImGuiAddComponent(std::weak_ptr<GameObject> _object);
	static std::weak_ptr<GameObject>& EditObject() 
	{
		static std::weak_ptr<GameObject> editObject;
		return editObject;
	};
};

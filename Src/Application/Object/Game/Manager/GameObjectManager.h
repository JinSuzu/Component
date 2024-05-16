#pragma once

class GameObject;
class Component;
enum  ComponentID;


class GameObjectManager
{
public:
	~GameObjectManager() {}

	void Draw();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void ImGuiUpdate();
	void ImGuiCreateObject(std::weak_ptr<GameObject> _parent = std::weak_ptr<GameObject>());
	void ImGuiAddComponent(std::weak_ptr<GameObject> _object);

	void Init();
	void Release();

	std::shared_ptr<Component> ToComponent(unsigned int _id);
	std::string ToTag(unsigned int _id);
	unsigned int ToID(std::string _tag);

	std::shared_ptr<GameObject> CreateObject(std::string _tag,bool flg = true);
private:
	std::list<std::shared_ptr<GameObject>>	m_obList;

	GameObjectManager() { Init(); };
public:
	static GameObjectManager& Instance() {
		static GameObjectManager inst;
		return inst;
	}
};

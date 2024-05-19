#pragma once

class GameObject;
class Component;
enum  ComponentID;


class GameObjectManager
{
private:
public:
	GameObjectManager() {}
	~GameObjectManager() {}

	void Draw();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void ImGuiUpdate();
	void Init();
	void Release();

	static std::shared_ptr<Component> ToComponent(unsigned int _id);
	static std::string ToTag(unsigned int _id);
	static unsigned int ToID(std::string _tag);

	static std::string GetGameObjectPath() { return "GameObject/"; };
	static std::string GetGameObjectSetPath() { return "GameObject/Set/"; }

private:
	struct GameObjectFamily
	{
		operator bool() { return this != nullptr; }

		GameObjectFamily(std::shared_ptr<GameObject> _obj)
			:parent(_obj)
		{}

		GameObjectFamily(nlohmann::json::iterator json, GameObjectFamily* _parent = nullptr)
			:parent(GameObjectManager::CreateObject((*json)["Parent"], _parent))
		{
			auto name = (*json)["Childs"].begin();
			while (name != (*json)["Childs"].end())
			{
				childs.push_back(GameObjectFamily(name, this));
				name++;
			}
		}

		~GameObjectFamily() { if (GameObjectManager::EditObject() == this)delete GameObjectManager::EditObject(); }


		void Draw();

		void PreUpdate();
		void Update();
		void PostUpdate();

		void ImGuiUpdate(int num);
		void ImGuiOpenOption(int num);

		nlohmann::json GetJson();

		std::shared_ptr<GameObject>						parent = nullptr;

		std::list<GameObjectFamily>	childs = std::list<GameObjectFamily>();
	};
	std::list<GameObjectFamily>		m_obList;

	//単品GameObject用Json専
	std::shared_ptr<GameObject> CreateObject(std::string _tag, GameObjectFamily* _family,bool bPush = true);
	//階層構造のあるGameObject用Json専
	static std::shared_ptr<GameObject> CreateObject(nlohmann::json _json, GameObjectFamily* _family);

	
	void ImGuiCreateObject(bool bOrigin = false);

	void LoadJson(std::string _path,bool _bOrigin = true);
public:
	static void ImGuiAddComponent(std::weak_ptr<GameObject> _object);
	static GameObjectFamily* EditObject(GameObjectFamily* _edit = nullptr)
	{
		static GameObjectFamily* inst;
		if(_edit)inst = _edit;
		return inst;
	};
};

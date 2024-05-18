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
	void ImGuiCreateObject(bool bOrigin = false);
	void ImGuiAddComponent(std::weak_ptr<GameObject> _object);

	void Init();
	void Release();

	std::shared_ptr<Component> ToComponent(unsigned int _id);
	std::string ToTag(unsigned int _id);
	unsigned int ToID(std::string _tag);

	std::shared_ptr<GameObject> CreateObject(std::string _tag, bool flg = false);
private:
	struct GameObjectFamily
	{
		operator bool() { return this != nullptr; }

		GameObjectFamily(std::shared_ptr<GameObject> _obj)
			:parent(_obj)
		{}

		GameObjectFamily(nlohmann::json::iterator json, GameObjectFamily* _parent = nullptr)
			:parent(GameObjectManager::Instance().CreateObject((*json)["parent"], _parent, false))
		{
			auto name = (*json)["Childs"].begin();
			while (name != (*json)["Childs"].end())
			{
				childs.push_back(GameObjectFamily(name, this));
				name++;
			}
		}

		~GameObjectFamily() { if(GameObjectManager::Instance().m_editObject == this)GameObjectManager::Instance().m_editObject = nullptr; }


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
	std::shared_ptr<GameObject> CreateObject(std::string _tag, GameObjectFamily* _family,bool bPush = true);
	std::shared_ptr<GameObject> CreateObject(nlohmann::json _json, GameObjectFamily* _family,bool bPush = true);
	
	GameObjectManager() { };
public:
	GameObjectFamily* m_editObject = nullptr;

	static GameObjectManager& Instance() {
		static GameObjectManager inst;
		return inst;
	}

};

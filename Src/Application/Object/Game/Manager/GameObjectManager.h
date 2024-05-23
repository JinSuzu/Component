#pragma once

class GameObject;
class Component;
enum  ComponentID;


class GameObjectManager
{
public:
	GameObjectManager() {}
	~GameObjectManager() {}

	void PreUpdate();
	void Update();
	void PostUpdate();

	#pragma region void Draw
	void PreDraw()						{ for (auto& family : m_obList) family.PreDraw(); }
	void GenerateDepthMapFromLight()	{ for (auto& family : m_obList) family.GenerateDepthMapFromLight(); }
	void DrawLit()						{ for (auto& family : m_obList) family.DrawLit(); }
	void DrawUnLit()					{ for (auto& family : m_obList) family.DrawUnLit(); }
	void DrawBright()					{ for (auto& family : m_obList) family.DrawBright(); }
	void DrawSprite()					{ for (auto& family : m_obList) family.DrawSprite(); }
#pragma endregion

	void ImGuiUpdate();
	void Load(std::string _path);
	void Release(std::string _path);

	static std::string GetGameObjectPath() { return "GameObject/"; };
	static std::string GetGameObjectSetPath() { return "GameObject/Set/"; }

private:
	struct GameObjectFamily
	{
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

		~GameObjectFamily() { if (GameObjectManager::EditObject() == this)GameObjectManager::EditObject(nullptr, true); }

		void PreUpdate();
		void Update();
		void PostUpdate();

	#pragma region	void Draw
		void PreDraw();
		void GenerateDepthMapFromLight();
		void DrawLit();
		void DrawUnLit();
		void DrawBright();
		void DrawSprite();

		void ImGuiUpdate(int num);
		void ImGuiOpenOption(int num);
#pragma endregion

		nlohmann::json GetJson();

		std::shared_ptr<GameObject>						parent = nullptr;

		std::list<GameObjectFamily>	childs = std::list<GameObjectFamily>();
	};
	std::list<GameObjectFamily>		m_obList;

	//単品GameObject用Json専
	std::shared_ptr<GameObject> CreateObject(std::string _tag, GameObjectFamily* _family = nullptr,bool bPush = true);
	//階層構造のあるGameObject用Json専
	static std::shared_ptr<GameObject> CreateObject(nlohmann::json _json, GameObjectFamily* _family = nullptr);

	void LoadJson(std::string _path,bool _bOrigin = true);
	void ImGuiCreateObject(bool bOrigin = false);
public:
	static std::shared_ptr<Component> ToComponent(unsigned int _id);
	static std::string ToTag(unsigned int _id);
	static unsigned int ToID(std::string _tag);

	static void ImGuiAddComponent(std::weak_ptr<GameObject> _object);
	static GameObjectFamily* EditObject(GameObjectFamily* _edit = nullptr,bool bDelete = false)
	{
		static GameObjectFamily* inst = nullptr;
		if (_edit || bDelete)inst = _edit;
		return inst;
	};
};

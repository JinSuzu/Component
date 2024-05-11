#pragma once

class Object;
class Component;
enum  ComponentID;


class ObjectManager
{
public:
	~ObjectManager() {}

	void Draw();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void ImGuiUpdate();

	void Init();
	void Relese();

	std::shared_ptr<Component> ToComponent(unsigned int _id);
	std::string ToTag(unsigned int _id);
	unsigned int ToID(std::string _tag);

	std::shared_ptr<Object> CreateObject(std::string _tag,bool flg = true);

	void UpdateRange(std::list<std::shared_ptr<Object>>::iterator start, std::list<std::shared_ptr<Object>>::iterator end);
private:
	std::list<std::shared_ptr<Object>>	m_obList;

	ObjectManager() { Init(); };
public:
	static ObjectManager& Instance() {
		static ObjectManager inst;
		return inst;
	}
};

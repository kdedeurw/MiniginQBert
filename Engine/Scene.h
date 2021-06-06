#pragma once
#include <string>
#include <vector>

class GameObject;
class Scene final
{
public:
	virtual ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	GameObject* CreateGameObject();
	void RemoveGameObject(GameObject* pGameObject, bool isDelete = true);

	virtual void Initialize();
	virtual void Update();
	virtual void Render() const;
	virtual void PostRender() const;

	bool operator==(const Scene& other) const;
	bool operator==(Scene* pOther) const;

private:
	friend class GlobalMemoryPools;
	explicit Scene(std::string name);

	void SortGameObjects();
	static unsigned int m_IdCounter;

	friend class SceneManager;
	std::string m_Name;
	std::vector<GameObject*> m_GameObjects;
};
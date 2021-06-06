#pragma once
#include <vector>

class Component;
class TransformComponent;
class Scene;
class GameObject final
{
public:
	friend class GlobalMemoryPools;
	explicit GameObject();
	virtual ~GameObject() noexcept;
	GameObject(const GameObject & other) = delete;
	GameObject(GameObject && other) = delete;
	GameObject& operator=(const GameObject & other) = delete;
	GameObject& operator=(GameObject && other) = delete;

	void Initialize(bool forceInitialize = false);
	void Update();
	void Render() const;
	void PostRender() const;

	TransformComponent& GetTransform() const { return *m_pTransform; }
	GameObject& GetParent() const { return *m_pParent; }
	Scene& GetScene() const { return *m_pScene; }

	const unsigned int GetId() const { return m_Id; }

	void AddComponent(Component* pComponent, bool isInitialize = false);
	template <typename T>
	T* GetComponent() const;
	void RemoveComponent(Component* pComponent, bool isDelete = false);

	void AddChildObject(GameObject* pChild, bool isInitialize = true);
	const std::vector<GameObject*>& GetChildren() { return m_pChildren; }
	void RemoveChildObject(GameObject* pChild, bool isDelete = false);

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }
	void SetRenderable(bool isRendered) { m_IsRendered = isRendered; }
	bool IsRendered() const { return m_IsRendered; }

private:
	friend class Scene;
	unsigned int m_Id;
	Scene* m_pScene;

	bool m_IsInitialized, m_IsActive, m_IsRendered;
	TransformComponent* m_pTransform;
	GameObject* m_pParent;

	std::vector<Component*> m_pComponents;
	std::vector<GameObject*> m_pChildren;
};

template <typename T>
T* GameObject::GetComponent() const
{
	const type_info& typeInfo = typeid(T);
	for (Component* pComponent : m_pComponents)
	{
		if (typeid(*pComponent) == typeInfo)
			return static_cast<T*>(pComponent);
	}
	return nullptr;
}
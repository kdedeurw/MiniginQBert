#pragma once
#include <vector>

class Component;
class TransformComponent;
class GameObject final
{
public:
	virtual ~GameObject();
	GameObject(const GameObject & other) = delete;
	GameObject(GameObject && other) = delete;
	GameObject& operator=(const GameObject & other) = delete;
	GameObject& operator=(GameObject && other) = delete;

	virtual void Initialize();
	virtual void Update();
	virtual void Render() const;

	TransformComponent& GetTransform() const { return m_Transform; }
	GameObject& GetParent() const { return *m_pParent; }

	const unsigned int GetId() const { return m_Id; }

	void AddComponent(Component* pComponent);
	template <typename T>
	T* GetComponent() const;
	void RemoveComponent(Component* pComponent);

	void AddChildObject(GameObject* pChild);
	const std::vector<GameObject*>& GetChildren() { return m_pChildren; }
	void RemoveChildObject(GameObject* pChild);

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }
	void SetRenderable(bool isRendered) { m_IsRendered = isRendered; }
	bool IsRendered() const { return m_IsRendered; }

private:
	friend class Scene;
	unsigned int m_Id;

	bool m_IsActive, m_IsRendered;
	TransformComponent& m_Transform;
	GameObject* m_pParent;

	friend class GlobalMemoryPools;
	explicit GameObject(TransformComponent& transform);

	std::vector<Component*> m_pComponents;
	std::vector<GameObject*> m_pChildren;
};

template <typename T>
T* GameObject::GetComponent() const
{
	for (Component* pComponent : m_pComponents)
	{
		if (typeid(*pComponent) == typeid(T))
			return dynamic_cast<T*>(pComponent);
	}
	return nullptr;
}
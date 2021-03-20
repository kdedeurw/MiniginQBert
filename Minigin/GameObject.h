#pragma once
#include <vector>

enum class TransformMask : BYTE
{
	All = 0,
	NoScale = 1,
	NoRotation = 2,
	NoScaleNoRot = 4,
};

class Transform;
class Component;
class Subject;
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

	//local transform in local space
	//WRITE, this transform is used to store actual transform data
	Transform& GetLocalTransform() const { return m_LocalTransform; };
	//final transform in world space (all parents + own)
	//!READ ONLY!, this transform is used for calculations in world space
	Transform& GetWorldTransform() const { return m_WorldTransform; };
	GameObject& GetParent() const { return *m_pParent; };

	const unsigned int GetId() const { return m_Id; };

	void AddComponent(Component* pComponent);
	template <typename T>
	T* GetComponent() const;
	void RemoveComponent(Component* pComponent);

	void AddChildObject(GameObject* pChild);
	std::vector<GameObject*>& GetChildren() { return m_pChildren; };
	void RemoveChildObject(GameObject* pChild);

	void SetActive(bool isActive) { m_IsActive = isActive; };
	bool IsActive() const { return m_IsActive; };

	void SetTransformMask(TransformMask transformMask) { m_TransformMask = transformMask; };
	TransformMask GetTransformMask() const { return m_TransformMask; };

private:
	friend class Scene;
	unsigned int m_Id;

	bool m_IsActive;
	TransformMask m_TransformMask;
	Transform& m_WorldTransform;
	Transform& m_LocalTransform;
	GameObject* m_pParent;

	friend class GlobalMemoryPools;
	explicit GameObject(Transform& worldTransform, Transform& localTransform);

	std::vector<Component*> m_pComponents;
	std::vector<GameObject*> m_pChildren;

	void CalculateAllMask() const;
	void CalculateNoScaleMask() const;
	void CalculateNoRotationMask() const;
	void CalculateNoScaleNoRotMask() const;
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
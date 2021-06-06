#pragma once

//size of class:
//86x (4 bytes == sizeof(void*)) = 8bytes (4bytes extra for vf ptr)
//64x (8 bytes == sizeof(void*)) = 16bytes (8bytes extra for vf ptr)

class GameObject;
class Component
{
public:
	Component()
		: m_IsInitialized{}
		, m_pGameObject{}
	{}
	virtual ~Component() noexcept
	{
		m_pGameObject = nullptr;
	}
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Initialize(bool forceInitialize = false) = 0;
	virtual void PostInitialize() {};
	virtual void Render() const = 0;
	virtual void PostRender() const {};
	virtual void Update() = 0;
	virtual void PostUpdate() {};

	GameObject* GetGameObject() const { return m_pGameObject; }

protected:
	bool m_IsInitialized;

private:
	friend class GameObject;
	GameObject* m_pGameObject;
};
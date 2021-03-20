#pragma once
#include "SingletonRef.h"
#include <string>
#include <vector>

class Scene;
class SceneManager final : public SingletonRef<SceneManager>
{
public:
	~SceneManager();

	void Initialize();
	void Update();
	void Render() const;

	Scene& CreateScene(const std::string& name, bool isActive = true);
	void ToggleScene(Scene& scene, bool isActive);
	void ToggleScene(const std::string& name, bool isActive);

private:
	friend static SceneManager& SingletonRef<SceneManager>::GetInstance();
	SceneManager();

	std::vector<Scene*> m_pAllScenes;
	std::vector<Scene*> m_pActiveScenes;
};
#pragma once
#include "Component.h"
#include "Vectors.h"
#include <string>

struct SDL_Texture;
class Texture2D;
class Texture2DComponent : public Component
{
public:
	Texture2DComponent();
	virtual ~Texture2DComponent();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	Texture2D* GetTexture2D() const { return m_pTexture; };

	void SetTexture(SDL_Texture* pTexture);
	void SetTexture(const std::string& fileAsset);
	void SetPivot(Vector2 pivot);

protected:
	Texture2D* m_pTexture;
	Vector2 m_Pivot;

};
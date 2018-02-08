#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class CMenuState : public Scene
{
public:
	CMenuState();
	~CMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	TextEntity* textObj[3];
	TextEntity* Play;
	TextEntity* Options;
	TextEntity* Shop;
	double cursorX;
	double cursorY;
	Vector3 cursor;

	Mesh* playIcon;
	Mesh* optionIcon;
	Mesh* shopIcon;

	Mesh* pistol;
};

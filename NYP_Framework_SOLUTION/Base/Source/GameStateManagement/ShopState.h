
#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"
#include "../TextEntity.h"

class SceneManager;
//class TextEntity;
class CShopState : public Scene
{
public:
	CShopState();
	~CShopState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



private:
	FPSCamera camera;
	SpriteEntity* ShopStateBackground;
	TextEntity* Back;


	double cursorX;
	double cursorY;
	Vector3 cursor;
};

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
class COptionState : public Scene
{
public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	//	TextEntity* textObj[3];

	TextEntity* Sensitivity;
	TextEntity* S1;
	TextEntity* S2;
	TextEntity* S3;
	float sensitivity;

	TextEntity* Movement;
	TextEntity* c1;
	TextEntity* c2;
	TextEntity* c3;
	TextEntity* Save;

	std::string movef = "W";
	std::string moveb = "S";
	std::string movel = "A";
	std::string mover = "D";

	Vector3 cursor;
};

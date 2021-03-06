#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "WeaponInfo\Pistol.h"
#include "WeaponInfo\AR.h"
#include "SpriteEntity.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	//SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[3];
	Light* lights[2];
	TextEntity* Left[3];
	TextEntity* Right[3];
	TextEntity* Top[1];

	GenericEntity* theCube;
	CEnemy* theEnemy;
	CEnemy* theEnemy1;
	CEnemy* theEnemy2;
	CEnemy* parts;
	CEnemy* parts1;
	Mesh* pistol;
	SpriteEntity* rifle;

	//Timer
	double timer;

//	static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif
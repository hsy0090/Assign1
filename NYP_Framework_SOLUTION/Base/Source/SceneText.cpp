#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"
#include "RenderHelper.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "Waypoint\WaypointManager.h"
#include "../Lua/LuaInterface.h"

#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CWaypointManager::GetInstance()->DropInstance();
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	/*
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");
	
	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	*/

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//mud1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//mud2.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	pistol = MeshBuilder::GetInstance()->GenerateQuad("pistolicon", Color(1, 1, 1), 1.f);
	pistol->textureID = LoadTGA("Image//pistol_icon.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);

	//assign models
	MeshBuilder::GetInstance()->GenerateOBJ("Cottage", "OBJ//COTTAGE.obj");
	MeshBuilder::GetInstance()->GetMesh("Cottage")->textureID = LoadTGA("Image//COTTAGE.tga");


	//tank
	//body
	MeshBuilder::GetInstance()->GenerateOBJ("TankBody", "OBJ//TankBody.obj");
	MeshBuilder::GetInstance()->GetMesh("TankBody")->textureID = LoadTGA("Image//TankPaint2.tga");
	//turret
	MeshBuilder::GetInstance()->GenerateOBJ("TankTurret", "OBJ//TankTurret.obj");
	MeshBuilder::GetInstance()->GetMesh("TankTurret")->textureID = LoadTGA("Image//TankPaint2.tga");
	//gun
	MeshBuilder::GetInstance()->GenerateOBJ("TankGun", "OBJ//TankGun.obj");
	MeshBuilder::GetInstance()->GetMesh("TankGun")->textureID = LoadTGA("Image//TankPaint2.tga");


	//LOD model Crates
	//most detailed
	MeshBuilder::GetInstance()->GenerateOBJ("Crate", "OBJ//Crate.obj");
	MeshBuilder::GetInstance()->GetMesh("Crate")->textureID = LoadTGA("Image//Crate.tga");

	//mid detailed
	MeshBuilder::GetInstance()->GenerateOBJ("Crate2", "OBJ//Crate2.obj");
	MeshBuilder::GetInstance()->GetMesh("Crate2")->textureID = LoadTGA("Image//Crate2.tga");

	//least detailed
	MeshBuilder::GetInstance()->GenerateOBJ("Crate3", "OBJ//Crate3.obj");
	MeshBuilder::GetInstance()->GetMesh("Crate3")->textureID = LoadTGA("Image//Crate3.tga");

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	//house
	GenericEntity* cottage = Create::Entity("Cottage", CLuaInterface::GetInstance()->getVector3Values("CottagePos"), Vector3(15.0f, 15.0f, 15.0f));
	cottage->SetCollider(true);
	cottage->SetAABB(Vector3(30.f, 30.f, 30.f), Vector3(-30.f, -30.f, -30.f));

	//lod crate
	GenericEntity* aCube = Create::Entity("Crate", CLuaInterface::GetInstance()->getVector3Values("Create1Pos"), Vector3(0.2f, 0.2f, 0.2f));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(5.f, 5.f, 5.f), Vector3(-5.f, -5.f, -5.f));
	aCube->InitLOD("Crate", "Crate2", "Crate3");

	GenericEntity* aCube1 = Create::Entity("Crate", CLuaInterface::GetInstance()->getVector3Values("Create2Pos"), Vector3(0.2f, 0.2f, 0.2f));
	aCube1->SetCollider(true);
	aCube1->SetAABB(Vector3(5.f, 5.f, 5.f), Vector3(-5.f, -5.f, -5.f));
	aCube1->InitLOD("Crate", "Crate2", "Crate3");

	// Add the pointer to this new entity to the Scene Graph
	/*CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	if (theNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}

	GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	anotherCube->SetCollider(true);
	anotherCube->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	if (anotherNode == NULL)
	{
		cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	}*/
	
	//Tank===================================================================================
	Vector3 Tankpos = CLuaInterface::GetInstance()->getVector3Values("TankInitPos");
	Vector3 Tanksize = CLuaInterface::GetInstance()->getVector3Values("TankSize");
	GenericEntity* baseCube = Create::Asset("TankBody", Tankpos, Tanksize);
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankBasePos");
	baseNode->ApplyTranslate(CLuaInterface::GetInstance()->GetField("basex"), CLuaInterface::GetInstance()->GetField("basey"), CLuaInterface::GetInstance()->GetField("basez"));

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankBaseUpdate");
	baseMtx->ApplyUpdate(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z"));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankBaseAnimate");
	baseMtx->SetSteps(CLuaInterface::GetInstance()->GetField("Min"),
		CLuaInterface::GetInstance()->GetField("Max"));
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childCube = Create::Asset("TankTurret", Tankpos, Tanksize);
	CSceneNode* childNode = baseNode->AddChild(childCube);
	CUpdateTransformation* aTurnMtx = new CUpdateTransformation();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankTurretUpdate");
	aTurnMtx->ApplyUpdate(CLuaInterface::GetInstance()->GetField("r"), CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z"));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankTurretAnimate");
	aTurnMtx->SetSteps(CLuaInterface::GetInstance()->GetField("Min"),
		CLuaInterface::GetInstance()->GetField("Max"));
	childNode->SetUpdateTransformation(aTurnMtx);

	GenericEntity* grandchildCube = Create::Asset("TankGun", Tankpos, Tanksize);
	CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankGunUpdate");
	aRotateMtx->ApplyUpdate(CLuaInterface::GetInstance()->GetField("r"), CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z"));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "TankGunAnimate");
	aRotateMtx->SetSteps(CLuaInterface::GetInstance()->GetField("Min"),
		CLuaInterface::GetInstance()->GetField("Max"));
	grandchildNode->SetUpdateTransformation(aRotateMtx);
	//=======================================================================================

	// Create a Waypoint inside WaypointManager
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_1");
	int aWayPoint = CWaypointManager::GetInstance()->AddWaypoint(Vector3(	CLuaInterface::GetInstance()->GetField("x"),
																			CLuaInterface::GetInstance()->GetField("y"),
																			CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_2");
	int anotherWaypoint = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint, Vector3(	CLuaInterface::GetInstance()->GetField("x"),
																							CLuaInterface::GetInstance()->GetField("y"),
																							CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_A_3");
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint, Vector3(CLuaInterface::GetInstance()->GetField("x"),
																			CLuaInterface::GetInstance()->GetField("y"),
																			CLuaInterface::GetInstance()->GetField("z")));
	
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_B_1");
	int aWayPoint2 = CWaypointManager::GetInstance()->AddWaypoint(Vector3(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_B_2");
	int anotherWaypoint2 = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint2, Vector3(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_B_3");
	CWaypointManager::GetInstance()->AddWaypoint(anotherWaypoint2, Vector3(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")));

	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_C_1");
	int aWayPoint3 = CWaypointManager::GetInstance()->AddWaypoint(Vector3(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")));
	lua_getglobal(CLuaInterface::GetInstance()->theLuaState, "Waypoint_C_2");
	int anotherWaypoint3 = CWaypointManager::GetInstance()->AddWaypoint(aWayPoint3, Vector3(CLuaInterface::GetInstance()->GetField("x"),
		CLuaInterface::GetInstance()->GetField("y"),
		CLuaInterface::GetInstance()->GetField("z")));

	CWaypointManager::GetInstance()->PrintSelf();

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));
	//Create::Sprite2DObject("pistolicon", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Create a CEnemy instance
	theEnemy = new CEnemy();
	theEnemy->Init();
	theEnemy->Assignpath(3);
	theEnemy->SetPos(CLuaInterface::GetInstance()->getVector3Values("Enemy3Pos"));
	theEnemy->SetScale(Vector3(7, 7, 7));
	theEnemy->SetAABB(Vector3(3.5f, 3.5f, 3.5f), Vector3(-3.5f, -3.5f, -3.5f));


	// Customise the ground entity
	groundEntity->SetPosition(CLuaInterface::GetInstance()->getVector3Values("GroundPos"));
	groundEntity->SetScale(CLuaInterface::GetInstance()->getVector3Values("GroundSize"));
	groundEntity->SetGrids(CLuaInterface::GetInstance()->getVector3Values("GroundGrid"));
	playerInfo->SetTerrain(groundEntity);
	theEnemy->SetTerrain(groundEntity);

	// Create a CEnemy instance
	theEnemy1 = new CEnemy();
	theEnemy1->Init1(0);
	theEnemy1->Assignpath(1);
	theEnemy1->SetPos(CLuaInterface::GetInstance()->getVector3Values("Enemy1Pos"));
	theEnemy1->SetScale(Vector3(7, 7, 7));
	theEnemy1->SetAABB(Vector3(3.5f, 3.5f, 3.5f), Vector3(-3.5f, -3.5f, -3.5f));
	theEnemy1->SetTerrain(groundEntity);
	CSceneNode* headNode1 = CSceneGraph::GetInstance()->AddNode(theEnemy1);
	//parts of ennemy
	parts = new CEnemy();
	parts->Init1(1);
	parts->Assignpath(1);
	parts->SetPos(Vector3(theEnemy1->position.x + 2.f, theEnemy1->position.y - 2.f, theEnemy1->position.z + 15.0f));
	parts->SetScale(Vector3(5, 5, 5));
	parts->SetAABB(Vector3(2.5f, 2.5f, 2.5f), Vector3(-2.5f, -2.5f, -2.5f));
	parts->SetTerrain(groundEntity);
	parts->SetTarget(theEnemy1->GetTarget());
	CSceneNode* part2 = headNode1->AddChild(parts);
	parts = NULL;
	//parts of ennemy
	parts = new CEnemy();
	parts->Init1(1);
	parts->Assignpath(1);
	parts->SetPos(Vector3(theEnemy1->position.x - 2.f, theEnemy1->position.y - 2.f, theEnemy1->position.z + 15.0f));
	parts->SetScale(Vector3(5, 5, 5));
	parts->SetAABB(Vector3(2.5f, 2.5f, 2.5f), Vector3(-2.5f, -2.5f, -2.5f));
	parts->SetTerrain(groundEntity);
	parts->SetTarget(theEnemy1->GetTarget());
	CSceneNode* part3 = headNode1->AddChild(parts);
	parts = NULL;
	theEnemy1 = NULL;

	theEnemy2 = new CEnemy();
	theEnemy2->Init1(0);
	theEnemy2->Assignpath(2);
	theEnemy2->SetPos(CLuaInterface::GetInstance()->getVector3Values("Enemy2Pos"));
	theEnemy2->SetScale(Vector3(7, 7, 7));
	theEnemy2->SetAABB(Vector3(3.5f, 3.5f, 3.5f), Vector3(-3.5f, -3.5f, -3.5f));
	theEnemy2->SetTerrain(groundEntity);
	CSceneNode* headNode2 = CSceneGraph::GetInstance()->AddNode(theEnemy2);
	//parts of ennemy
	parts1 = new CEnemy();
	parts1->Init1(1);
	parts1->Assignpath(2);
	parts1->SetPos(Vector3(theEnemy2->position.x + 2.f, theEnemy2->position.y - 2.f, theEnemy2->position.z + 15.0f));
	parts1->SetScale(Vector3(5, 5, 5));
	parts1->SetAABB(Vector3(2.5f, 2.5f, 2.5f), Vector3(-2.5f, -2.5f, -2.5f));
	parts1->SetTerrain(groundEntity);
	parts1->SetTarget(theEnemy2->GetTarget());
	CSceneNode* part4 = headNode2->AddChild(parts1);
	parts1 = NULL;
	//parts of ennemy
	parts1 = new CEnemy();
	parts1->Init1(1);
	parts1->Assignpath(2);
	parts1->SetPos(Vector3(theEnemy2->position.x - 2.f, theEnemy2->position.y - 2.f, theEnemy2->position.z + 15.0f));
	parts1->SetScale(Vector3(5, 5, 5));
	parts1->SetAABB(Vector3(2.5f, 2.5f, 2.5f), Vector3(-2.5f, -2.5f, -2.5f));
	parts1->SetTerrain(groundEntity);
	parts1->SetTarget(theEnemy2->GetTarget());
	CSceneNode* part5 = headNode2->AddChild(parts1);
	parts1 = NULL;
	theEnemy2 = NULL;

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;

	for (int i = 0; i < 5; ++i)
	{
		Left[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	Left[0]->SetText("HELLO WORLD");

	for (int i = 0; i < 5; ++i)
	{
		Right[i] = Create::Text2DObject("text", Vector3(halfWindowWidth / 10, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	Right[0]->SetText("HELLO WORLD");

	Top[0] = Create::Text2DObject("text", Vector3(-halfWindowWidth / 2, halfWindowHeight - fontSize * 2, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));

	timer = CLuaInterface::GetInstance()->getFloatValue("timer");

	rifle = Create::Sprite2DObject("cube", Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
}

void SceneText::Update(double dt)
{
	timer -= dt;

	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	//Health info
	std::ostringstream ss0;
	ss0.precision(5);
	ss0 << "Health: " << playerInfo->GetHealth();
	Left[0]->SetText(ss0.str());

	//Score info
	std::ostringstream ss3;
	ss3.precision(4);
	ss3 << "Score: " << playerInfo->GetScore();
	Left[1]->SetText(ss3.str());

	//Debug info
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	Left[2]->SetText(ss.str());

	//Timer info
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Timer: " << timer;
	Top[0]->SetText(ss1.str());

	//Primary Weapon Ammo info
	if (playerInfo->GetPWeapon() != 2)
	{
		std::ostringstream ss2;
		ss2.precision(4);
		ss2 << "PAmmo:" << playerInfo->weaponPManager[playerInfo->GetPWeapon()]->GetMagRound() << " / " << playerInfo->weaponPManager[playerInfo->GetPWeapon()]->GetTotalRound();
		Right[0]->SetText(ss2.str());
	}
	else
	{
		std::ostringstream ss2;
		ss2.precision(4);
		ss2 << "PAmmo:" << playerInfo->weaponPManager[playerInfo->GetPWeapon()]->GetMagRound();
		Right[0]->SetText(ss2.str());
	}

	//Secondary Weapon Ammo info
	std::ostringstream ss4;
	ss4.precision(4);
	ss4 << "SAmmo:" << playerInfo->weaponSManager[playerInfo->GetSWeapon()]->GetMagRound() << " / " << playerInfo->weaponSManager[playerInfo->GetSWeapon()]->GetTotalRound();
	Right[1]->SetText(ss4.str());

	if (playerInfo->GetHealth() <= 0)
	{
		CLuaInterface::GetInstance()->saveIntValue("score", playerInfo->GetScore(), false);
		exit(0);
	}

	if (timer <= 0)
	{
		CLuaInterface::GetInstance()->saveIntValue("score", playerInfo->GetScore(), false);
		exit(0);
	}

	
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();

	// Enable blend mode
	//glEnable(GL_BLEND);

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();

	/*MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(400, 300, 1);
	modelStack.Scale(10.f, 10.f, 10.f);
	RenderHelper::RenderMesh(pistol);
	modelStack.PopMatrix();*/

	// Disable blend mode
	//glDisable(GL_BLEND);
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}

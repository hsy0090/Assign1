#include <iostream>
using namespace std;

#include "introstate.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"
#include "MouseController.h"

#include "KeyboardController.h"
#include "SceneManager.h"
#include <sstream>

CMenuState::CMenuState()
{

}
CMenuState::~CMenuState()
{

}

void CMenuState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	pistol = MeshBuilder::GetInstance()->GenerateQuad("pistolicon", Color(1, 1, 1), 1.f);
	pistol->textureID = LoadTGA("Image//pistol_icon.tga");

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;

	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));
	
	//Create::Sprite2DObject("pistolicon", Vector3(halfWindowWidth, halfWindowHeight, 1.0f), Vector3(10.0f, 10.0f, 10.0f));

	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;

	Top[0] = Create::Text2DObject("text", Vector3(halfWindowWidth, halfWindowHeight, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 0.0f, 0.0f));
	

	/*for (int i = 0; i < 2; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");*/
	cursor.SetZero();

	cout << "CMenuState loaded\n" << endl;
}

void CMenuState::Update(double dt)
{
	
	//Test
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Text: " << 1;
	Top[0]->SetText(ss1.str());
	MouseController::GetInstance()->GetMousePosition(cursor.x, cursor.y);
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		std::cout << "c: " << cursor << std::endl;
		std::cout << Top[0]->GetMaxAABB() << std::endl;
		std::cout << Top[0]->GetMinAABB() << std::endl;
	}
	//if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	//MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && 
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, Top[0]->GetMinAABB(), Top[0]->GetMaxAABB()))
	{
		cout << "Loading CGameState" << endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_F1))
	{
		cout << "Loading COptionsState" << endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_F2))
	{
		cout << "Loading CShopState" << endl;
		SceneManager::GetInstance()->SetActiveScene("ShopState");
	}
}
void CMenuState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 
															  Application::GetInstance().GetWindowWidth(), 
															  0, 
															  Application::GetInstance().GetWindowHeight(), 
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void CMenuState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);

	// Remove the meshes which are specific to CMenuState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
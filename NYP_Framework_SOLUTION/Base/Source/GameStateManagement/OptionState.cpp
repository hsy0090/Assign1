#include <iostream>
using namespace std;

#include "OptionState.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"
#include "LuaInterface.h"
#include "MouseController.h"
#include <sstream>

COptionState::COptionState()
{

}
COptionState::~COptionState()
{

}

void COptionState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	OptionStateBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(800.0f, 600.0f, 0.0f));

	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;

	//Sensitivity
	Sensitivity = Create::Text2DObject("text", Vector3(150, halfWindowHeight, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	sensitivity = CLuaInterface::GetInstance()->getPFloatvalue("Sensitivty");
	S1 = Create::Text2DObject("text", Vector3(150, halfWindowHeight - 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	S1->SetPosition(Vector3(S1->position.x, Application::GetInstance().GetWindowHeight() - S1->position.y - fontSize, S1->position.z));
	S2 = Create::Text2DObject("text", Vector3(300, halfWindowHeight - 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	S2->SetPosition(Vector3(S2->position.x, Application::GetInstance().GetWindowHeight() - S2->position.y - fontSize, S2->position.z));
	S3 = Create::Text2DObject("text", Vector3(550, halfWindowHeight - 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	S3->SetPosition(Vector3(S3->position.x, Application::GetInstance().GetWindowHeight() - S3->position.y - fontSize, S3->position.z));
	
	//Controls
	Movement = Create::Text2DObject("text", Vector3(150, halfWindowHeight - 8 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	
	c1 = Create::Text2DObject("text", Vector3(150, halfWindowHeight + 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	c1->SetPosition(Vector3(c1->position.x, Application::GetInstance().GetWindowHeight() - c1->position.y - fontSize, c1->position.z));
	c2 = Create::Text2DObject("text", Vector3(300, halfWindowHeight + 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	c2->SetPosition(Vector3(c2->position.x, Application::GetInstance().GetWindowHeight() - c2->position.y - fontSize, c2->position.z));
	c3 = Create::Text2DObject("text", Vector3(550, halfWindowHeight + 4 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	c3->SetPosition(Vector3(c3->position.x, Application::GetInstance().GetWindowHeight() - c3->position.y - fontSize, c3->position.z));

	Save = Create::Text2DObject("text", Vector3(150, halfWindowHeight + 10 * fontSize, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	Save->SetPosition(Vector3(Save->position.x, Application::GetInstance().GetWindowHeight() - Save->position.y, Save->position.z));

	cout << "OptionState loaded\n" << endl;
}
void COptionState::Update(double dt)
{
	MouseController::GetInstance()->GetMousePosition(cursor.x, cursor.y);

	//Sensitivity
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Sensitivity: " << sensitivity;
	Sensitivity->SetText(ss1.str());

	std::ostringstream ss2;
	ss2.precision(4);
	ss2 << "Low";
	S1->SetText(ss2.str());

	std::ostringstream ss3;
	ss3.precision(4);
	ss3 << "Medium";
	S2->SetText(ss3.str());

	std::ostringstream ss4;
	ss4.precision(4);
	ss4 << "High";
	S3->SetText(ss4.str());


	/*std::ostringstream ss5;
	ss5.precision(4);
	ss5 << "Movement: ";
	Movement->SetText(ss5.str());

	std::ostringstream ss6;
	ss6.precision(4);
	ss6 << "WASD";
	c1->SetText(ss6.str());

	std::ostringstream ss7;
	ss7.precision(4);
	ss7 << "IJKL";
	c2->SetText(ss7.str());

	std::ostringstream ss8;
	ss8.precision(4);
	ss8 << "Arrowkeys";
	c3->SetText(ss8.str());*/

	std::ostringstream ss9;
	ss9.precision(4);
	ss9 << "Save";
	Save->SetText(ss9.str());

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_SPACE))
	{
		cout << "Loading CMenuState" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, S1->GetMinAABB(), S1->GetMaxAABB()))
	{
		cout << "Editing Sensitivity...Low" << endl;
		sensitivity = 20.0;
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, S2->GetMinAABB(), S2->GetMaxAABB()))
	{
		cout << "Editing Sensitivity...Medium" << endl;
		sensitivity = 40.0;
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, S3->GetMinAABB(), S3->GetMaxAABB()))
	{
		cout << "Editing Sensitivity...High" << endl;
		sensitivity = 60.0;
	}

	/*if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, c1->GetMinAABB(), c1->GetMaxAABB()))
	{
		cout << "Editing Controls...WASD" << endl;
		movef = "W";
		moveb = "S";
		movel = "A";
		mover = "D";
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, c2->GetMinAABB(), c2->GetMaxAABB()))
	{
		cout << "Editing Sensitivity...IJKL" << endl;
		movef = 'I';
		moveb = 'K';
		movel = 'J';
		mover = 'L';
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, c3->GetMinAABB(), c3->GetMaxAABB()))
	{
		cout << "Editing Sensitivity...Arrowkeys" << endl;
		movef = VK_UP;
		moveb = VK_DOWN;
		movel = VK_LEFT;
		mover = VK_RIGHT;
	}*/

	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && EntityManager::GetInstance()->CheckPOverlap(cursor, Save->GetMinAABB(), Save->GetMaxAABB()))
	{
		cout << "Saving..." << endl;
		CLuaInterface::GetInstance()->setPFloatvalue("Sensitivty", sensitivity, true);
		/*CLuaInterface::GetInstance()->setPCharvalue("moveForward", movef.c_str());
		CLuaInterface::GetInstance()->setPCharvalue("moveBackward", moveb.c_str());
		CLuaInterface::GetInstance()->setPCharvalue("moveLeft", movel.c_str());
		CLuaInterface::GetInstance()->setPCharvalue("moveRight", mover.c_str());*/
	}
}
void COptionState::Render()
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
void COptionState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);

	// Remove the meshes which are specific to CMenuState
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}
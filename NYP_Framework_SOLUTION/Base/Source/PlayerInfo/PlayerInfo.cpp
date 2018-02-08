#include "PlayerInfo.h"
#include <iostream>

#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "../Projectile/Projectile.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/LaserBlaster.h"
#include "../WeaponInfo/GrenadeThrow.h"
#include "../WeaponInfo/AR.h"
#include "../WeaponInfo/MineThrow.h"
#include "../WeaponInfo/Detonator.h"
#include "../EntityManager.h"
#include "../Lua/LuaInterface.h"

// Allocating and initializing CPlayerInfo's static data member.  
// The pointer is allocated but not the object's constructor.
CPlayerInfo *CPlayerInfo::s_instance = 0;

CPlayerInfo::CPlayerInfo(void)
	: m_dSpeed(40.0)
	, m_dAcceleration(10.0)
	, m_bJumpUpwards(false)
	, m_dJumpSpeed(10.0)
	, m_dJumpAcceleration(-10.0)
	, m_bFallDownwards(false)
	, m_dFallSpeed(0.0)
	, m_dFallAcceleration(-10.0)
	, attachedCamera(NULL)
	, m_pTerrain(NULL)
	, primaryWeapon(NULL)
	, secondaryWeapon(NULL)
	, keyMoveForward(' ')
	, keyMoveBackward(' ')
	, keyMoveLeft(' ')
	, keyMoveRight(' ')
	, keyJump(' ')
	, keyReload(' ')
	, keychangeSweapon(' ')
	, mouseSensitivity(0.f)
	, weaponPManager(NULL)
	, weaponSManager(NULL)
	, m_iCurrentPWeapon(0)
	, m_iCurrentSWeapon(0)
	, m_iNumOfPWeapon(0)
	, m_iNumOfSWeapon(0)
	, m_fhealth(0)
	, m_iscore(0)
	, GenericEntity(NULL)
	, m_bmove(true)
	, m_bgrid(false)
{
}

CPlayerInfo::~CPlayerInfo(void)
{
	//deletes primary weapon
	if (weaponPManager)
	{
		for (int i = 0; i < m_iNumOfPWeapon; i++)
		{
			delete weaponPManager[i];
		}
		delete[] weaponPManager;
		weaponPManager = NULL;
	}

	if (weaponSManager)
	{
		for (int i = 0; i < m_iNumOfSWeapon; i++)
		{
			delete weaponSManager[i];
		}
		delete[] weaponSManager;
		weaponSManager = NULL;
	}

	/*if (secondaryWeapon)
	{
		delete secondaryWeapon;
		secondaryWeapon = NULL;
	}
	if (primaryWeapon)
	{
		delete primaryWeapon;
		primaryWeapon = NULL;
	}*/
	m_pTerrain = NULL;
}

// Initialise this class instance
void CPlayerInfo::Init(void)
{
	// Set the default values
	defaultPosition.Set(0,0,10);
	defaultTarget.Set(0,0,0);
	defaultUp.Set(0,1,0);

	// Set the current values
	position = CLuaInterface::GetInstance()->getVector3Values("CPlayerInfoStartPos");
//	position.Set(0, 0, 10);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);

	// Set Boundary
	maxBoundary.Set(1,1,1);
	minBoundary.Set(-1, -1, -1);

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Initialise the custom keyboard inputs
	mouseSensitivity = CLuaInterface::GetInstance()->getPFloatvalue("Sensitivty");
	keyMoveForward = CLuaInterface::GetInstance()->getCharValue("moveForward");
	keyMoveBackward = CLuaInterface::GetInstance()->getCharValue("moveBackward");
	keyMoveLeft = CLuaInterface::GetInstance()->getCharValue("moveLeft");
	keyMoveRight = CLuaInterface::GetInstance()->getCharValue("moveRight");
	keyJump = CLuaInterface::GetInstance()->getCharValue("jump");
	keyReload = CLuaInterface::GetInstance()->getCharValue("reload");
	keychangeSweapon = CLuaInterface::GetInstance()->getCharValue("changeSweapon");
	m_fhealth = CLuaInterface::GetInstance()->getFloatValue("health");
	m_iNumOfPWeapon = CLuaInterface::GetInstance()->getIntValue("numberofPweapons");
	m_iNumOfSWeapon = CLuaInterface::GetInstance()->getIntValue("numberofSweapons");

	float distanceSquare = CLuaInterface::GetInstance()->getDistanceSquareValue("CalculateDistanceSquare", 
																				 Vector3(0, 0, 0),
																				 Vector3(10, 10, 10));

	//int a = 1, b = 2, c = 3, d = 4;
	//CLuaInterface::GetInstance()->getVariableValues("GetMinMax", a, b, c, d);


	//Set Weapons
	weaponPManager = new CWeaponInfo*[m_iNumOfPWeapon];
	weaponPManager[0] = new CPistol();
	weaponPManager[0]->Init();

	weaponPManager[1] = new CAssualtRifle();
	weaponPManager[1]->Init();

	weaponPManager[2] = new CDetonator();
	weaponPManager[2]->Init();

	weaponSManager = new CWeaponInfo*[m_iNumOfSWeapon];
	weaponSManager[0] = new CC4Throw();
	weaponSManager[0]->Init();

	weaponSManager[1] = new CGrenadeThrow();
	weaponSManager[1]->Init();

	// Set primary weapon
	primaryWeapon = weaponPManager[GetPWeapon()];
	primaryWeapon->Init();
	// Set secondary weapon
	secondaryWeapon = weaponSManager[GetSWeapon()];
	secondaryWeapon->Init();

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Returns true if the player is on ground
bool CPlayerInfo::isOnGround(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is jumping upwards
bool CPlayerInfo::isJumpUpwards(void)
{
	if (m_bJumpUpwards == true && m_bFallDownwards == false)
		return true;

	return false;
}

// Returns true if the player is on freefall
bool CPlayerInfo::isFreeFall(void)
{
	if (m_bJumpUpwards == false && m_bFallDownwards == true)
		return true;

	return false;
}

// Set the player's status to free fall mode
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		m_bJumpUpwards = false;
		m_bFallDownwards = true;
		m_dFallSpeed = 0.0;
	}
}

// Set the player to jumping upwards
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		m_bJumpUpwards = true;
		m_bFallDownwards = false;
		m_dJumpSpeed = 4.0;
	}
}

// Set position
void CPlayerInfo::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CPlayerInfo::SetTarget(const Vector3& target)
{
	this->target = target;
}

// Set position
void CPlayerInfo::SetUp(const Vector3& up)
{
	this->up = up;
}

// Set m_dJumpAcceleration of the player
void CPlayerInfo::SetJumpAcceleration(const double m_dJumpAcceleration)
{
	this->m_dJumpAcceleration = m_dJumpAcceleration;
}

// Set Fall Acceleration of the player
void CPlayerInfo::SetFallAcceleration(const double m_dFallAcceleration)
{
	this->m_dFallAcceleration = m_dFallAcceleration;
}

// Set the boundary for the player info
void CPlayerInfo::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}

// Set the terrain for the player info
void CPlayerInfo::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Stop the player's movement
void CPlayerInfo::StopVerticalMovement(void)
{
	m_bJumpUpwards = false;
	m_bFallDownwards = false;
}

// Reset this player instance to default
void CPlayerInfo::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;

	// Stop vertical movement too
	StopVerticalMovement();
}

// Get position x of the player
Vector3 CPlayerInfo::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CPlayerInfo::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CPlayerInfo::GetUp(void) const
{
	return up;
}

// Get m_dJumpAcceleration of the player
double CPlayerInfo::GetJumpAcceleration(void) const
{
	return m_dJumpAcceleration;
}

// Get the terrain for the player info
GroundEntity* CPlayerInfo::GetTerrain(void)
{
	return m_pTerrain;
}

// Update Jump Upwards
void CPlayerInfo::UpdateJumpUpwards(double dt)
{
	if (m_bJumpUpwards == false)
		return;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dJumpSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	target.y += (float)(m_dJumpSpeed*dt + 0.5 * m_dJumpAcceleration * dt * dt);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dJumpSpeed = m_dJumpSpeed + m_dJumpAcceleration * dt;
	// Check if the jump speed is less than zero, then it should be falling
	if (m_dJumpSpeed < 0.0)
	{
		m_dJumpSpeed = 0.0;
		m_bJumpUpwards = false;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = true;
	}
}

// Update FreeFall
void CPlayerInfo::UpdateFreeFall(double dt)
{
	if (m_bFallDownwards == false)
		return;

	// Update position and target y values
	// Use SUVAT equation to update the change in position and target
	// s = u * t + 0.5 * a * t ^ 2
	position.y += (float)(m_dFallSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	target.y += (float)(m_dFallSpeed * dt + 0.5 * m_dJumpAcceleration * dt * dt);
	// Use this equation to calculate final velocity, v
	// SUVAT: v = u + a * t;
	// v is m_dJumpSpeed AFTER updating using SUVAT where u is the initial speed and is equal to m_dJumpSpeed
	m_dFallSpeed = m_dFallSpeed + m_dFallAcceleration * dt;
	// Check if the jump speed is below terrain, then it should be reset to terrain height
	if (position.y < m_pTerrain->GetTerrainHeight(position))
	{
		Vector3 viewDirection = target - position;
		position.y = m_pTerrain->GetTerrainHeight(position);
		target = position + viewDirection;
		m_dFallSpeed = 0.0;
		m_bFallDownwards = false;
	}
}

/********************************************************************************
 Hero Update
 ********************************************************************************/
void CPlayerInfo::Update(double dt)
{
	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	// Update the position if the WASD buttons were activated
	if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft) ||
		KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
	{
		Vector3 viewVector = target - position;
		Vector3 rightUV;
		bool move = true;
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveForward))
		{
			std::list<EntityBase*>::iterator collider;
			for (collider = EntityManager::GetInstance()->entityList.begin(); collider != EntityManager::GetInstance()->entityList.end(); ++collider)
			{
				if (*collider == this)
					continue;

				if ((*collider)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*collider);
					if (EntityManager::GetInstance()->CheckSphereCollision(this, thatEntity))
					{
						if (EntityManager::GetInstance()->CheckAABBCollision(this, thatEntity))
						{
							//m_fhealth--;
							move = false;

						}
					}
				}
			}

			if (move)
				position += viewVector.Normalized() * (float)m_dSpeed * (float)dt;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(keyMoveBackward))
		{
			position -= viewVector.Normalized() * (float)m_dSpeed * (float)dt;
		}
		bool move1 = true;
		if (KeyboardController::GetInstance()->IsKeyDown(keyMoveLeft))
		{
			std::list<EntityBase*>::iterator collider;
			for (collider = EntityManager::GetInstance()->entityList.begin(); collider != EntityManager::GetInstance()->entityList.end(); ++collider)
			{
				if (*collider == this)
					continue;

				if ((*collider)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*collider);
					if (EntityManager::GetInstance()->CheckSphereCollision(this, thatEntity))
					{
						if (EntityManager::GetInstance()->CheckAABBCollision(this, thatEntity))
						{
							//m_fhealth--;
							move1 = false;
						}
					}
				}
			}

			if (move1)
			{
				rightUV = (viewVector.Normalized()).Cross(up);
				rightUV.y = 0;
				rightUV.Normalize();
				position -= rightUV * (float)m_dSpeed * (float)dt;
			}
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(keyMoveRight))
		{
			std::list<EntityBase*>::iterator collider;
			for (collider = EntityManager::GetInstance()->entityList.begin(); collider != EntityManager::GetInstance()->entityList.end(); ++collider)
			{
				if (*collider == this)
					continue;

				if ((*collider)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*collider);
					if (EntityManager::GetInstance()->CheckSphereCollision(this, thatEntity))
					{
						if (EntityManager::GetInstance()->CheckAABBCollision(this, thatEntity))
						{
							//m_fhealth--;
							move1 = false;
						}
					}
				}
			}

			if (move1)
			{
				rightUV = (viewVector.Normalized()).Cross(up);
				rightUV.y = 0;
				rightUV.Normalize();
				position += rightUV * (float)m_dSpeed * (float)dt;
			}
		}
		// Constrain the position
		Constrain();
		// Update the target
		target = position + viewVector;
	}

	// Rotate the view direction
	/*if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_UP) ||
		KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	{
		Vector3 viewUV = (target - position).Normalized();
		Vector3 rightUV;
		if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		{
			float yaw = (float)(mouseSensitivity * 0.05f * m_dSpeed * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		{
			float yaw = (float)(mouseSensitivity * 0.05f * -m_dSpeed * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		{
			float pitch = (float)(mouseSensitivity * 0.05f * m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		{
			float pitch = (float)(mouseSensitivity * 0.05f * -m_dSpeed * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
	}*/

	//Update the camera direction based on mouse move
	{
		Vector3 viewUV = (target - position).Normalized();
		Vector3 rightUV;

		{
			float yaw = (float)(-mouseSensitivity * camera_yaw * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			target = position + viewUV;
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
		}
		{
			float pitch = (float)(-mouseSensitivity * camera_pitch * (float)dt);
			rightUV = viewUV.Cross(up);
			rightUV.y = 0;
			rightUV.Normalize();
			up = rightUV.Cross(viewUV).Normalized();
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			target = position + viewUV;
		}
	}

	// If the user presses SPACEBAR, then make him jump
	if (KeyboardController::GetInstance()->IsKeyDown(keyJump) &&
		position.y == m_pTerrain->GetTerrainHeight(position))
	{
		SetToJumpUpwards(true);
	}

	// Update the weapons
	if (KeyboardController::GetInstance()->IsKeyReleased(keyReload))
	{
		if (primaryWeapon)
		{
			primaryWeapon->Reload();
			//primaryWeapon->PrintSelf();
		}
		if (secondaryWeapon)
		{
			secondaryWeapon->Reload();
			//secondaryWeapon->PrintSelf();
		}
	}

	if (primaryWeapon)
		primaryWeapon->Update(dt);
	if (secondaryWeapon)
		secondaryWeapon->Update(dt);

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		if (primaryWeapon)
		{
			if (primaryWeapon == weaponPManager[2])
				dynamic_cast<CDetonator*>(primaryWeapon)->Discharge();
			else
				primaryWeapon->Discharge(position, target, this);
		}
	}
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
	{
		if (secondaryWeapon)
			secondaryWeapon->Discharge(position, target, this);
	}

	// If the user presses R key, then reset the view to default values
	/*if (KeyboardController::GetInstance()->IsKeyDown('P'))
	{
		Reset();
	}
	else*/
	{
		UpdateJumpUpwards(dt);
		UpdateFreeFall(dt);
	}

	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != GetPWeapon())
	{
		ChangePWeapon();
	}

	static bool keycS = false;
	if (KeyboardController::GetInstance()->IsKeyDown(keychangeSweapon) && !keycS)
	{
		keycS = true;
		ChangeSWeapon();
	}
	else
	{
		keycS = false;
	}

	// If a camera is attached to this playerInfo class, then update it
	if (attachedCamera)
	{
		attachedCamera->SetCameraPos(position);
		attachedCamera->SetCameraTarget(target);
		attachedCamera->SetCameraUp(up);
	}

	this->SetPAAABB(Vector3(5, 5, 5), GetPos());
}

// Constrain the position within the borders
void CPlayerInfo::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	//if (position.y > maxBoundary.y - 1.0f)
	//	position.y = maxBoundary.y - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	//if (position.y < minBoundary.y + 1.0f)
	//	position.y = minBoundary.y + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the player is not jumping nor falling, then adjust his y position
	if ((m_bJumpUpwards == false) && (m_bFallDownwards == false))
	{
		// if the y position is not equal to terrain height at that position, 
		// then update y position to the terrain height
		if (position.y != m_pTerrain->GetTerrainHeight(position))
			position.y = m_pTerrain->GetTerrainHeight(position);
	}
}

void CPlayerInfo::AttachCamera(FPSCamera* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void CPlayerInfo::DetachCamera()
{
	attachedCamera = nullptr;
}

float CPlayerInfo::GetHealth()
{
	return m_fhealth;
}

void CPlayerInfo::AddHealth(float _hp)
{
	m_fhealth += _hp;
}

float CPlayerInfo::GetScore()
{
	return m_iscore;
}

void CPlayerInfo::AddScore(float _score)
{
	m_iscore += _score;
}

bool CPlayerInfo::ChangePWeapon(void)
{
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != m_iCurrentPWeapon)
	{
		if ((MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) >= 0) &&
			(MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) < m_iNumOfPWeapon))
		{
			m_iCurrentPWeapon = MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET);
			primaryWeapon = weaponPManager[GetPWeapon()];
			return true;
		}
	}
	return false;
}

int CPlayerInfo::GetPWeapon(void) const
{
	return m_iCurrentPWeapon;
}

bool CPlayerInfo::ChangeSWeapon(void)
{
	m_iCurrentSWeapon++;

	if (m_iCurrentSWeapon > 1)
	{
		m_iCurrentSWeapon = 0;
	}

	if (secondaryWeapon == weaponSManager[GetSWeapon()])
	{
		return false;
	}
	else
	{
		secondaryWeapon = weaponSManager[GetSWeapon()];
		return true;
	}
}

int CPlayerInfo::GetSWeapon(void) const
{
	return  m_iCurrentSWeapon;
}

void CPlayerInfo::Setmove(bool _move)
{
	m_bmove = _move;
}

bool CPlayerInfo::GetGrid()
{
	return m_bgrid;
}

void CPlayerInfo::SetGrid(bool c)
{
	m_bgrid = c;
}

#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Waypoint/WaypointManager.h"

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iWayPointIndex(-1)
	, state(IDLE)
{
	listOfWaypoints.clear();
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	//target.Set(10.0f, 0.0f, 450.0f);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target = Vector3(0, 0, 0);
	cout << "Next target: " << target << endl;
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	Searchrange.SetPAABB(Vector3(scale.x * 50.f, scale.y * 50.f, scale.z * 50.f), position);
	Attackrange.SetPAABB(Vector3(scale.x * 20.5f, scale.y * 20.5f, scale.z * 20.5f), position);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

}

void CEnemy::Init(int type)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set up the waypoints
	listOfWaypoints.push_back(0);
	listOfWaypoints.push_back(1);
	listOfWaypoints.push_back(2);

	m_iWayPointIndex = 0;

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	//target.Set(10.0f, 0.0f, 450.0f);
	CWaypoint* nextWaypoint = GetNextWaypoint();
	if (nextWaypoint)
		target = nextWaypoint->GetPosition();
	else
		target = Vector3(0, 0, 0);
	cout << "Next target: " << target << endl;
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	// Initialise the LOD meshes
	switch (type)
	{
	case 0:
		// core
		InitLOD("sphere", "cube", "cubeSG");
		break;
	case 1:
		// shields
		InitLOD("cubeSG", "sphere", "cube");
	}

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));
	Searchrange.SetPAABB(Vector3(scale.x * 50.f, scale.y * 50.f, scale.z * 50.f), position);
	Attackrange.SetPAABB(Vector3(scale.x * 20.5f, scale.y * 20.5f, scale.z * 20.5f), position);

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Get next Waypoint for this CEnemy
CWaypoint* CEnemy::GetNextWaypoint(void)
{
	if ((int)listOfWaypoints.size() > 0)
	{
		m_iWayPointIndex++;
		if (m_iWayPointIndex >= (int)listOfWaypoints.size())
			m_iWayPointIndex = 0;
		return CWaypointManager::GetInstance()->GetWaypoint(listOfWaypoints[m_iWayPointIndex]);
	}
	else
		return NULL;
}

// Update
void CEnemy::Update(double dt)
{
	switch (state)
	{
	case IDLE:
		Idle(CPlayerInfo::GetInstance()->GetMaxAAABB(), CPlayerInfo::GetInstance()->GetMinAAABB(), dt);
		break;
	case SEARCH:
		Search(CPlayerInfo::GetInstance()->GetMaxAAABB(), CPlayerInfo::GetInstance()->GetMinAAABB(), dt);
		break;
	case ATTACK:
		Attack(CPlayerInfo::GetInstance()->GetMaxAAABB(), CPlayerInfo::GetInstance()->GetMinAAABB(), dt);
		break;
	default:
		break;
	}


	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	Constrain();

	//Vector3 viewVector = (target - position).Normalized();
	//position += viewVector * (float)m_dSpeed * (float)dt;

	this->SetPAAABB(Vector3(scale.x * 3.f, scale.y * 3.f, scale.z * 3.f), position);
	Searchrange.SetPAABB(Vector3(scale.x * 50.f, scale.y * 50.f, scale.z * 50.f), position);
	Attackrange.SetPAABB(Vector3(scale.x * 20.5f, scale.y * 20.5f, scale.z * 20.5f), position);

	// Update the target
	
	/*if (position.z > 400.0f)
	target.z = position.z * -1;
	else if (position.z < -400.0f)
	target.z = position.z * -1;*/
	

	/*if ((target - position).LengthSquared() < 25.0f)
	{
		CWaypoint* nextWaypoint = GetNextWaypoint();
		if (nextWaypoint)
			target = nextWaypoint->GetPosition();
		else
			target = Vector3(0, 0, 0);
		cout << "Next target: " << target << endl;
	}*/
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

void CEnemy::Idle(Vector3 playermax, Vector3 playermin, double dt)
{
	if (EntityManager::GetInstance()->CheckOverlap(Searchrange.GetMinAABB(), Searchrange.GetMaxAABB(), playermin, playermax))
	{
		state = SEARCH;
	}
}

void CEnemy::Search(Vector3 playermax, Vector3 playermin, double dt)
{
	if (!EntityManager::GetInstance()->CheckOverlap(Searchrange.GetMinAABB(), Searchrange.GetMaxAABB(), playermin, playermax))
	{
		state = IDLE;
	}
	else
	{
		if ((target - position).LengthSquared() < 25.0f)
		{
			CWaypoint* nextWaypoint = GetNextWaypoint();
			if (nextWaypoint)
				target = nextWaypoint->GetPosition();
			else
				target = Vector3(0, 0, 0);
			cout << "Next target: " << target << endl;
		}
	}

	Vector3 viewVector = (target - position).Normalized();

	bool collision = EntityManager::GetInstance()->CheckOverlap(GetMinAABB(), GetMaxAABB(), playermin, playermax);

	if (!collision && state != IDLE)
		position += viewVector * (float)m_dSpeed * (float)dt;

	if (EntityManager::GetInstance()->CheckOverlap(Attackrange.GetMinAABB(), Attackrange.GetMaxAABB(), playermin, playermax))
	{
		state = ATTACK;
	}
}

void CEnemy::Attack(Vector3 playermax, Vector3 playermin, double dt)
{
	if (!EntityManager::GetInstance()->CheckOverlap(Attackrange.GetMinAABB(), Attackrange.GetMaxAABB(), playermin, playermax))
	{
		state = SEARCH;
	}

	target = CPlayerInfo::GetInstance()->GetPos();
	Vector3 viewVector = (target - position).Normalized();


	bool collision = EntityManager::GetInstance()->CheckOverlap(this->GetMinAAABB(), this->GetMaxAAABB(), playermin, playermax);

	if (EntityManager::GetInstance()->CheckOverlap(this->GetMinAAABB(), this->GetMaxAAABB(), playermin, playermax))
	{
		CPlayerInfo::GetInstance()->AddHealth(-1);
	}

	if (!collision && state != IDLE)
		position += viewVector * (float)m_dSpeed * (float)dt;
}

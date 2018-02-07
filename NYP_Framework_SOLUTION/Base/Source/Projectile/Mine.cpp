#include "Mine.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "MyMath.h"
#include "../SpatialPartition/SpatialPartition.h"
#include "../SceneGraph/SceneGraph.h"
#include "../WeaponInfo/Detonator.h"

#include <iostream>
using namespace std;

CC4::CC4(void)
	: CProjectile(NULL)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CC4::CC4(Mesh* _modelMesh)
	: CProjectile(_modelMesh)
	, m_fGravity(-10.0f)
	, m_fElapsedTime(0.0f)
	, m_pTerrain(NULL)
{
}

CC4::~CC4(void)
{
	m_pTerrain = NULL; // Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;

	

	// Check the SpatialPartition to destroy nearby objects
	vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1.0f);
	for (int i = 0; i < ExportList.size(); ++i)
	{
		ExportList[i]->SetIsDone(true);

		// Remove from Scene Graph
		if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
		{
			cout << "*** This Entity removed ***" << endl;
		}
	}


}

// Update the status of this projectile
void CC4::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Check if the Mine is already on the ground
	if (position.y >= m_pTerrain->GetTerrainHeight(position) - 10.0f + Math::EPSILON)
	{
		// Update Position
		m_fElapsedTime += dt;

		position.Set(position.x + (float)(theDirection.x * m_fElapsedTime * m_fSpeed),
			position.y + (float)(theDirection.y * m_fElapsedTime * m_fSpeed) + (0.5 * m_fGravity * m_fElapsedTime * m_fElapsedTime),
			position.z + (float)(theDirection.z * m_fElapsedTime * m_fSpeed));
		
		
		

		if (position.y < m_pTerrain->GetTerrainHeight(position) - 10.0f)
		{
			position.y = m_pTerrain->GetTerrainHeight(position) - 10.0f;
			m_fSpeed = 0.0f;
			return;
		}
	}
}

// Set the terrain for the player info
void CC4::SetTerrain(GroundEntity* m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

void CC4::Explode()
{
	if (isDone)
		return;

	// Check the SpatialPartition to destroy nearby objects
	vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1.0f);
	for (int i = 0; i < ExportList.size(); ++i)
	{
		ExportList[i]->SetIsDone(true);

		// Remove from Scene Graph
		if (CSceneGraph::GetInstance()->DeleteNode(ExportList[i]) == true)
		{
			cout << "*** This Entity removed ***" << endl;
		}
	}
}

// Create a projectile and add it into EntityManager
CC4* Create::Mine(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _direction,
	const float m_fLifetime,
	const float m_fSpeed,
	CPlayerInfo* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CC4* result = new CC4(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetTerrain(_source->GetTerrain());
	EntityManager::GetInstance()->AddEntity(result, true);
	//Add Mine to detonator
	dynamic_cast<CDetonator*>(CPlayerInfo::GetInstance()->weaponPManager[2])->Active.push_back(result);
	return result;
}
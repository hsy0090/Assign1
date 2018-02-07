#pragma once
#include "Projectile.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"
#include "../GroundEntity.h"

class Mesh;
class CPlayerInfo;

class CC4 : public CProjectile
{
protected:
	float m_fGravity;
	float m_fElapsedTime;
	GroundEntity* m_pTerrain;

public:
	CC4(void);
	CC4(Mesh* _modelMesh);
	~CC4(void);

	// Update the status of this projectile
	void Update(double dt = 0.0333f);

	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	void Explode();
};

namespace Create
{
	CC4* Mine(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _direction,
		const float m_fLifetime,
		const float m_fSpeed,
		CPlayerInfo* _source = NULL);
};


#pragma once

#include "WeaponInfo.h"
#include "../GenericEntity.h"

class CPistol :
	public CWeaponInfo , public GenericEntity
{
public:
	CPistol();
	CPistol(Mesh* _modelMesh);
	virtual ~CPistol();

	// Initialise this instance to default values
	void Init(void);
	void Render();
};


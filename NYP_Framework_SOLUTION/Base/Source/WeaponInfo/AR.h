#pragma once

#include "WeaponInfo.h"
#include "../GenericEntity.h"

class CAssualtRifle :
	public CWeaponInfo , public GenericEntity
{
public:
	CAssualtRifle();
	virtual ~CAssualtRifle();

	// Initialise this instance to default values
	void Init(void);
	void Render();
};


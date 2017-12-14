#pragma once

#include "WeaponInfo.h"

class CC4Throw :
	public CWeaponInfo
{
public:
	CC4Throw();
	virtual ~CC4Throw();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};
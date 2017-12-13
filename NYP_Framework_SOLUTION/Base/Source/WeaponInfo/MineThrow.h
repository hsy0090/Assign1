#pragma once

#include "WeaponInfo.h"

class CMineThrow :
	public CWeaponInfo
{
public:
	CMineThrow();
	virtual ~CMineThrow();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};
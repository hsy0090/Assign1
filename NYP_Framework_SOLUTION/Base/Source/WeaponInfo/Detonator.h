#pragma once

#include "WeaponInfo.h"
#include "..//Projectile/Mine.h"

#include <vector>

class CDetonator :
	public CWeaponInfo
{
public:
	CDetonator();
	virtual ~CDetonator();

	// Initialise this instance to default values
	void Init(void);

	void Discharge();

	std::vector <CMine*> Active;
};
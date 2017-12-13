#include "Detonator.h"


CDetonator::CDetonator()
{
}

CDetonator::~CDetonator()
{
}

// Initialise this instance to default values
void CDetonator::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 0;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 40;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 40;

	// The time between shots
	timeBetweenShots = 0.1111;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void CDetonator::Discharge()
{
	if (bFire)
	{
		// If there is any mines, then detonate
		if (magRounds > 0)
		{

			for (int i = 0; i < Active.size(); i++)
				Active[i]->SetIsDone(true);

			magRounds = 0;
		}

	}
}

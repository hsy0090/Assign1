#include "AR.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CAssualtRifle::CAssualtRifle():
	GenericEntity(NULL)
{
}


CAssualtRifle::~CAssualtRifle()
{
}

// Initialise this instance to default values
void CAssualtRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 40;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 40;
	// The current total number of rounds currently carried by this player
	totalRounds = 240;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 240;

	// The time between shots
	timeBetweenShots = 0.1111;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void CAssualtRifle::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(GetLODMesh());
	modelStack.PopMatrix();
}
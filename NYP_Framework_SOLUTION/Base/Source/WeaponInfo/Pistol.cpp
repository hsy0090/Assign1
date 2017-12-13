#include "Pistol.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CPistol::CPistol():
	GenericEntity(NULL)
{
}

CPistol::CPistol(Mesh* _modelMesh):
	GenericEntity(_modelMesh)
{
	
}

CPistol::~CPistol()
{
}

// Initialise this instance to default values
void CPistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 8;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 8;
	// The current total number of rounds currently carried by this player
	totalRounds = 40;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 40;

	// The time between shots
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	InitLOD("cube", "sphere", "cubeSG");
}

void CPistol::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMesh(GetLODMesh());
	}
	else
		RenderHelper::RenderMesh(modelMesh);

	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

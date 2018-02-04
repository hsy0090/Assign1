#include "Collider.h"


CCollider::CCollider()
	: maxAABB(Vector3(0, 0, 0))
	, minAABB(Vector3(0, 0, 0))
{
}


CCollider::~CCollider()
{
}

void CCollider::SetPAABB(Vector3 size, Vector3 pos)
{
	this->maxAABB = size * (0.5, 0.5, 0.5) + pos;
	this->minAABB = pos - size * (0.5, 0.5, 0.5);
}

void CCollider::SetPAAABB(Vector3 size, Vector3 pos)
{
	this->AmaxAABB = size * (0.5, 0.5, 0.5) + pos;
	this->AminAABB = pos - size * (0.5, 0.5, 0.5);
}

// Set the maxAABB and minAABB
void CCollider::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}
// Set the maxAABB
void CCollider::SetMaxAABB(Vector3 maxAABB)
{
	this->maxAABB = maxAABB;
}
// Get the maxAABB
Vector3 CCollider::GetMaxAABB(void)
{
	return maxAABB;
}
Vector3 CCollider::GetMaxAAABB(void)
{
	return AmaxAABB;
}
// Set the minAABB
void CCollider::SetMinAABB(Vector3 minAABB)
{
	this->minAABB = minAABB;
}
// Get the minAABB
Vector3 CCollider::GetMinAABB(void)
{
	return minAABB;
}

Vector3 CCollider::GetMinAAABB(void)
{
	return AminAABB;
}

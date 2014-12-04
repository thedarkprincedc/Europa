#include "ISteerable.h"

ISteerable::ISteerable()
{
	velocity = D3DXVECTOR3(0,0,0);
	speed = 1;
	maxSpd = 10;
}
ISteerable::~ISteerable(void)
{
}
void ISteerable::Initialize(DrawableGameObject *pDrawableGameObject)
{
	m_pDrawableGameObject = pDrawableGameObject;
}
void ISteerable::headFor(float x, float z)
{
}
void ISteerable::turnTo(ISteerable *object)
{
}
void ISteerable::turnLeft(float ang)
{
	heading.y = D3DXToRadian(ang);
}
void ISteerable::turnRight(float ang)
{
	heading.y = D3DXToRadian(ang);
}
void ISteerable::limitSpeed()
{
}
D3DXVECTOR3 ISteerable::steer(D3DXVECTOR3 desiredVec)
{
	D3DXVECTOR3 normPosition;
	D3DXVec3Normalize(&normPosition, &desiredVec);
	velocity += desiredVec;
	return normPosition;

	//m_pDrawableGameObject->rotation.y = D3DXVec3Dot(position, desiredVec);
}
void ISteerable::foward()
{
	//velocity.z += speed;
}
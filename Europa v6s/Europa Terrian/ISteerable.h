#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Math.h>
#include "DrawableGameObject.h"
class ISteerable 
{
public:
	ISteerable();
	~ISteerable(void);
	void Initialize(DrawableGameObject *pDrawableGameObject);
	
	void headFor(float x, float z);
	void turnTo(ISteerable *object);
	void turnLeft(float ang);
	void turnRight(float ang);

	void limitSpeed();
	D3DXVECTOR3 steer(D3DXVECTOR3 desiredVec);
	void foward();
	virtual void Update() = 0;
public:
	DrawableGameObject *m_pDrawableGameObject;	
	D3DXVECTOR3 heading;
	D3DXVECTOR3 velocity; 
	D3DXVECTOR3 desiredVelocity;
	
	float speed;
	float maxSpd;
};

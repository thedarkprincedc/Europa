#pragma once
#include "DrawableGameObject.h"
#include "ISteerable.h"
#include "PathFollowing.h"
#include "../Europa v6/CMesh.h"
class SpaceShip : public DrawableGameObject
{
public:
	SpaceShip(Game *pGame);
	~SpaceShip(void);
	void Draw();
	void Update();
	void GetKeys(D3DXVECTOR3 desiredVelocity);
protected:
	CMesh		 *pMesh;
	D3DMATRIX	 transformMatrix;
};

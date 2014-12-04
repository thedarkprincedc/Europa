#include "SpaceShip.h"

SpaceShip::SpaceShip(Game *pGame) : DrawableGameObject(pGame)
{
	pMesh = new CMesh(pGame->graphics.GetD3DDevice(), "./maps/ship.x", "./maps");

	//scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	//D3DXMatrixScaling(&this->transMatrix, scale.x, scale.y, scale.z);
	//addPoint(D3DXVECTOR3(0,0,0));
	//addPoint(D3DXVECTOR3(0,0,50));
	//addPoint(D3DXVECTOR3(50,0,50));
	//addPoint(D3DXVECTOR3(50,0,0));
}

SpaceShip::~SpaceShip(void)
{
}
void SpaceShip::Update()
{
	//position.x++;
	//D3DXVECTOR3 newPosition = D3DXVECTOR3(0,0,300);
	//D3DXVECTOR3 nextPosition;
	//D3DXVec3Subtract(&nextPosition, &position, &newPosition);
	//D3DXVec3Normalize(&nextPosition, &nextPosition);

	//D3DXVec3TransformCoord(&rotation, &rotation, &rotationMatrix);
	//stepFrame();
	//
	//velocity = D3DXVECTOR3(0,0,0);
	//if(GetAsyncKeyState(VK_LEFT))
	//{
	//	rotation.y += -D3DXToRadian(5);
	//}
	//if(GetAsyncKeyState(VK_RIGHT))
	//{
	//	rotation.y += D3DXToRadian(5);
	//}
	//if(GetAsyncKeyState(VK_UP))
	//{
	//	velocity.z++;
	//}
	//D3DXVECTOR newFoward;
	//position = position + velocity;
	//D3DXQuaternionRotationAxis(&quatRotation, &D3DXVECTOR3(0,1,0), rotation.y);
	//D3DXMatrixRotationQuaternion(&rotationMatrix, &quatRotation); 
	//D3DXMatrixTranslation(&translationMatrix, position.x, position.y, position.z);
	//transformMatrix = rotationMatrix * translationMatrix;
}
void SpaceShip::Draw()
{
	
	pMesh->Render(m_pGame->graphics.GetD3DDevice(), D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), D3DXVECTOR3(.2,.2,.2));
}
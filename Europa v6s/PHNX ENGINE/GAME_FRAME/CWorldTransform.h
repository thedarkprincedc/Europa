/***************************************
	Also found this method on www.c-unit.com
	It works well for tranforming 3d meshes.
	All you have to do is derive this this 
	to your mesh class and hook it up
***************************************/
#ifndef _CWORLDTRANSFORM_H_
#define _CWORLDTRANSFORM_H_
#include "./Global_Framework.h"
class CWorldTransform{
public:
	CWorldTransform();
	virtual ~CWorldTransform();
	void Reset();
	
	void TranslateRel(float x, float y, float z);
	void TranslateAbs(float x, float y, float z);
	
	void RotateRel(float x, float y, float z);
	void RotateAbs(float x, float y, float z);
	
	void ScaleRel(float x, float y, float z);
	void ScaleAbs(float x, float y, float z);
	
	D3DXMATRIX* GetTransform();
	D3DXVECTOR3* GetPosition();
	
	float GetXPosition();
	float GetYPosition();
	float GetZPosition();
	
	float GetXRotation();
	float GetYRotation();
	float GetZRotation();
	
	float GetXScale();
	float GetYScale();
	float GetZScale();

	void SetXPosition(float x);
	void SetYPosition(float y);
	void SetZPosition(float z);
	
	void SetXRotation(float x);
	void SetYRotation(float y);
	void SetZRotation(float z);
	
	void SetXScale(float x);
	void SetYScale(float y);
	void SetZScale(float z);
protected:
	D3DXMATRIX	m_transform;
	D3DXMATRIX	m_translate;
	D3DXMATRIX	m_rotate;
	D3DXMATRIX	m_scale;
	D3DXVECTOR3 m_position;
	float		m_rotationX;
	float		m_rotationY;
	float		m_rotationZ;
};
#endif\\
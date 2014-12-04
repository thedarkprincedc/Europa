#include "./Global_Framework.h"
CWorldTransform::CWorldTransform(){	 Reset(); }
CWorldTransform::~CWorldTransform(){ Reset(); }
void CWorldTransform::Reset(){
	D3DXMatrixIdentity(&m_translate);
	D3DXMatrixIdentity(&m_rotate);
	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixIdentity(&m_transform);
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotationX = m_rotationY = m_rotationZ = 0.0f;
}
void CWorldTransform::TranslateAbs(float x, float y, float z){
	m_translate._41 = x;	m_translate._42 = y;	m_translate._43 = z;
	m_position.x	= x;	m_position.y	= y;	m_position.z	= z;
}
void CWorldTransform::TranslateRel(float x, float y, float z){
	m_translate._41 += x;	m_translate._42 += y;	m_translate._43 += z;
	m_position.x	+= x;	m_position.y	+= y;	m_position.z	+= z;
}
void CWorldTransform::RotateAbs(float x, float y, float z){
	m_rotationX = x;	m_rotationY = y;	m_rotationZ = z;
	::D3DXMatrixRotationYawPitchRoll(&m_rotate, x, y, z);
}
void CWorldTransform::RotateRel(float x, float y, float z){
	m_rotationX += x;	m_rotationY += y;	m_rotationZ += z;
	::D3DXMatrixRotationYawPitchRoll(&m_rotate, x, y, z);
}
void CWorldTransform::ScaleAbs(float x, float y, float z){
	m_scale._11 = x;	m_scale._22 = y;	m_scale._33 = z;
}
void CWorldTransform::ScaleRel(float x, float y, float z){
	m_scale._11 += x;	m_scale._22 += y;	m_scale._33 += z;
}
D3DXMATRIX *CWorldTransform::GetTransform(){
	m_transform = m_scale * m_rotate * m_translate;
	return &m_transform;
}
float CWorldTransform::GetXPosition(){	return m_position.x;	}
float CWorldTransform::GetYPosition(){	return m_position.y;	}
float CWorldTransform::GetZPosition(){	return m_position.z;	}
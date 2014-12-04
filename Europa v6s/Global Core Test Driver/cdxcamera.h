/*
	CCamera Class <Direct3D9>
	BrettMosley@hotmail.com
*/
#pragma once
#include <d3dx9.h>

#define DEFAULT_POSITION D3DXVECTOR3(0.0f,0.0f,0.0f)
#define DEFAULT_RIGHT	 D3DXVECTOR3(1.0f,0.0f,0.0f)
#define DEFAULT_UP		 D3DXVECTOR3(0.0f,1.0f,0.0f)
#define DEFAULT_LOOK	 D3DXVECTOR3(0.0f,0.0f,1.0f)
enum CameraType	{	AIRCRAFT, LANDOBJECT	};

class CCamera
{
public:
	void SetCameraType(CameraType cameraType);
public:
	CCamera();
	CCamera(CameraType cameraType);
	~CCamera();
public:
	void Walk(float units);				//	Forward / Backward
	void Strafe(float units);			//	Left / Right
	void Fly(float units);				//	Up / Down
	void Pitch(float angle);			//	Rotate right vector
	void Yaw(float angle);				//	Rotate up vector
	void Roll(float angle);				//	Rotate look vector
	void GetViewMatrix(D3DXMATRIX *viewMatix);
	void GetRightVector(D3DXVECTOR3 *right);
	void GetUpVector(D3DXVECTOR3 *up);
	void GetLookVector(D3DXVECTOR3 *look);
	void GetPosition(D3DXVECTOR3 *position);
	void SetPosition(D3DXVECTOR3 *position);
private:
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vPosition;
	CameraType	m_CameraType;
};
CCamera::CCamera(){
	SetCameraType(AIRCRAFT);
	m_vPosition = DEFAULT_POSITION;
	m_vUp		= DEFAULT_UP;
	m_vRight	= DEFAULT_RIGHT;
	m_vLook		= DEFAULT_LOOK;
}
CCamera::CCamera(CameraType cameraType){
	SetCameraType(cameraType);
	m_vPosition = DEFAULT_POSITION;
	m_vUp		= DEFAULT_UP;
	m_vRight	= DEFAULT_RIGHT;
	m_vLook		= DEFAULT_LOOK;
}
CCamera::~CCamera(){}
void CCamera::SetCameraType(CameraType cameraType){
	m_CameraType = cameraType;
}
void CCamera::Walk(float units){
	if(m_CameraType == LANDOBJECT)
		m_vPosition += D3DXVECTOR3(m_vLook.x, 0.0f, m_vLook.z) * units;
	if(m_CameraType == AIRCRAFT)
		m_vPosition += m_vLook * units;
}
void CCamera::Strafe(float units){
	if(m_CameraType == LANDOBJECT)
		m_vPosition += D3DXVECTOR3(m_vRight.x, 0.0f, m_vRight.z);
	if(m_CameraType == AIRCRAFT)
		m_vPosition += m_vRight * units;
}	
void CCamera::Fly(float units){
	if(m_CameraType == LANDOBJECT)
		m_vPosition.y += units;
	if(m_CameraType == AIRCRAFT)
		m_vPosition += m_vUp * units;
}			
void CCamera::Pitch(float angle){
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_vRight,	angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&m_vUp,&m_vUp, &T);
	D3DXVec3TransformCoord(&m_vLook,&m_vLook, &T);
}	
void CCamera::Yaw(float angle){
	D3DXMATRIX T;
	// rotate around world y (0, 1, 0) always for land object
	if( m_CameraType == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);
	// rotate around own up vector for aircraft
	if( m_CameraType == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &m_vUp, angle);
	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&m_vRight,&m_vRight, &T);
	D3DXVec3TransformCoord(&m_vLook,&m_vLook, &T);
}	
void CCamera::Roll(float angle){
	if( m_CameraType == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_vLook, angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&m_vRight,&m_vRight, &T);
		D3DXVec3TransformCoord(&m_vUp,&m_vUp, &T);
	}
}		
void CCamera::GetViewMatrix(D3DXMATRIX *viewMatrix){
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	float x = -D3DXVec3Dot(&m_vRight, &m_vPosition);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPosition);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPosition);

	(*viewMatrix)(0,0) = m_vRight.x;(*viewMatrix)(0,1) = m_vUp.x; 
	(*viewMatrix)(1,0) = m_vRight.y;(*viewMatrix)(1,1) = m_vUp.y;
	(*viewMatrix)(2,0) = m_vRight.z;(*viewMatrix)(2,1) = m_vUp.z;
	(*viewMatrix)(3,0) = x;			(*viewMatrix)(3,1) = y;
	(*viewMatrix)(0,2) = m_vLook.x; (*viewMatrix)(0,3) = 0.0f;
	(*viewMatrix)(1,2) = m_vLook.y; (*viewMatrix)(1,3) = 0.0f;
	(*viewMatrix)(2,2) = m_vLook.z; (*viewMatrix)(2,3) = 0.0f;
	(*viewMatrix)(3,2) = z;			(*viewMatrix)(3,3) = 1.0f;
}
void CCamera::GetRightVector(D3DXVECTOR3 *right){
	*right = m_vRight;
}
void CCamera::GetUpVector(D3DXVECTOR3 *up){
	*up = m_vUp;
}
void CCamera::GetLookVector(D3DXVECTOR3 *look){
	*look = m_vLook;
}
void CCamera::GetPosition(D3DXVECTOR3 *position){
	*position = m_vPosition;
}
void CCamera::SetPosition(D3DXVECTOR3 *position){
	m_vPosition = *position;
}
#include "./cemitter.h"
CEmitter::CEmitter(int numParticles, D3DXVECTOR3 Origin){
	m_iNumParticles = numParticles;
	m_vOrigin = Origin;
}
CEmitter::~CEmitter(){}
CParticleExplosion::CParticleExplosion(IDirect3DDevice9 *pDevice, int numParticles, 
									   D3DXVECTOR3 Origin, char *pszTexture)
	:	CEmitter(numParticles, Origin)
{
	HRESULT rslt = S_OK;
	// The maximum number of particles we want visible
	m_iNumParticles = numParticles;
	// The object space starting position of the emitter (and thus, every particle)
	//m_vOrigin = Origin
	// Our nice particle
	WCHAR wchtexturefilename[255];	//unicode char
	mbstowcs(wchtexturefilename, pszTexture, 255); // conversion
	rslt=D3DXCreateTextureFromFileEx(pDevice, wchtexturefilename, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
									 D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
									 &m_pTexture);
	//if(FAILED(rslt)) { return D3DError(rslt, __LINE__, __FILE__, "Could not create texture"); }
	// We're using quads instead of pointsprites now, so we need 4 vertices for every particle.
	rslt=pDevice->CreateVertexBuffer(sizeof(BILLBOARDPARTICLEVERTEX) * m_iNumParticles * 4, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
									 D3DFVF_PARTICLES, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
//	if(FAILED(rslt)) { return D3DError(rslt, __LINE__, __FILE__, "Could not create vertex buffer"); }
	// Again, we need 4* the vertices now, as we're using quads.
	m_akParticles = new BILLBOARDPARTICLEVERTEX[m_iNumParticles * 4];
	// Seed the randomisation.
	srand((unsigned)timeGetTime());
	// And set the first particle's age to 0, so that when we start the rendering all our particles are
	// recycled.
	m_akParticles[0].m_fAge = 0;
}
CParticleExplosion::~CParticleExplosion(){}
HRESULT CParticleExplosion::Render(IDirect3DDevice9 *pDevice){
	HRESULT rslt = S_OK;
	// If the age of the first particle is 0, regenerate all the particles (all particles age at the
	// same rate in this demo, so we don't need to check each one individually)
	if(m_akParticles[0].m_fAge == 0)
	{
		// Handle the vertices in batches of 4 (ie: one particle = 1 quad = 4 verts)
		{
		for(int iCount = 0; iCount < m_iNumParticles * 4; iCount += 4)
		{

			// I'm sure you can work this code out. Just remember it's a quad!
			m_akParticles[iCount].m_fX = -0.5f;
			m_akParticles[iCount].m_fY = -0.5f;
			m_akParticles[iCount].m_fZ = 0.0f;
			m_akParticles[iCount].m_fTU = 0.0f;
			m_akParticles[iCount].m_fTV = 1.0f;

			m_akParticles[iCount].m_dwColor = D3DCOLOR_ARGB(255,255,255,255);

			m_akParticles[iCount].m_fAge = 255;
			m_akParticles[iCount].m_fVelocityX = (((rand()%100) - 50) / 1125.0f);
			m_akParticles[iCount].m_fVelocityY = (((rand()%100) - 50) / 1125.0f);
			m_akParticles[iCount].m_fVelocityZ = (((rand()%100) - 50) / 1125.0f);

			memcpy((void*)&m_akParticles[iCount + 1], (void*)&m_akParticles[iCount], sizeof(BILLBOARDPARTICLEVERTEX));
			m_akParticles[iCount + 1].m_fX = -0.5f;
			m_akParticles[iCount + 1].m_fY = 0.5f;
			m_akParticles[iCount + 1].m_fTU = 0.0f;
			m_akParticles[iCount + 1].m_fTV = 0.0f;

			memcpy((void*)&m_akParticles[iCount + 2], (void*)&m_akParticles[iCount], sizeof(BILLBOARDPARTICLEVERTEX));
			m_akParticles[iCount + 2].m_fX = 0.5f;
			m_akParticles[iCount + 2].m_fY = -0.5f;
			m_akParticles[iCount + 2].m_fTU = 1.0f;
			m_akParticles[iCount + 2].m_fTV = 1.0f;

			memcpy((void*)&m_akParticles[iCount + 3], (void*)&m_akParticles[iCount], sizeof(BILLBOARDPARTICLEVERTEX));
			m_akParticles[iCount + 3].m_fX = 0.5f;
			m_akParticles[iCount + 3].m_fY = 0.5f;
			m_akParticles[iCount + 3].m_fTU = 1.0f;
			m_akParticles[iCount + 3].m_fTV = 0.0f;

		}
		}
	}
	else
	{
		// The first particle isn't aged 0, so move all particles by their velocities
		{
		for(int iCount = 0; iCount < m_iNumParticles * 4; iCount += 4)
		{
			for(int iInner = iCount; iInner < iCount + 4; iInner++)
			{
				m_akParticles[iInner].m_fX += m_akParticles[iInner].m_fVelocityX;
				m_akParticles[iInner].m_fY += m_akParticles[iInner].m_fVelocityY;
				m_akParticles[iInner].m_fZ += m_akParticles[iInner].m_fVelocityZ;

				m_akParticles[iInner].m_fX += m_akParticles[iInner].m_fVelocityX;
				m_akParticles[iInner].m_fY += m_akParticles[iInner].m_fVelocityY;
				m_akParticles[iInner].m_fZ += m_akParticles[iInner].m_fVelocityZ;

				// Reduce the age of the particle, and get an integer rounded value of the float.
				int x = (int)(m_akParticles[iInner].m_fAge -= 1.0f);

				// And use that int for our vertex colour, to make the particle fade with age.
				m_akParticles[iInner].m_dwColor = D3DCOLOR_ARGB(x,x,x,x);
			}
		}
		}
	}
	// You've seen this a million times. Lock the vertex buffer, copy the particles in and unlock.
	BYTE* pVerticeLock=0;
	rslt=m_pVertexBuffer->Lock(0, sizeof(BILLBOARDPARTICLEVERTEX) * m_iNumParticles * 4, (void**)&pVerticeLock,
							   D3DLOCK_DISCARD);
//	if(FAILED(rslt)) { return D3DError(rslt, __LINE__, __FILE__, "Could not lock vertex buffer"); }
	CopyMemory(pVerticeLock, m_akParticles, sizeof(BILLBOARDPARTICLEVERTEX) * m_iNumParticles * 4);
	m_pVertexBuffer->Unlock();
	// Should really check the return codes here, but, what the hey :)
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(BILLBOARDPARTICLEVERTEX));
	pDevice->SetFVF(D3DFVF_PARTICLES);
	pDevice->SetTexture(0, m_pTexture);
	// And now, time for our billboard.
	D3DXMATRIX matView, matBillboardView;
	// Obtain the view matrix from D3D.
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	// Elements 41, 42 and 43 (row 4, columns 1, 2 and 3) in the matrix represent the translation, so by
	// setting them to 0 we ensure no translation will occur.
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	// Transpose the matrix (basically, switch the rows & columns of the matrix)
	D3DXMatrixTranspose(&matBillboardView, &matView);
	// Now set the world transformation to our clever billboard matrix.
	pDevice->SetTransform(D3DTS_WORLD, &matBillboardView);
	// Render each particle
	for(int iCount = 0; iCount < m_iNumParticles; iCount++)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, iCount * 4, 2);
	// And reset the world transform to the identity matrix.
	D3DXMatrixIdentity(&matBillboardView);
	pDevice->SetTransform(D3DTS_WORLD, &matBillboardView);
	return S_OK;
}
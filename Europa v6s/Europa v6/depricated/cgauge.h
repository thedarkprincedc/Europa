#include "../PHNX ENGINE/phoenixengine.h"
//180 degree gauge 
//starts at the left
//		 0    + 
//90  =  50   +45
//		 100  +90
///#define PI 3.14
//#define QuaterGauge 3.14/2
//#define HalfGauge 3.14/2
//#define 3QuarterGauge (2*3.14)-(3.14/2)
//#define FullGauge 3.14*2
static const float PI = 3.14;
static const float HALFGAUGE = 3.14;

class CGauge : public CSprite{
public:
	CGauge(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
		int currVal, int maxVal, char *needle, float gaugeRadians)
		: CSprite(pDevice, pSprite, needle, 0){
		//maxRad = PI / maxVal
	//	CSprite::SetTranslation(100, 100);
	//	CSprite::SetRotationPoint(D3DXVECTOR2(100, 0));
		//m_radian = 0;
	}
	void Render(){
		//m_radian += 0.01;
		//if(m_radian < maxRad)
	//		CSprite::SetRotation(m_radian);
	//	CSprite::Render();
	}
protected:
	float maxVal;
	double maxRad;
};

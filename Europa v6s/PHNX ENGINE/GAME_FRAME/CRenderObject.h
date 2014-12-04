#ifndef _CRENDEROBJECT_H_
#define _CRENDEROBJECT_H_
typedef enum {UNKNOWN, SPRITE, FONT} RITTI;
class CRenderObject{
public:
	virtual RECT getRect()=0;
	virtual float getWidth()=0;
	virtual float getHeight()=0;
	virtual void SetTranslation(float x, float y) = 0;
	virtual void Render()=0;
	virtual RITTI getRITTI() = 0;
};
#endif
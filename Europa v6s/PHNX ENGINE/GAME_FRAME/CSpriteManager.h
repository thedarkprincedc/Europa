#ifndef _CSPRITEMANAGER_H_
#define _CSPRITEMANAGER_H_
#include <map>
#include <hash_map>

//#define _DEFINE_DEPRECATED_HASH_CLASSES 0
using namespace stdext;

class CSpriteManager{
	static CSpriteManager *m_pSpriteManagerSingleton;
protected:
	hash_map<char*, IDirect3DTexture9*>			  m_TextureCache;
	hash_map<char*, IDirect3DTexture9*>::iterator Iter;
public:
	static CSpriteManager *get(){
		if(m_pSpriteManagerSingleton == NULL)
			m_pSpriteManagerSingleton = new CSpriteManager();
		return m_pSpriteManagerSingleton;
	}
	IDirect3DTexture9 *getSprite(char *filename, 
		IDirect3DDevice9 *pDevice, D3DCOLOR colorKey);
};
#ifdef CObjectEx 
class CObjectManager{
	CSprite *getSprite(char *pfilename){}
	CXMesh  *getMesh(char *pfilename){}
	CFont   *getFont(char *pfilename){}
	inline CObject *pObject(char *pfilename){
		CObject *pObject = NULL;
		return NULL;
	}
protected:
	int numOfObject;
	map<int,  
	struct CObjectInfo{
		Object  *pObject;
		char	*textureFilename;
		int		 nObjectID;
		int collisionProfile;
	}
};
class CCollision{
}
	
};
#endif
#endif
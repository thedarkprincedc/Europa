#pragma once

#include <vector>
#include "SpaceShip.h"
#include "Game.h"
using namespace std;
static void SetUpLightsB(IDirect3DDevice9 *_pDevice);
static void SetUp3DCameraB(IDirect3DDevice9 *pD3DDevice);
class ScreenManager : public Game
{
public:
	static ScreenManager *getInstance();
public:
	void Initialize(HWND &hWnd);
	void Update();
	void Draw();
	void Shutdown();
protected:
	ScreenManager(void);
	~ScreenManager(void);
private:
	static ScreenManager *instance;
	vector<DrawableGameObject*> pGameObjectList;
};
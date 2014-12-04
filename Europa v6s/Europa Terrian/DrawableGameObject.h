#pragma once
#include "Game.h"

class DrawableGameObject
{
public:
	DrawableGameObject(Game *pGame);
	virtual ~DrawableGameObject(void);
	virtual void Update();
	virtual void Draw();
	Game *m_pGame;
};

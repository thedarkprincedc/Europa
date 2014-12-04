#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include <vector>
#include "ISteerable.h"
#include "DrawableGameObject.h"
using namespace std;
class PathFollowing 
{
public:
	PathFollowing();
	~PathFollowing(void);
	void addPoint(float x, int y, int z);
	void removePoint(int pointIndex);
protected:
};

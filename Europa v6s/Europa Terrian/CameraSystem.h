#pragma once
#include <map>
#include <string>
#include <utility>
#include "DummyCamera.h"
#include "DynamicCamera.h"
using namespace std;

static map<const char*, CDummyCamera*> cameraMap;
static int currentCamera = 0;
static CDummyCamera *createCamera(const char* cameraName)
{
	map<const char*, CDummyCamera*>::iterator iter = cameraMap.find(cameraName);
	if(iter != cameraMap.end())
		return (*iter).second;
	CDummyCamera *newCamera = new CDummyCamera();
	cameraMap[cameraName] = newCamera;
	return newCamera;
}
static CDummyCamera *createVarCamera(const char* cameraName, CDummyCamera *newCamera)
{
	map<const char*, CDummyCamera*>::iterator iter = cameraMap.find(cameraName);
	if(iter != cameraMap.end())
		return (*iter).second;
	cameraMap[cameraName] = newCamera;
	return newCamera;
}
static CDummyCamera *getCamera(string cameraName)
{
	map<const char*, CDummyCamera*>::iterator iter = cameraMap.find(cameraName.c_str());
	if(iter != cameraMap.end())
		return (*iter).second;
	return 0;
}
static CDummyCamera *getCamera(int cameraNumber = 0)
{
	map<const char*, CDummyCamera*>::iterator iter = cameraMap.begin();
	if(!cameraMap.empty())
	{
		//return
	}
	return 0;
}
static void deleteCamera(string cameraName)
{
	map<const char*, CDummyCamera*>::iterator iter = cameraMap.find(cameraName.c_str());
	if(iter != cameraMap.end())
		 cameraMap.erase(iter);
}
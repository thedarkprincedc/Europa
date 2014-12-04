#pragma once
#pragma comment(lib, "fmodex_vc.lib")
#include "../../api/inc/fmod.hpp"
#include "../../api/inc/fmod_errors.h"
FMOD::System*						_PSYSTEM;
FMOD_RESULT							_RESULT;
unsigned int						_VERSION;
hash_map<char *, FMOD::Sound*>		_SOUNDCACHE;
hash_map<char *, FMOD::Channel*>	_CHANNELS;
void ERRCHECK(FMOD_RESULT result){
    if (result != FMOD_OK){
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
    }
}
int INIT_FMOD(){
	_RESULT = FMOD::System_Create(&_PSYSTEM);
    ERRCHECK(_RESULT);
    _RESULT = _PSYSTEM->getVersion(&_VERSION);
    ERRCHECK(_RESULT);
    if (_VERSION < FMOD_VERSION){
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", _VERSION, FMOD_VERSION);
        return 0;
    }
    _RESULT = _PSYSTEM->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(_RESULT);
	return 0;
}
#define STREAM 0x0004
#define SOUND 0x0007

FMOD::Sound *LOADSOUND_FMOD(char *pStringSound, int soundType = SOUND){
	hash_map<char *, FMOD::Sound*>::iterator iter;
	if((iter = _SOUNDCACHE.find(pStringSound)) != _SOUNDCACHE.end())
		return (*iter).second;
	FMOD::Sound *pTmp = NULL;
	switch(soundType){
		case STREAM: _PSYSTEM->createStream(pStringSound, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &pTmp); break;
		case SOUND:	 _PSYSTEM->createSound(pStringSound, FMOD_HARDWARE, 0, &pTmp);	break;
	}
	pTmp->setMode(FMOD_LOOP_OFF);
	_SOUNDCACHE[pStringSound] = pTmp;
	return pTmp;
}
FMOD::Channel *PLAYSOUND_FMOD(char *pSoundString){
	hash_map<char *, FMOD::Sound*>::iterator iter;
	FMOD::Channel *pTmpChannel = NULL;
	if((iter = _SOUNDCACHE.find(pSoundString)) != _SOUNDCACHE.end()){
		_PSYSTEM->playSound(FMOD_CHANNEL_FREE, (*iter).second, false, &pTmpChannel);
		_CHANNELS[pSoundString] = pTmpChannel;
	}
	return pTmpChannel;
}
void STOPSOUND_FMOD(char *pSoundString){
	hash_map<char *, FMOD::Channel*>::iterator iter;
	if((iter = _CHANNELS.find(pSoundString)) != _CHANNELS.end()){
		(*iter).second->stop();
	}
}
void RELEASESOUND_FMOD(char *pSoundString){

}
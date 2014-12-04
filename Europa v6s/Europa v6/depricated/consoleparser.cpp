#include "europa.cpp"
static BOOL CommandParser(char *pStringCommand){
	char tmp[255];
	sscanf(pStringCommand, "%s", &tmp);
	MATCH(tmp, "GAMERESET"){
		GAMERESET();
	}
	MATCH(tmp, "POP"){
		pStateManager->Pop();
	}
	return TRUE;
}
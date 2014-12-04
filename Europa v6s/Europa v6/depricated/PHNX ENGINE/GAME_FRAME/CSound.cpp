#include "./Global_Framework.h"
/*
CSound::CSound()
	:	m_GraphBuilder(NULL), m_MediaControl(NULL),
		m_MediaEvent(NULL),	m_MediaPosition(NULL){
	Init();
}
CSound::CSound(char *pszSongName)
	:	m_GraphBuilder(NULL), m_MediaControl(NULL),
		m_MediaEvent(NULL),	m_MediaPosition(NULL){
	Init();
	Load(pszSongName);
}
CSound::~CSound(){	}
void CSound::Init(){
	m_duration	=	0;	
	m_position	=	0;			
	m_stoptime	=	0;			
	CoInitialize(NULL);
	if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
			  IID_IGraphBuilder, (void**)&m_GraphBuilder))){
		::OutputDebugString("Error: CSound Graphbuilder Failed");
		return;
	}
	m_GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_MediaControl);
	m_GraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_MediaEvent);
	m_GraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_MediaPosition);
}
HRESULT CSound::Load(char *pszSongName){
	//WCHAR	*wchSongName;
	//int		 length = 0;
	//strcpy(m_pFilename, pszSongName);
	//length = static_cast<int>(strlen(pszSongName)+1);
	//wchSongName = new WCHAR[length];
	//MultiByteToWideChar(CP_ACP, 0, pszSongName, -1,	
	//					wchSongName, length);
	WCHAR wchSongName[255];
	mbstowcs(wchSongName, pszSongName, 255);
	m_GraphBuilder->Release();
	if(!FAILED(m_GraphBuilder->RenderFile(wchSongName, NULL))){	//	Load Song
		//m_GraphBuilder->Release();
		m_MediaPosition->get_Duration(&m_duration);				//	Get Durations
		return S_OK;
	}
	
//	delete [] wchSongName;
	return E_FAIL;
}
void CSound::Play(){	
	m_MediaControl->Run();	
}
void CSound::Play(double position){
	m_MediaPosition->put_CurrentPosition(position);
	m_MediaControl->Run();
}
double CSound::Stop(){
	m_MediaControl->Stop();
	m_MediaPosition->get_StopTime(&m_stoptime);
	return m_stoptime;
}
void CSound::SetPosition(double position){
	m_MediaPosition->put_CurrentPosition(position);
}
double CSound::GetDuration(){
	return m_duration;
}
double CSound::GetPosition(){
	double position;
	m_MediaPosition->get_CurrentPosition(&position);
	return position;
}
const char *CSound::GetFilenameText(){
	return m_pFilename.c_str();
}
const char *CSound::GetDurationText(){
	char tempVar[16];
	return gcvt(GetDuration(), 5, tempVar);
}
const char *CSound::GetPositionText(){
	char tempVar[16];
	return gcvt(GetPosition(), 5, tempVar);
}*/
//string CSound::GetStatusText(){
	//char tempText[255];
	//strcpy(tempText, "*****Sound System*****\n");
	//strcat(tempText, "Filename: ");
	//strcat(tempText, m_pFilename);
	//strcat(tempText, "\nCurrent Position: ");
	//strcat(tempText, GetPositionText());
	//strcat(tempText, "s");
	//strcat(tempText, "\nDuration: ");
	//strcat(tempText, GetDurationText());
	//strcat(tempText, "s");
	/*scanf(tempText,"%s %s %s %s %s %s %s %s %s",
		"*****Sound System*****",
		"Filename: ", m_pFilename,
		"Current Position: ", GetPositionText(),"s",
		"Duration: ", GetDurationText(),"s");
	*/
/*	string tempText;
	scanf((char*)tempText.c_str(), "%s",
		"*****Sound System*****");
		//"Filename: ", m_pFilename,
		//"Current Position: ", GetPositionText(),"s",
		//"Duration: ", GetDurationText(),"s");
	return tempText;
}
BOOL CSound::ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent){
	char command[256], filename[256];
	sscanf((char*)pEvent->GetCommand().c_str(), "%s %s", command, filename);
	MATCH(command, "LoadSong"){
		Stop();
		Init();
		Load(filename);
		Play();
		return TRUE;
	}
	MATCH(command, "findfile"){
				
		return TRUE;
	}
	return FALSE;
}*/
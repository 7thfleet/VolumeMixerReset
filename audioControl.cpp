#include "audioControl.h"
#include "Windows.h"
#include "Audiopolicy.h"
#include "Audioclient.h"
#include "Mmdeviceapi.h"
#include "Unknwn.h"


extern "C"{
	#include <objbase.h>
}

#pragma comment(lib, "Ole32.lib")

//Matches the application volume level with the master volume lvel
void levelVolumes(){
	
	HRESULT hr;
	IMMDeviceEnumerator* pDeviceEnumerator = NULL;	//Release needed(?)

	//Create device enumerator
	hr = CoCreateInstance((CLSID)__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, (IID)__uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
	if(FAILED(hr)){throw hr;}
	
	//Get the default audio endpoint
	IMMDevice* pMMDevice = NULL;	//Release needed
	hr = pDeviceEnumerator->GetDefaultAudioEndpoint((EDataFlow)eRender, (ERole)eMultimedia, &pMMDevice);
	if(FAILED(hr)){throw hr;}

	//Create the session manager
	IAudioSessionManager2* pAudioSessionManager2 = NULL;	//Release needed
	hr = pMMDevice->Activate((REFIID)__uuidof(IAudioSessionManager2), (DWORD)CLSCTX_ALL, NULL, (void**)&pAudioSessionManager2);
	if(FAILED(hr)){throw hr;}

	//Create the session enumerator
	IAudioSessionEnumerator* pAudioSessionEnumerator = NULL;	//Release needed
	hr = pAudioSessionManager2->GetSessionEnumerator(&pAudioSessionEnumerator);
	if(FAILED(hr)){throw hr;}
	
	//Get the number of audio sessions
	int sessionCount = 0;
	hr = pAudioSessionEnumerator->GetCount(&sessionCount);
	if(FAILED(hr)){throw hr;}

	//For each audio session, set vol to master
	IAudioSessionControl* pAudioSessionControl = NULL;	//Release needed
	ISimpleAudioVolume* pSimpleAudioVolume = NULL;	//Release needed
	

	for(int i=0; i<sessionCount; i++){
		//Get the session controller
		hr = pAudioSessionEnumerator->GetSession((int)i, &pAudioSessionControl);
		if(FAILED(hr)){throw hr;}

		//Get the volume controller
			//Ugly unsupported way to get an ISimpleAudioVolume from an IAudioSessionControl
		hr = pAudioSessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&pSimpleAudioVolume);
		if(FAILED(hr)){throw hr;}
		
		//Set the volume
		hr = pSimpleAudioVolume->SetMasterVolume((float)1.0, (LPCGUID)NULL);
		if(FAILED(hr)){throw hr;}

		//Release interfaces
		pAudioSessionControl->Release();
		pAudioSessionControl=NULL;
		pSimpleAudioVolume->Release();
		pSimpleAudioVolume=NULL;
		
	}
	
	//Release other interfaces
	pAudioSessionEnumerator->Release();
	pAudioSessionEnumerator=NULL;
	pAudioSessionManager2->Release();
	pAudioSessionManager2=NULL;
	pMMDevice->Release();
	pMMDevice=NULL;
	pDeviceEnumerator->Release();
	pDeviceEnumerator=NULL;
	return;
}


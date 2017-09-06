#include "audioControl.h"
#include <iostream>
#include "Windows.h"
#include "comdef.h"


extern "C"{
	#include <objbase.h>
}

#pragma comment(lib, "Ole32.lib") 

int main(int argc, const char * argv[]){
	try{
		HRESULT hr = CoInitialize(nullptr);
		if(FAILED(hr)){throw hr;} 
		levelVolumes();
	}
	catch(HRESULT e){
		_com_error error(e);
		std::cout << "RAW: " << e << std::endl;
		std::cout << "com error: " << (LPCTSTR)(error.ErrorMessage()) << std::endl;
	}
	std::cout << "Ran" << std::endl;
	return 0;
}



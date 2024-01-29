#include "sg10.h"
#include "mywindow.h"

#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <tchar.h>

i16 ssdlevels = 2;
i16 dithercnt=0;
u8 scopy[400 * 240]; //handle last line overflow

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int nCmdShow){

	sg::Rect r(50,50,800,600);
	sg::Cmywindow *w = new sg::Cmywindow(r,WS_OVERLAPPEDWINDOW,NULL,(u16*)"Test");
	int res = w->Run();

	delete w;
	return res;
}
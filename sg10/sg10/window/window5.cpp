
#include "cdef.h"
#include "window5.h"


namespace sg
{
#pragma warning( push )

	u8 CWindow::cnt = 0;

	LRESULT CALLBACK CWindow::WndProcedure(HWND hWnd, UINT Msg,	WPARAM wParam, LPARAM lParam) {
#pragma warning( disable : 4312 )
		CWindow *self = (CWindow *)(GetWindowLongPtr(hWnd, GWL_USERDATA));
		LRESULT res = 0;
		if(self) {
			res = self->WndProc(Msg,wParam,lParam);
		}
		if ( res == 0 ) {
			switch(Msg) {
					case WM_DESTROY:						
						if( self->parent_hWnd==NULL ) {
							PostQuitMessage(0);											
						}								
						break;		

					case WM_NCCREATE:
#pragma warning( disable : 4244 )
						SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) ((CREATESTRUCT*)lParam)->lpCreateParams);
						break;

					case WM_PAINT: 
						PAINTSTRUCT ps;
						HDC hdc;
						hdc = BeginPaint(hWnd, &ps);	
						self->Paint(hdc);
						EndPaint(hWnd, &ps);
						break;

					case WM_SIZE: 
						{
							u16 w = LOWORD(lParam),h=HIWORD(lParam);
							if(self) self->Resize(w,h);					
						}
						break;
			}
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}


#pragma warning( pop ) 
};// sg

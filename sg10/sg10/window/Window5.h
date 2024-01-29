#pragma once

#include "cdef.h"
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include "obj.h"
#include "surface.h"
#include "winsurface.h"
#include "events.h"

using namespace sg::gr;

namespace sg{

	class CWindow {
	public:
		CWinSurface *surf;		
		Events *e;

		HWND hWnd,parent_hWnd;

		BITMAPINFO BitInfo;
		HDC hDC;
		HBITMAP hBitmap;

		static u8 cnt;

		CWindow(void){}

		CWindow(Rect r, u32 style,HWND parent_hWnd,u16 *name ) {
			surf = new CWinSurface(r.w,r.h);
			ATOM res;
			this->parent_hWnd = parent_hWnd;

			HINSTANCE hInstance=GetModuleHandle(NULL);
			if(cnt==0) {
				WNDCLASSEX	WndClsEx;
				WndClsEx.cbSize        = sizeof(WNDCLASSEX);
				WndClsEx.style         = 0;
				WndClsEx.lpfnWndProc   = WndProcedure;
				WndClsEx.cbClsExtra    = 0;
				WndClsEx.cbWndExtra    = 0;
				WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
				WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
				WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);				
				WndClsEx.lpszMenuName  = NULL;
				WndClsEx.lpszClassName = _T("siege");
				WndClsEx.hInstance     = hInstance;
				WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
				res = RegisterClassEx(&WndClsEx);			
			}	
			RECT rr={0,0,r.w,r.h};
			AdjustWindowRect(&rr,style,FALSE);
			hWnd = CreateWindow(_T("siege"),(LPCSTR)name,	style,r.x,r.y,rr.right-rr.left,rr.bottom-rr.top,parent_hWnd,NULL,hInstance,this);
			
			cnt++;
		}

		virtual ~CWindow(void) {			
			cnt--;
		}

		static LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT CALLBACK WndProc(UINT Msg,WPARAM wParam, LPARAM lParam)	{
			return 0;
		}

		virtual int Run(void) {			
			MSG msg;
			HINSTANCE hInstance=GetModuleHandle(NULL);			
			
			while (GetMessage(&msg, NULL, 0, 0)) {				
				TranslateMessage(&msg);
					DispatchMessage(&msg);				
			}
			return (int)msg.wParam;
		}

		void Resize(u16 w, u16 h) {
			delete surf;
			surf = new CWinSurface(w,h);
			OnResize(w,h);
			InvalidateRect(hWnd,0,0);			
		}

		virtual void OnResize(u16 w, u16 h){}

		virtual void Paint(HDC dc) {			
			surf->Paint(dc);		
		}

	};// window5

};// sg


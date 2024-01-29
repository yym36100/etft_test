#pragma once
#include "window5.h"
#include "SimpleFont.h"
#include "stmfont.h"

#include <stdio.h>
#include <math.h>
extern i16 ssdlevels;
extern i16 dithercnt;
namespace sg{

	class Cmywindow;
	class Cmywindow : public CWindow {
	public:
        enum { noimages = 5 +3+1+1+1+3+1+1} ;
		sg::res::CSimpleFont sf;
		sg::res::CstmFont stmf;
		sg::gr::CImg *img,*imgs[noimages];

		float brightness;
		float contrast;
		float gamma;
		int imgidx;

		Cmywindow(Rect r, u32 style,HWND parent_hWnd,u16 *name) : CWindow( r, style, parent_hWnd, name),sf(surf,black),stmf(surf) {
			//end ctor
			imgidx=0;

			brightness = 0;
			contrast = 0;
			gamma = 1;

			surf->do4 = 0;
			surf->do8 = 0;


			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
			//SetTimer(hWnd,9000,16,0);	
		}

#define M_LOG2E 1.44269504088896340736 // log2(e)

        inline long double log2(const long double x){
            return log(x) * M_LOG2E;
        }

		virtual void Paint(HDC dc) {		
			surf->color = white;
			surf->Clear();
			surf->color = red;

			static char text[100];
			sprintf(text,"test");

			sf.DrawT(0,0,text);

			surf->Paint(dc);		
		}
        u8 getbits(u32 offs){
            u8 *pdata = &scopy[offs];
            u8 res = 0;
            for (int i = 0; i < 8; i++){
                res |= *pdata++ ? (0x80 >> i) : 0;
            }
            return res;
        }
       
        
		virtual LRESULT CALLBACK WndProc(UINT Msg,WPARAM wParam, LPARAM lParam) {
			static bool playing = true;
			LRESULT res = 0;
			switch(Msg)
			{
			/*case WM_TIMER:
				InvalidateRect(hWnd,0,0);
				break;*/

			case WM_CHAR:
				switch(wParam)
				{				
                case 'q': PostQuitMessage(0); break;				
				}
				break;
			case WM_TIMER:
				dithercnt++; 
				InvalidateRect(hWnd, 0, 0); break;
			}
			return 0;
		}

	};
};// sg



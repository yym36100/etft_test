#pragma once
#include "window5.h"
#include "SimpleFont.h"
#include "stmfont.h"

#include <stdio.h>
#include <math.h>

#include "etft.h"
extern i16 ssdlevels;
extern i16 dithercnt;
namespace sg{

	HDC* pdc;
	CWinSurface *pws;

	void debugdraw(int n){
		if(n==3){
			pws->Paint(*pdc);
			Sleep(1);
		}
	};
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

		eTFT _tft;

		Cmywindow(Rect r, u32 style,HWND parent_hWnd,u16 *name) : CWindow( r, style, parent_hWnd, name),sf(surf,black),stmf(surf) {
			//end ctor
			imgidx=0;

			brightness = 0;
			contrast = 0;
			gamma = 1;

			surf->do4 = 0;
			surf->do8 = 0;

			_tft.surf=surf;
			
			//_tft.drawPixel(10,10,0);


			ShowWindow(hWnd, SW_SHOWNORMAL);
			UpdateWindow(hWnd);
			//SetTimer(hWnd,9000,16,0);	
		}

#define M_LOG2E 1.44269504088896340736 // log2(e)

        inline long double log2(const long double x){
            return log(x) * M_LOG2E;
        }

		virtual void Paint(HDC dc) {		
			static int once = 0;
			if(once ==0){
				once = 1;
				pws = surf;
				_tft.dbg = debugdraw;
			}
			pdc = &dc;

			surf->color = white;
			surf->Clear();
			surf->color = red;

			static char text[100];
			sprintf(text,"test");

			sf.DrawT(0,0,text);
			_tft.drawPixel(10,10,0);
			//_tft.drawSmoothArc(100,100,50,40,30,360-60,TFT_GREEN,TFT_WHITE,1);
			//_tft.drawSmoothArc(200,200,30,25,110,20,TFT_PURPLE,TFT_WHITE,0);
			//_tft.drawRoundRect(50,50,150,150,20,TFT_BLUE);
			//_tft.drawSmoothCircle(120,120,50,TFT_GOLD,TFT_WHITE);
			_tft.fillSmoothCircle(250,100,80,TFT_GOLD,TFT_WHITE);
			//_tft.drawSmoothRoundRect(100,100,7,7,100,80,TFT_BLUE,TFT_WHITE);

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




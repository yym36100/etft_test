#pragma once
#include "cdef.h"
#include <windows.h>

#include "surface.h"

namespace sg{
	namespace gr{

		class CWinSurface : public CSurface {
		public:

			BITMAPINFO BitInfo;
			HDC hDC;
			HBITMAP hBitmap;

			CWinSurface(void){}

			CWinSurface(u16 w, u16 h){
				this->w = w; 
				this->h = h;
				ZeroMemory(&BitInfo,sizeof(BitInfo));
				BitInfo.bmiHeader.biBitCount = 32;			// rgb 8 bytes for each component(3)
				BitInfo.bmiHeader.biCompression = BI_RGB;	// rgb = 3 components
				BitInfo.bmiHeader.biPlanes = 1;
				BitInfo.bmiHeader.biSize = sizeof(BitInfo.bmiHeader); // size of this struct
				BitInfo.bmiHeader.biWidth = w;		// width of window
				BitInfo.bmiHeader.biHeight = -h;	// height of window

				hDC = CreateCompatibleDC(0);
				void *p;
				hBitmap = CreateDIBSection(hDC, &BitInfo, DIB_RGB_COLORS, &p, 0, 0); // create a dib section for the dc
				pData = (u32*)p;

				u32 *p2=(u32*)pData;
				for(int i=0;i<w*h;i++)
				{				
					*p2++=white;				
				}
				SelectObject(hDC, hBitmap); // assign the dib section to the dc
			}

			void Paint(HDC hdc) {						
				BitBlt(hdc, 0, 0, w, h, hDC, 0, 0, SRCCOPY); // copy hdc to their hdc	
			}

			~CWinSurface(void) {
				DeleteObject(hBitmap);
				DeleteDC(hDC);
				pData = NULL;
			}
		};// CWinSurface

	};// gr
};// sg

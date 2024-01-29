#pragma once
#include "cdef.h"
#include "Surface.h"
#include "fonts.h"

namespace sg {
	namespace res {
		class CstmFont {		
		public:
			gr::CSurface *s;
			u32 Color;
			u32 bgc;
			u8 size;

			sFONT *pFont;
			
			CstmFont(gr::CSurface *ss,u32 cc= black):s(ss),Color(cc),size(1),bgc(white){
				pFont = &Font24;
			}
			~CstmFont(void){}

			void DrawC( u16 x, u16 y, u8 c) const;
			void DrawVC( u16 x, u16 y, u8 c) const;

			

			void DrawT(u16 x, u16 y, char *t)  { DrawT(x,y,(u8*)t); }
			void DrawTB(u16 x, u16 y, u8 const *t) const;

			void DrawVTB(u16 x, u16 y, u8 const *t) const;
			void DrawVT(u16 x, u16 y, u8 const *t) const;

			void DrawT(u16 x, u16 y, u8 const *t)  {
			while(*t) {
				DrawChar(x,y,*t++);
				x+=pFont->Width;
			}
		}

			void DrawChar(u16 Xpos, u16 Ypos,  u8 ch){
				u32 i = 0, j = 0;
				u16 height, width;
				u8  offset;
				u8  *pchar;
				u32 line;
				const u8 *c;

				height = pFont->Height;
				width  = pFont->Width;

				offset =  8 *((width + 7)/8) -  width ;

  //DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii-' ') *DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
				c = &pFont->table[(ch-' ') *pFont->Height * ((pFont->Width + 7) / 8)];



				for(i = 0; i < height; i++)
				{
					pchar = ((u8 *)c + (width + 7)/8 * i);

					switch(((width + 7)/8))
					{

					case 1:
						line =  pchar[0];      
						break;

					case 2:
						line =  (pchar[0]<< 8) | pchar[1];      
						break;

					case 3:
					default:
						line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];      
						break;
					} 

					for (j = 0; j < width; j++)
					{
						if(line & (1 << (width- j + offset- 1))) 
						{
							//BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
							s->SetPixel((Xpos + j), Ypos);	
						}
						else
						{
							//BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
							//s->SetPixel((Xpos + j), Ypos);	
						} 
					}
					Ypos++;
				}
			}

		};


	};// res
};// sg

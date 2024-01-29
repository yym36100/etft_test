#pragma once
#include "cdef.h"
#include "Surface.h"

namespace sg {
	namespace res {
		class CSimpleFont {		
		public:
			gr::CSurface *s;
			u32 Color;
			u32 bgc;
			u8 size;

			static u8 const TABLE5[];
			CSimpleFont(gr::CSurface *ss,u32 cc):s(ss),Color(cc),size(1),bgc(white){}
			~CSimpleFont(void){}

			void DrawC( u16 x, u16 y, u8 c) const;
			void DrawVC( u16 x, u16 y, u8 c) const;

			void DrawT(u16 x, u16 y, u8 const *t) const;

			void DrawT(u16 x, u16 y, char *t) const { DrawT(x,y,(u8*)t); }
			void DrawTB(u16 x, u16 y, u8 const *t) const;

			void DrawVTB(u16 x, u16 y, u8 const *t) const;
			void DrawVT(u16 x, u16 y, u8 const *t) const;

		};


	};// res
};// sg

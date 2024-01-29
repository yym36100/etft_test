#pragma once
#include "cdef.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _tFont
	{    
		const u8 *table;
		u16 Width;
		u16 Height;

	} sFONT;

	extern sFONT Font24;
	extern sFONT Font20;
	extern sFONT Font16;
	extern sFONT Font12;
	extern sFONT Font8;

#ifdef __cplusplus
}
#endif

//#define LINE(x) ((x) * (((sFONT *)BSP_LCD_GetFont())->Height))

#pragma once
#include "stdint.h"
#include <math.h>


#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

const float deg2rad      = 3.14159265359f/180.0f;
const float PixelAlphaGain   = 255.0;
const float LoAlphaTheshold  = 1.0/32.0;
const float HiAlphaTheshold  = 1.0 - LoAlphaTheshold;


template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }

// Fast alphaBlend
template <typename A, typename F, typename B> static inline uint16_t
fastBlend(A alpha, F fgc, B bgc)
{
	// Split out and blend 5-bit red and blue channels
	uint32_t rxb = bgc & 0xF81F;
	rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
	// Split out and blend 6-bit green channel
	uint32_t xgx = bgc & 0x07E0;
	xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
	// Recombine channels
	return (rxb & 0xF81F) | (xgx & 0x07E0);
}

//class CSurface;

class eTFT{
public:

	CSurface *surf;
	void (*dbg)(int lvl);

	// Viewport variables
	int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
	int32_t  _xDatum;
	int32_t  _yDatum;
	int32_t  _xWidth;
	int32_t  _yHeight;
	bool     _vpDatum;
	bool     _vpOoB;


	eTFT(void){
		_vpX = 0;
		_vpY=0;
		_vpW=800;
		_vpH=600;    // Note: x start, y start, x end + 1, y end + 1
		_xDatum=0;
		_yDatum=0;
		_xWidth=800;
		_yHeight=600;
		_vpDatum = false;
		_vpOoB = false;;

	}

	void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color)
{
  
  // smarter version
  drawFastHLine(x + r  , y    , w - r - r, color); // Top
  drawFastHLine(x + r  , y + h - 1, w - r - r, color); // Bottom
  drawFastVLine(x    , y + r  , h - r - r, color); // Left
  drawFastVLine(x + w - 1, y + r  , h - r - r, color); // Right
  // draw four corners
  drawCircleHelper(x + r    , y + r    , r, 1, color);
  drawCircleHelper(x + w - r - 1, y + r    , r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(x + r    , y + h - r - 1, r, 8, color);
  
}
	void drawCircleHelper( int32_t x0, int32_t y0, int32_t rr, uint8_t cornername, uint32_t color)
{
  if (rr <= 0) return;
  int32_t f     = 1 - rr;
  int32_t ddF_x = 1;
  int32_t ddF_y = -2 * rr;
  int32_t xe    = 0;
  int32_t xs    = 0;
  int32_t len   = 0;

 
  do
  {
    while (f < 0) {
      ++xe;
      f += (ddF_x += 2);
    }
    f += (ddF_y += 2);

    if (xe-xs==1) {
      if (cornername & 0x1) { // left top
        drawPixel(x0 - xe, y0 - rr, color);
        drawPixel(x0 - rr, y0 - xe, color);
      }
      if (cornername & 0x2) { // right top
        drawPixel(x0 + rr    , y0 - xe, color);
        drawPixel(x0 + xs + 1, y0 - rr, color);
      }
      if (cornername & 0x4) { // right bottom
        drawPixel(x0 + xs + 1, y0 + rr    , color);
        drawPixel(x0 + rr, y0 + xs + 1, color);
      }
      if (cornername & 0x8) { // left bottom
        drawPixel(x0 - rr, y0 + xs + 1, color);
        drawPixel(x0 - xe, y0 + rr    , color);
      }
    }
    else {
      len = xe - xs++;
      if (cornername & 0x1) { // left top
        drawFastHLine(x0 - xe, y0 - rr, len, color);
        drawFastVLine(x0 - rr, y0 - xe, len, color);
      }
      if (cornername & 0x2) { // right top
        drawFastVLine(x0 + rr, y0 - xe, len, color);
        drawFastHLine(x0 + xs, y0 - rr, len, color);
      }
      if (cornername & 0x4) { // right bottom
        drawFastHLine(x0 + xs, y0 + rr, len, color);
        drawFastVLine(x0 + rr, y0 + xs, len, color);
      }
      if (cornername & 0x8) { // left bottom
        drawFastVLine(x0 - rr, y0 + xs, len, color);
        drawFastHLine(x0 - xe, y0 + rr, len, color);
      }
    }
    xs = xe;
  } while (xe < rr--);

 
}



	inline uint8_t sqrt_fraction(uint32_t num) {
		if (num > (0x40000000)) return 0;
		uint32_t bsh = 0x00004000;
		uint32_t fpr = 0;
		uint32_t osh = 0;

		// Auto adjust from U8:8 up to U15:16
		while (num>bsh) {bsh <<= 2; osh++;}

		do {
			uint32_t bod = bsh + fpr;
			if(num >= bod)
			{
				num -= bod;
				fpr = bsh + bod;
			}
			num <<= 1;
		} while(bsh >>= 1);

		return fpr>>osh;
	}

	uint32_t color16to24(uint16_t color565)
{
  uint8_t r = (color565 >> 8) & 0xF8; r |= (r >> 5);
  uint8_t g = (color565 >> 3) & 0xFC; g |= (g >> 6);
  uint8_t b = (color565 << 3) & 0xF8; b |= (b >> 5);

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | ((uint32_t)b << 0);
}

	void drawPixel(int32_t x, int32_t y, uint32_t color){

		color = color16to24(color);
		dbg(3);
		surf->color = color;
		surf->SetPixel(x,y);
	}
	uint16_t drawPixel(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color = 0x00FFFFFF){

  if (bg_color == 0x00FFFFFF) bg_color = readPixel(x, y);
  color = fastBlend(alpha, color, bg_color);
  drawPixel(x, y, color);
  return color;
}
	uint16_t readPixel(int32_t x, int32_t y){
		
		return 0xffff;//surf->GetPixel(x,y);
	}

	float wedgeLineDistance(float xpax, float ypay, float bax, float bay, float dr){
		float h = max(min((xpax * bax + ypay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
		float dx = xpax - bax * h, dy = ypay - bay * h;
		return sqrtf(dx * dx + dy * dy) + h * dr;
	}

	bool clipWindow(int32_t *xs, int32_t *ys, int32_t *xe, int32_t *ye)
	{
		if (_vpOoB) return false; // Area is outside of viewport

		*xs+= _xDatum;
		*ys+= _yDatum;
		*xe+= _xDatum;
		*ye+= _yDatum;

		if ((*xs >= _vpW) || (*ys >= _vpH)) return false;  // Area is outside of viewport
		if ((*xe <  _vpX) || (*ye <  _vpY)) return false;  // Area is outside of viewport

		// Crop drawing area bounds
		if (*xs < _vpX) *xs = _vpX;
		if (*ys < _vpY) *ys = _vpY;

		if (*xe > _vpW) *xe = _vpW - 1;
		if (*ye > _vpH) *ye = _vpH - 1;

		return true;  // Area is wholly or partially inside viewport
	}

	// draw an anti-aliased line with different width radiused ends
	void drawWedgeLine(float ax, float ay, float bx, float by, float ar, float br, uint32_t fg_color, uint32_t bg_color) {
		if ( (ar < 0.0) || (br < 0.0) )return;
		if ( (fabsf(ax - bx) < 0.01f) && (fabsf(ay - by) < 0.01f) ) bx += 0.01f;  // Avoid divide by zero

		// Find line bounding box
		int32_t x0 = (int32_t)floorf(min(ax-ar, bx-br));
		int32_t x1 = (int32_t) ceilf(max(ax+ar, bx+br));
		int32_t y0 = (int32_t)floorf(min(ay-ar, by-br));
		int32_t y1 = (int32_t) ceilf(max(ay+ar, by+br));

		if (!clipWindow(&x0, &y0, &x1, &y1)) return;

		// Establish x start and y start
		int32_t ys = ay;
		if ((ax-ar)>(bx-br)) ys = by;

		float rdt = ar - br; // Radius delta
		float alpha = 1.0f;
		ar += 0.5f;

		uint16_t bg = bg_color;
		float xpax, ypay, bax = bx - ax, bay = by - ay;

		int32_t xs = x0;
		// Scan bounding box from ys down, calculate pixel intensity from distance to line
		for (int32_t yp = ys; yp <= y1; yp++) {
			bool swin = true;  // Flag to start new window area
			bool endX = false; // Flag to skip pixels
			ypay = yp - ay;
			for (int32_t xp = xs; xp <= x1; xp++) {
				if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side
				xpax = xp - ax;
				alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
				if (alpha <= LoAlphaTheshold ) continue;
				// Track edge to minimise calculations
				if (!endX) { endX = true; xs = xp; }
				if (alpha > HiAlphaTheshold) {
					drawPixel(xp, yp, fg_color);
					continue;
				}
				//Blend color with background and plot
				if (bg_color == 0x00FFFFFF) {
					bg = readPixel(xp, yp); swin = true;
				}
				uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
				drawPixel(xp, yp, pcol);
				swin = swin;
			}
		}

		// Reset x start to left side of box
		xs = x0;
		// Scan bounding box from ys-1 up, calculate pixel intensity from distance to line
		for (int32_t yp = ys-1; yp >= y0; yp--) {
			bool swin = true;  // Flag to start new window area
			bool endX = false; // Flag to skip pixels
			ypay = yp - ay;
			for (int32_t xp = xs; xp <= x1; xp++) {
				if (endX) if (alpha <= LoAlphaTheshold) break;  // Skip right side of drawn line
				xpax = xp - ax;
				alpha = ar - wedgeLineDistance(xpax, ypay, bax, bay, rdt);
				if (alpha <= LoAlphaTheshold ) continue;
				// Track line boundary
				if (!endX) { endX = true; xs = xp; }
				if (alpha > HiAlphaTheshold) {

					drawPixel(xp, yp, fg_color);
					continue;
				}
				//Blend colour with background and plot
				if (bg_color == 0x00FFFFFF) {
					bg = readPixel(xp, yp); swin = true;
				}

				uint16_t pcol = fastBlend((uint8_t)(alpha * PixelAlphaGain), fg_color, bg);
				drawPixel(xp, yp, pcol);
				swin = swin;
			}
		}
	}


	void drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color){  
		drawWedgeLine( ax, ay, ax, ay, r, r, fg_color, bg_color);
	}

	void drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds) {

		if (endAngle != startAngle && (startAngle != 0 || endAngle != 360))
		{
			float sx = -sinf(startAngle * deg2rad);
			float sy = +cosf(startAngle * deg2rad);
			float ex = -sinf(  endAngle * deg2rad);
			float ey = +cosf(  endAngle * deg2rad);

			if (roundEnds)
			{ // Round ends
				sx = sx * (r + ir)/2.0f + x;
				sy = sy * (r + ir)/2.0f + y;
				drawSpot(sx, sy, (r - ir)/2.0, fg_color, bg_color);

				ex = ex * (r + ir)/2.0f + x;
				ey = ey * (r + ir)/2.0f + y;
				drawSpot(ex, ey, (r - ir)/2.0f, fg_color, bg_color);
			}
			else
			{ // Square ends
				float asx = sx * ir + x;
				float asy = sy * ir + y;
				float aex = sx *  r + x;
				float aey = sy *  r + y;
				drawWedgeLine(asx, asy, aex, aey, 0.3f, 0.3f, fg_color, bg_color);

				asx = ex * ir + x;
				asy = ey * ir + y;
				aex = ex *  r + x;
				aey = ey *  r + y;
				drawWedgeLine(asx, asy, aex, aey, 0.3f, 0.3f, fg_color, bg_color);
			}

			// Draw arc
			drawArc(x, y, r, ir, startAngle, endAngle, fg_color, bg_color);

		}
		else // Draw full 360
		{
			drawArc(x, y, r, ir, 0, 360, fg_color, bg_color);
		}
	}

	void drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smooth=true) {
		if (endAngle   > 360)   endAngle = 360;
		if (startAngle > 360) startAngle = 360;
		if (_vpOoB || startAngle == endAngle) return;
		if (r < ir) transpose(r, ir);  // Required that r > ir
		if (r <= 0 || ir < 0) return;  // Invalid r, ir can be zero (circle sector)

		if (endAngle < startAngle) {
			// Arc sweeps through 6 o'clock so draw in two parts
			if (startAngle < 360) drawArc(x, y, r, ir, startAngle, 360, fg_color, bg_color, smooth);
			if (endAngle == 0) return;
			startAngle = 0;
		}		

		int32_t xs = 0;        // x start position for quadrant scan
		uint8_t alpha = 0;     // alpha value for blending pixels

		uint32_t r2 = r * r;   // Outer arc radius^2
		if (smooth) r++;       // Outer AA zone radius
		uint32_t r1 = r * r;   // Outer AA radius^2
		int16_t w  = r - ir;   // Width of arc (r - ir + 1)
		uint32_t r3 = ir * ir; // Inner arc radius^2
		if (smooth) ir--;      // Inner AA zone radius
		uint32_t r4 = ir * ir; // Inner AA radius^2

		//     1 | 2
		//    ---¦---    Arc quadrant index
		//     0 | 3
		// Fixed point U16.16 slope table for arc start/end in each quadrant
		uint32_t startSlope[4] = {0, 0, 0xFFFFFFFF, 0};
		uint32_t   endSlope[4] = {0, 0xFFFFFFFF, 0, 0};

		// Ensure maximum U16.16 slope of arc ends is ~ 0x8000 0000
		const float minDivisor = 1.0f/0x8000;

		// Fill in start slope table and empty quadrants
		float fabscos = fabsf(cosf(startAngle * deg2rad));
		float fabssin = fabsf(sinf(startAngle * deg2rad));

		// U16.16 slope of arc start
		uint32_t slope = (fabscos/(fabssin + minDivisor)) * (float)(1UL<<16);

		// Update slope table, add slope for arc start
		if (startAngle <= 90) {
			startSlope[0] =  slope;
		}
		else if (startAngle <= 180) {
			startSlope[1] =  slope;
		}
		else if (startAngle <= 270) {
			startSlope[1] = 0xFFFFFFFF;
			startSlope[2] = slope;
		}
		else {
			startSlope[1] = 0xFFFFFFFF;
			startSlope[2] =  0;
			startSlope[3] = slope;
		}

		// Fill in end slope table and empty quadrants
		fabscos  = fabsf(cosf(endAngle * deg2rad));
		fabssin  = fabsf(sinf(endAngle * deg2rad));

		// U16.16 slope of arc end
		slope   = (uint32_t)((fabscos/(fabssin + minDivisor)) * (float)(1UL<<16));

		// Work out which quadrants will need to be drawn and add slope for arc end
		if (endAngle <= 90) {
			endSlope[0] = slope;
			endSlope[1] =  0;
			startSlope[2] =  0;
		}
		else if (endAngle <= 180) {
			endSlope[1] = slope;
			startSlope[2] =  0;
		}
		else if (endAngle <= 270) {
			endSlope[2] =  slope;
		}
		else {
			endSlope[3] =  slope;
		}

		// Scan quadrant
		for (int32_t cy = r - 1; cy > 0; cy--)
		{
			dbg(0);
			uint32_t len[4] = { 0,  0,  0,  0}; // Pixel run length
			int32_t  xst[4] = {-1, -1, -1, -1}; // Pixel run x start
			uint32_t dy2 = (r - cy) * (r - cy);

			// Find and track arc zone start point
			while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

			for (int32_t cx = xs; cx < r; cx++)
			{
				dbg(1);
				// Calculate radius^2
				uint32_t hyp = (r - cx) * (r - cx) + dy2;

				// If in outer zone calculate alpha
				if (hyp > r2) {
					alpha = ~sqrt_fraction(hyp); // Outer AA zone
				}
				// If within arc fill zone, get line start and lengths for each quadrant
				else if (hyp >= r3) {
					// Calculate U16.16 slope
					slope = ((r - cy) << 16)/(r - cx);
					if (slope <= startSlope[0] && slope >= endSlope[0]) { // slope hi -> lo
						xst[0] = cx; // Bottom left line end
						len[0]++;
					}
					if (slope >= startSlope[1] && slope <= endSlope[1]) { // slope lo -> hi
						xst[1] = cx; // Top left line end
						len[1]++;
					}
					if (slope <= startSlope[2] && slope >= endSlope[2]) { // slope hi -> lo
						xst[2] = cx; // Bottom right line start
						len[2]++;
					}
					if (slope <= endSlope[3] && slope >= startSlope[3]) { // slope lo -> hi
						xst[3] = cx; // Top right line start
						len[3]++;
					}
					continue; // Next x
				}
				else {
					if (hyp <= r4) break;  // Skip inner pixels
					alpha = sqrt_fraction(hyp); // Inner AA zone
				}

				if (alpha < 16) continue;  // Skip low alpha pixels

				// If background is read it must be done in each quadrant
				uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
				// Check if an AA pixels need to be drawn
				slope = ((r - cy)<<16)/(r - cx);
				if (slope <= startSlope[0] && slope >= endSlope[0]) // BL
					drawPixel(x + cx - r, y - cy + r, pcol);
				if (slope >= startSlope[1] && slope <= endSlope[1]) // TL
					drawPixel(x + cx - r, y + cy - r, pcol);
				if (slope <= startSlope[2] && slope >= endSlope[2]) // TR
					drawPixel(x - cx + r, y + cy - r, pcol);
				if (slope <= endSlope[3] && slope >= startSlope[3]) // BR
					drawPixel(x - cx + r, y - cy + r, pcol);
			}
			// Add line in inner zone
			if (len[0]) drawFastHLine(x + xst[0] - len[0] + 1 - r, y - cy + r, len[0], fg_color); // BL
			if (len[1]) drawFastHLine(x + xst[1] - len[1] + 1 - r, y + cy - r, len[1], fg_color); // TL
			if (len[2]) drawFastHLine(x - xst[2] + r, y + cy - r, len[2], fg_color); // TR
			if (len[3]) drawFastHLine(x - xst[3] + r, y - cy + r, len[3], fg_color); // BR
		}

		// Fill in centre lines
		//fg_color=TFT_GOLD;

		if (startAngle ==   0 || endAngle == 360) drawFastVLine(x, y + r - w, w, fg_color); // Bottom
		if (startAngle <=  90 && endAngle >=  90) drawFastHLine(x - r + 1, y, w, fg_color); // Left
		if (startAngle <= 180 && endAngle >= 180) drawFastVLine(x, y - r + 1, w, fg_color); // Top
		if (startAngle <= 270 && endAngle >= 270) drawFastHLine(x + r - w, y, w, fg_color); // Right

	}

	void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
	{
		if (_vpOoB) return;

		x+= _xDatum;
		y+= _yDatum;

		// Clipping
		if ((y < _vpY) || (x >= _vpW) || (y >= _vpH)) return;

		if (x < _vpX) { w += x - _vpX; x = _vpX; }

		if ((x + w) > _vpW) w = _vpW - x;

		if (w < 1) return;

		for(int i=x;i<=x+w-1;i++){
			drawPixel(i,y,color);
		}

		//setWindow(x, y, x + w - 1, y);

		//pushBlock(color, w);
		//todo


	}

	void drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color)
	{
		if (_vpOoB) return;

		x+= _xDatum;
		y+= _yDatum;

		// Clipping
		if ((x < _vpX) || (x >= _vpW) || (y >= _vpH)) return;

		if (y < _vpY) { h += y - _vpY; y = _vpY; }

		if ((y + h) > _vpH) h = _vpH - y;

		if (h < 1) return;


		//setWindow(x, y, x, y + h - 1);
		for(int i=y;i<=y+h-1;i++){
			drawPixel(x,i,color);
		}

		//pushBlock(color, h);
		//todo

	}

	void drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color)
{
  drawSmoothRoundRect(x-r, y-r, r, r-1, 0, 0, fg_color, bg_color);
}
	void fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color)
{
  if (r <= 0) return;

  

  drawFastHLine(x - r, y, 2 * r + 1, color);
  int32_t xs = 1;
  int32_t cx = 0;

  int32_t r1 = r * r;
  r++;
  int32_t r2 = r * r;
  
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t dy2 = (r - cy) * (r - cy);
    for (cx = xs; cx < r; cx++)
    {
      int32_t hyp2 = (r - cx) * (r - cx) + dy2;
      if (hyp2 <= r1) break;
      if (hyp2 >= r2) continue;

      uint8_t alpha = ~sqrt_fraction(hyp2);
      if (alpha > 246) break;
      xs = cx;
      if (alpha < 9) continue;

      if (bg_color == 0x00FFFFFF) {
        drawPixel(x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y - cy + r, color, alpha, bg_color);
        drawPixel(x + cx - r, y - cy + r, color, alpha, bg_color);
      }
      else {
        uint16_t pcol = drawPixel(x + cx - r, y + cy - r, color, alpha, bg_color);
        drawPixel(x - cx + r, y + cy - r, pcol);
        drawPixel(x - cx + r, y - cy + r, pcol);
        drawPixel(x + cx - r, y - cy + r, pcol);
      }
    }
    drawFastHLine(x + cx - r, y + cy - r, 2 * (r - cx) + 1, color);
    drawFastHLine(x + cx - r, y - cy + r, 2 * (r - cx) + 1, color);
  }
  
}

// x,y is top left corner of bounding box for a complete rounded rectangle
// r = arc outer corner radius, ir = arc inner radius. Arc thickness = r-ir+1
// w and h are width and height of the bounding rectangle
// If w and h are < radius (e.g. 0,0) a circle will be drawn with centre at x+r,y+r
// Arc foreground fg_color anti-aliased with background colour at edges
// A subset of corners can be drawn by specifying a quadrants mask. A bit set in the
// mask means draw that quadrant (all are drawn if parameter missing):
//   0x1 | 0x2
//    ---Â¦---    Arc quadrant mask select bits (as in drawCircleHelper fn)
//   0x8 | 0x4
void drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color, uint8_t quadrants=0x0f)
{
  if (_vpOoB) return;
  if (r < ir) transpose(r, ir); // Required that r > ir
  if (r <= 0 || ir < 0) return; // Invalid

  w -= 2*r;
  h -= 2*r;

  if (w < 0) w = 0;
  if (h < 0) h = 0;
  

  x += r;
  y += r;

  uint16_t t = r - ir + 1;
  int32_t xs = 0;
  int32_t cx = 0;

  int32_t r2 = r * r;   // Outer arc radius^2
  r++;
  int32_t r1 = r * r;   // Outer AA zone radius^2

  int32_t r3 = ir * ir; // Inner arc radius^2
  ir--;
  int32_t r4 = ir * ir; // Inner AA zone radius^2

  uint8_t alpha = 0;

  // Scan top left quadrant x y r ir fg_color  bg_color
  for (int32_t cy = r - 1; cy > 0; cy--)
  {
    int32_t len = 0;  // Pixel run length
    int32_t lxst = 0; // Left side run x start
    int32_t rxst = 0; // Right side run x start
    int32_t dy2 = (r - cy) * (r - cy);

    // Find and track arc zone start point
    while ((r - xs) * (r - xs) + dy2 >= r1) xs++;

    for (cx = xs; cx < r; cx++)
    {
      // Calculate radius^2
      int32_t hyp = (r - cx) * (r - cx) + dy2;

      // If in outer zone calculate alpha
      if (hyp > r2) {
        alpha = ~sqrt_fraction(hyp); // Outer AA zone
      }
      // If within arc fill zone, get line lengths for each quadrant
      else if (hyp >= r3) {
        rxst = cx; // Right side start
        len++;     // Line segment length
        continue;  // Next x
      }
      else {
        if (hyp <= r4) break;  // Skip inner pixels
        alpha = sqrt_fraction(hyp); // Inner AA zone
      }

      if (alpha < 16) continue;  // Skip low alpha pixels

      // If background is read it must be done in each quadrant - TODO
      uint16_t pcol = fastBlend(alpha, fg_color, bg_color);
      if (quadrants & 0x8) drawPixel(x + cx - r, y - cy + r + h, pcol);     // BL
      if (quadrants & 0x1) drawPixel(x + cx - r, y + cy - r, pcol);         // TL
      if (quadrants & 0x2) drawPixel(x - cx + r + w, y + cy - r, pcol);     // TR
      if (quadrants & 0x4) drawPixel(x - cx + r + w, y - cy + r + h, pcol); // BR
    }
    // Fill arc inner zone in each quadrant
    lxst = rxst - len + 1; // Calculate line segment start for left side
    if (quadrants & 0x8) drawFastHLine(x + lxst - r, y - cy + r + h, len, fg_color);     // BL
    if (quadrants & 0x1) drawFastHLine(x + lxst - r, y + cy - r, len, fg_color);         // TL
    if (quadrants & 0x2) drawFastHLine(x - rxst + r + w, y + cy - r, len, fg_color);     // TR
    if (quadrants & 0x4) drawFastHLine(x - rxst + r + w, y - cy + r + h, len, fg_color); // BR
  }

  // Draw sides
  if ((quadrants & 0xC) == 0xC) fillRect(x, y + r - t + h, w + 1, t, fg_color); // Bottom
  if ((quadrants & 0x9) == 0x9) fillRect(x - r + 1, y, t, h + 1, fg_color);     // Left
  if ((quadrants & 0x3) == 0x3) fillRect(x, y - r + 1, w + 1, t, fg_color);     // Top
  if ((quadrants & 0x6) == 0x6) fillRect(x + r - t + w, y, t, h + 1, fg_color); // Right
  
}

void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }
  if (y < _vpY) { h += y - _vpY; y = _vpY; }

  if ((x + w) > _vpW) w = _vpW - x;
  if ((y + h) > _vpH) h = _vpH - y;

  if ((w < 1) || (h < 1)) return;

  
//  setWindow(x, y, x + w - 1, y + h - 1);
//  pushBlock(color, w * h);
  sg::Rect r;
  r.x=x;
  r.y=y;
  r.w=w;
  r.h=h;
  surf->color = color16to24(color);
  surf->FillRect(r);

  
}

};
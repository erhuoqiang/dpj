#ifndef __GUI_H__
#define __GUI_H__

#include "stm32f10x.h"

//--------------------------------------------------------------------------------------
void GUI_Init(void);                                              
bool GUI_IsFontRdy(void);                                        
bool GUI_DownLoadFont(void);                                     
bool GUI_PrtScreen(void);
bool GUI_SetBgLight(u8 Value);
u8 GUI_GetBgLightVal(void);
void GUI_SetLand(bool LandMode);
bool GUI_GetLandMode(void);
void GUI_SetBgr(bool UseBgr);
void GUI_FillScreen(u16 Color);
bool GUI_Fill(u16 x,u16 y,u16 width,u16 hight,u16 Color);
u16 GUI_ReadBit16Point(u16 x,u16 y);
void GUI_ReadBit24Point(u16 x,u16 y,u8 *Buf);
u16 GUI_DrawGBK16(u16 x,u16 y,u16 width,u16 hight,const u8 *Str,u16 Color);
void GUI_DrawPicFromSpiFlash(u32 StartPageAddr,u16 x,u16 y,u16 width,u16 hight);
bool GUI_DrawPicBinFromSD(const u8 * pPath);
bool GUI_DrawPicBmpFromSD(const u8 * pPath);
//--------------------------------------------------------------------------------------
#endif

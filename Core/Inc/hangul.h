/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef __hangul_h__
#define __hangul_h__

#include "main.h"
#include "hancode.h"

void outeng(uint8_t colum, uint8_t row, char eng);
void outhan(uint8_t colum, uint8_t row, unsigned char hanx1, unsigned char hanx2);
void outtextxy_engs(uint8_t colum, uint8_t row, char *engbuf);
void outtextxy_hans(uint8_t colum, uint8_t row, char *hanbuf);
void outtextxy68_fonts(char x, char y, char *snumbuf, _Bool invert);

#endif

/* [] END OF FILE */

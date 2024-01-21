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

#ifndef __hancode_h__
#define __hancode_h__

#include "main.h"

char *gethanimage(unsigned char hanx1, unsigned char hanx2);
char *getascimage(unsigned char c);
char *get68image(unsigned char c);
char *geticonimage(unsigned char u1);
unsigned short hconv(unsigned short hhcode);

#endif

/* [] END OF FILE */

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

#include "hangul.h"
#include "ssd1306.h"

// 00 - 잠시 기다리십시오.
// 01 -  Ver. 1.01a
char hsbuff[2][25] = {"시은짱 은비짱 ", "우리가족 사랑해 "};
extern uint8_t SSD1306_Buffer[1024];
                             
#define LCDTIME	1

void outengu(unsigned char eng);

void testLCD(void)
{
	ssd1306_Fill(0);
	
	//outtextxy_hanf(0, 8,hsbuff[0]);	// 잠시 기다리십시오.
	//outtextxy_hans(0,32,hsbuff[1]);	// ver. 0.10a
}

uint8_t shiftBit(uint8_t inData, int8_t pos) {
    uint8_t result;
    if(pos < 0) {
        result = inData >> abs(pos);
    }
    else {
        result = inData << pos;
    }
    return result;
}

void outeng(uint8_t colum, uint8_t row, char eng) {
	char	*image;
	
	image = getascimage(eng);
	
    //row *= 16;
    for(int i = 0; i < 8; i++) {
    	SSD1306_Buffer[colum + ((row+i)*16)] = (shiftBit(image[0], 7 - i) & 0x80) |
                                              (shiftBit(image[1], 6 - i) & 0x40) |
                                              (shiftBit(image[2], 5 - i) & 0x20) |
                                              (shiftBit(image[3], 4 - i) & 0x10) |
                                              (shiftBit(image[4], 3 - i) & 0x08) |
                                              (shiftBit(image[5], 2 - i) & 0x04) |
                                              (shiftBit(image[6], 1 - i) & 0x02) |
                                              (shiftBit(image[7], 0 - i) & 0x01);
    }
    for(int i = 0; i < 8; i++) {
    	SSD1306_Buffer[colum + ((row+i+8)*16)] = (shiftBit(image[8], 7 - i) & 0x80) |
                                                (shiftBit(image[9], 6 - i) & 0x40) |
                                                (shiftBit(image[10], 5 - i) & 0x20) |
                                                (shiftBit(image[11], 4 - i) & 0x10) |
                                                (shiftBit(image[12], 3 - i) & 0x08) |
                                                (shiftBit(image[13], 2 - i) & 0x04) |
                                                (shiftBit(image[14], 1 - i) & 0x02) |
                                                (shiftBit(image[15], 0 - i) & 0x01);
    }
}

void outhan(uint8_t colum, uint8_t row, unsigned char hanx1, unsigned char hanx2) {
	char	*image;
	unsigned short hh;
	
	hh = (unsigned short)((unsigned short)(hanx1)<<(unsigned short)8) | (unsigned short)hanx2;
	
	hh = hconv(hh); // 완성형을 조합형으로 변환
	
	hanx1 = (hh >> 8) & 0x00ff;
	hanx2 = hh & 0x00ff;

	image = gethanimage(hanx1, hanx2);
	
	memcpy(&SSD1306_Buffer[(colum*16)+(row*256)], image, 16);
	memcpy(&SSD1306_Buffer[(colum*16)+128+(row*256)], &image[16], 16);
}

void outtextxy_engs(uint8_t colum, uint8_t row, char *engbuf) {
	while(*engbuf) {
		outeng(colum++, row, *engbuf++);
	}
}

void outtextxy_hans(uint8_t colum, uint8_t row, char *hanbuf) {
	unsigned char i = 0;
    
	while(hanbuf[i]) {
		if(hanbuf[i] & 0x80) {
			outhan(colum, row, hanbuf[i], hanbuf[i+1]);
      colum++;
			i+=2;
		}
		else {  
			outeng(colum++, row, hanbuf[i]);
            i++;
		}
	}
}

// 0000 0011 1111 2222 2233 3333
//0-
// 16:21 8:6 4:3
void outtext68_font(char x, char y, unsigned char num, _Bool invert) {
	char	*image;
    char    rotImage[8];
	
    image = get68image(num - 0x20);

    for(int i = 0; i < 8; i++) {
        rotImage[i] = (shiftBit(image[0], 7-i) & 0x80) |
                    (shiftBit(image[1], 6-i) & 0x40) |
                    (shiftBit(image[2], 5-i) & 0x20) |
                    (shiftBit(image[3], 4-i) & 0x10) |
                    (shiftBit(image[4], 3-i) & 0x08) |
                    (shiftBit(image[5], 2-i) & 0x04) |
                    (shiftBit(image[6], 1-i) & 0x02) |
                    (shiftBit(image[7], 0-i) & 0x01);
        if(invert) rotImage[i] ^= 0xfc;
    }
    for(int i = 0; i < 8; i++) {
        uint32_t tmp = SSD1306_Buffer[(x/4)*3 + ((i+y)*16)] << 16 | SSD1306_Buffer[(x/4)*3 + 1 + ((i+y)*16)] << 8 | SSD1306_Buffer[(x/4)*3 + 2 + ((i+y)*16)];
        if(x % 4 == 3) {
            tmp &= 0xffffffc0;
            tmp |= rotImage[i] >> 2;
        }
        else {
            tmp &= 0xffffffff - (0xff << (16 - ((x % 4) * 6)));
            tmp |= rotImage[i] << (16 - ((x % 4) * 6));
        }
        SSD1306_Buffer[(x/4)*3 + ((i+y)*16)] = tmp >> 16;
        SSD1306_Buffer[(x/4)*3 + 1 + ((i+y)*16)] = tmp >> 8;
        SSD1306_Buffer[(x/4)*3 + 2 + ((i+y)*16)] = tmp;
    }//*/
    /*for(int i = 0; i < 8; i++) {
        displayBuffer[x + ((i+y)*16)] = rotImage[i];
    }//*/
}

void outtextxy68_fonts(char x, char y, char *snumbuf, _Bool invert) {
    while(*snumbuf) {
        outtext68_font(x++, y, *snumbuf, invert);
	    snumbuf++;
    }
}//*/

/* [] END OF FILE */

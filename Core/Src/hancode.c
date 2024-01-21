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

#include "hancode.h"
#include "hanTable.h"

char HBUF[32];
char EBUF[16];

// 2byte(16bit)의 한글 처리
// 1bit(한1영0) + 5bit(초성) + 5bit(중성) + 5bit(종성)
char *gethanimage(unsigned char hanx1, unsigned char hanx2) {
	unsigned char first,mid,last,b1,b2,b3,i;
	unsigned short c;

	c = (unsigned short)(((unsigned short)hanx1 <<(unsigned short)8) | (unsigned short)hanx2);  // 8bit 2byte를 16bit 1byte로 변환
	
   	for(i=0; i<32; i++) HBUF[i] = 0x00;
        
    first = (c>>10)& 0x1f;		    // 초성을 분리해냄.  
    mid   = (c>>5) & 0x1f;          // 중성을 분리해냄.  
    last  =  c     & 0x1f;		    // 종성 코드값.
    					            // 꼴의 위치를 결정.  
    first =  table[0][first];
    mid   =  table[1][mid]; 		// 중성 글꼴의 위치를 결정.  
    last  =  table[2][last]; 		// 종성 글꼴의 위치를 결정.   
    
    b3 = midtable[0][mid]; 		    // 중성으로 종성의 벌수 결정.  
    if (!last) { 					// 종성이 없는 경우.  
        b2 = fsttable[0][first];  	// 초성으로 중성 결정.  
        b1 = midtable[1][mid];  	// 중성으로 초성 결정.  
    } 
    else {  					    // 종성이 있는 경우.  
        b2 = fsttable[1][first];  	// 초성으로 중성 결정.  
        b1 = midtable[2][mid];  	// 중성으로 초성 결정.  
    }

    if (first) {  
        for(i=0; i<32; i++)
            HBUF[i] |= HAN1[b1][first][i];  
    }		
      
    if (mid) {  
        for(i=0; i<32; i++)
            HBUF[i] |= HAN2[b2][mid][i];  
    }		  
    if (last) {  
        for(i=0; i<32; i++)
            HBUF[i] |= HAN3[b3][last][i];  
    }

	/*if ( glcdattr & LCD_UNDERLINE ) {
		for ( i =  8; i < 16; i++) {
			HBUF[i] |= 0x40;
			HBUF[(unsigned char)(i+16)] |= 0x40;
		}
	}

	if ( glcdattr & LCD_REVERSE ) {
		for(i=0; i<32; i++) HBUF[i] ^= 0xff;
	}*/
              			     
	return &HBUF[0];          			        
}
 

char *getascimage(unsigned char c) {       
	unsigned char i;
	for(i=0; i<16; i++)
 	EBUF[i] &= 0x00;
	
	for(i=0; i<16; i++) EBUF[i] = eng_font2[c][i];  

	/*if ( glcdattr & LCD_UNDERLINE ) {
		for ( i =  8; i < 16; i++) EBUF[i] |= 0x40;
	}
	if ( glcdattr & LCD_REVERSE ) {
		for(i=0; i<16; i++) EBUF[i] ^= 0xff;
	}*/
	
	return &EBUF[0];
}

char *get68image(unsigned char c) {       
	unsigned char i;
	for(i=0; i<16; i++) EBUF[i] = 0x00;
	
	//if ( c <= 10 )for(i=0; i<6; i++) EBUF[i] = db_num[c][i];  // 0123456789/
    for(i=0; i<6; i++) EBUF[i] = font6x8[c][i];

	/*if ( glcdattr & LCD_UNDERLINE ) {
		for ( i =  0; i < 6; i++) EBUF[i] |= 0x80;
	}
	if ( glcdattr & LCD_REVERSE ) {
		for(i=0; i<6; i++) EBUF[i] ^= 0xff;
	}*/
	
	return &EBUF[0];
}


char *geticonimage(unsigned char u1) {       

	unsigned char i;
	
	for(i=0; i<32; i++) HBUF[i] = db_icon[u1][i];

 	return &HBUF[0];
}       

// 완성형을 조합영으로 변환
unsigned short hconv(unsigned short hhcode) {   /* ret=1 : 변환안된경우,  ret=0 : 한글변환된경우 */
                                            /* hg한글point의 완성형을 조합형으로 1글자만으로 변환함 */
	unsigned short loc;
	unsigned char hg;
	unsigned char hg1;
	 
	hg = (hhcode & 0xff00) >> 8;
	hg1 = hhcode & 0x00ff;
	 
	if(hg<0x80) return(1);
	if(hg<0xA1 || hg1<0xA1) return(1);
	if(hg>0xA0 && hg<0xAD) {
		switch(hg) {
			case 0xA1 :
				if(hg1>0xA1 && hg1<0xEF) { hg=0xD9; hg1=0x32+((unsigned short)(hg1-0xA2)); break; }
				if(hg1>0xEE && hg1<0xFF) { hg=0xD9; hg1=0x91+((unsigned short)(hg1-0xEF)); break; }
			case 0xA2 :
				if(hg1>0xA0 && hg1<0xE6) { hg=0xD9; break; }
			case 0xA3 :
				if(hg1>0xA0 && hg1<0xEF) { hg=0xDA; hg1=0x31+hg1-0xA1; break; }
				if(hg1>0xEE && hg1<0xFF) { hg=0xDA; hg1=0x91+hg1-0xEF; break; }
			case 0xA5 :
			 	if(hg1>0xA0 && hg1<0xAB) { hg=0xDB; hg1=0x31+hg1-0xA1; break; }
				if(hg1>0xAF && hg1<0xBA) { hg=0xDB; hg1=0x40+hg1-0xB0; break; }
			case 0xA6 :
				if(hg1>0xA0 && hg1<0xE5) { hg=0xDB; break; }
			case 0xA8 :
			  	if(hg1>0xB0 && hg1<0xFF) { hg=0xDC; break; }
			case 0xA9 :
				if(hg1>0xB0 && hg1<0xEF) { hg=0xDD; hg1=0x41+hg1-0xB1; break; }
				if(hg1>0xF0 && hg1<0xFF) { hg=0xDD; hg1=0x91+hg1-0xEF; break; }
			 	hg=' '; hg1=' '; break;
			default : hg=' '; hg1=' ';
		}
	}
	loc = (unsigned short)(hg-176)*94;
	loc+= (unsigned short)(hg1-161);
	if(loc>=2350) {
        hg=' ';
        hg1=' ';
    }
	else if(loc>=0) {
        //hg = I_H_BUFFER[loc*2];
        //hg1=I_H_BUFFER[loc*2+1];
        hg = KS_Table[loc][0];
        hg1 = KS_Table[loc][1];
    }
	 
	return (unsigned short)(((unsigned short)hg<<(unsigned short)8) | hg1);
}
/* [] END OF FILE */

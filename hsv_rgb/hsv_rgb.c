#include <stdio.h>

typedef struct {float H,S,V;}tsHSV;
tsHSV RGB_HSV(unsigned char R, unsigned char G, unsigned char B)
{
	#define MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))
	#define MIN(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))   
	
    float V = MAX(R, G, B);
    float S = (V==0)?0:(V-MIN(R,G,B));
	float H = (V == R)?(60*(G-B) / (V-MIN(R,G,B))):((V==G)?(120 + 60*(B-R) / (V - MIN(R,G,B))):(H = 240 + 60*(R-G) / (V - MIN(R,G,B))));
    
	tsHSV HSV;
	HSV.H = (H < 0)?(H+360):H;
	HSV.S = S/255;
	HSV.V = V/255;
	
	return HSV;
}

typedef struct{unsigned char R,G,B;}tsRGB;
tsRGB HSV_RGB(float H, float S, float V)
{
	tsRGB RGB;
	unsigned char hi = ((unsigned int)(H / 60)) % 6;
	float f = H / 60 - (float)hi;
	float p = V * (1 - S);
	float q = V * (1 - f * S);
	float t = V * (1 - (1 - f) * S);
	#define SET(a,b,c) do{RGB.R=(unsigned char)(a*255);RGB.G=(unsigned char)(b*255);RGB.B=(unsigned char)(c*255);}while(0)
	switch(hi){
		case 0:{SET(V,t,p);}break;
		case 1:{SET(q,V,p);}break;
		case 2:{SET(p,V,t);}break;
		case 3:{SET(p,q,V);}break;
		case 4:{SET(t,p,V);}break;
		case 5:{SET(V,p,q);}break;
	}
	return RGB;
}

int main()
{
    unsigned char R = 211, G = 102, B = 249;
	tsHSV HSV;
	HSV = RGB_HSV(R,G,B);
	printf("HSV=%f,%f,%f\n", HSV.H,HSV.S,HSV.V);
	
	tsRGB RGB = HSV_RGB(HSV.H,HSV.S,HSV.V);
	printf("RGB:%d,%d,%d\n",RGB.R,RGB.G,RGB.B);
	
    return 0;
}

#include <stdio.h>

#define TEST P2

const short wCRCTalbeAbs[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
    0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};
short CRC16(unsigned char* pchMsg, int wDataLen)
{
	short wCRC = 0xFFFF;
	int i;
	char chChar;
	for (i = 0; i < wDataLen; i++)
	{
			chChar = *pchMsg++;
			wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
			wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
	}
	return wCRC;
}

void vFormat(unsigned char *data)
{    
    short crc = CRC16(data, 3);    
    data[3] = (crc>>8)&0xff;    
    data[4] = crc&0xff;
}

int main()
{
	unsigned char test[5] = {0x55, 0x00, 0x00, 0x00, 0x00};
	
	unsigned char cmd = 0x00;
	printf("input cmd:\n");
	scanf("%x", &cmd);

	switch(cmd){
		case 0x03:{
			unsigned char value = 0x00;
			printf("Please input the value:");
			scanf("%x", &value);
			test[1] = cmd;
			test[2] = value;
		}break;
		case 0x01:{
			unsigned char value = 0x00;
			printf("Please input the value:");
			scanf("%x", &value);
			test[1] = cmd;
			test[2] = value;
		}break;
		case 0x05:{
			test[1] = cmd;
		}break;
		case 0x06:{
			test[1] = cmd;
		}break;
	default:	
		printf("error cmd\n");
		return 0;
	}

	vFormat(test);

	for(int i = 0;i < 5; i++){
		printf("%02x ", test[i]);
	}printf("\n");
	
	return 0;
}

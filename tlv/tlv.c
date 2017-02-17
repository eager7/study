#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <syslog.h>
#include <json-c/json.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <syslog.h>

#define TLV_NUM 10
#define TLV_TYPE_LEN 1
#define TLV_LEN_LEN 1
#define TLV_HEADER (TLV_TYPE_LEN + TLV_LEN_LEN)
#define TLV_FRAGMENTED 255
#define CHECK_POINTER(value,ret) do{ if(value==NULL){printf( "Pointer is NULL");return ret;}}while(0)
#define PrintArray(a, x, len) do{if(a){printf("----Array Len:%d\n", len);for (int j = 0; j < len; ++j) {printf("0x%02x,", x[j]);}printf("\n");}}while(0)

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef long long           int64;

typedef enum{
    E_TLV_STATUS_ERROR,
    E_TLV_STATUS_OK,
}teTlvStatus;

typedef struct {
    uint8   u8Type;
    uint16  u16Len;
    uint16  u16Offset;
    uint8   *psValue;
} tsTlv;

typedef uint8* (*tpfTlvGetRawData)(struct _tsTlvMessage *psTlvMessage);
typedef struct _tsTlvMessage{
    tsTlv sTlvRecord[TLV_NUM];
    uint8 u8RecordNum;
    tpfTlvGetRawData pfTlvGetRawData;
} tsTlvMessage;

teTlvStatus eTlvMessageFormat(uint8 *psBuffer, uint16 u16Len, tsTlvMessage *psTlvMsg)
{
    CHECK_POINTER(psBuffer, E_TLV_STATUS_ERROR);
    CHECK_POINTER(psTlvMsg, E_TLV_STATUS_ERROR);
    uint16 u16OffSet = 0;
    //while(u16Len > u16OffSet)
	{
        for(int i = 0; (i < TLV_NUM)&&(u16Len > u16OffSet); i++){
            psTlvMsg->sTlvRecord[i].u8Type = psBuffer[u16OffSet];
			printf("type:%d\n", psTlvMsg->sTlvRecord[i].u8Type);
            u16OffSet += TLV_TYPE_LEN;
            while(psBuffer[u16OffSet] == TLV_FRAGMENTED){
				psTlvMsg->sTlvRecord[i].u16Len += TLV_FRAGMENTED;
                uint8* psTemp = (uint8*)realloc(psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
                CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
                psTlvMsg->sTlvRecord[i].psValue = psTemp;
                u16OffSet += TLV_LEN_LEN;
				printf("memcpy:%d\n", u16OffSet);
                memcpy(&psTlvMsg->sTlvRecord[i].psValue[psTlvMsg->sTlvRecord[i].u16Offset], &psBuffer[u16OffSet], TLV_FRAGMENTED);
               	psTlvMsg->sTlvRecord[i].u16Offset += TLV_FRAGMENTED;
                u16OffSet += TLV_FRAGMENTED + TLV_TYPE_LEN;
            }
            psTlvMsg->sTlvRecord[i].u16Len += psBuffer[u16OffSet];
			printf("len:%d\n", psTlvMsg->sTlvRecord[i].u16Len);
            uint8* psTemp = (uint8*)realloc(psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
            CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
            psTlvMsg->sTlvRecord[i].psValue = psTemp;         
            memcpy(&psTlvMsg->sTlvRecord[i].psValue[psTlvMsg->sTlvRecord[i].u16Offset], &psBuffer[u16OffSet+1], psBuffer[u16OffSet]);
            u16OffSet += psBuffer[u16OffSet];
			u16OffSet += TLV_LEN_LEN;
            psTlvMsg->u8RecordNum ++;
        }
    }
}

teTlvStatus eTlvMsgGetBinaryData(tsTlvMessage *psTlvMsg, uint8 **psBuf, uint16 *pu16Len)
{
    uint8 *psBuffer = NULL;
    uint16 u16Len = 0;
    uint16 offset = 0;
    for (int i = 0; i < psTlvMsg->u8RecordNum; ++i) {
        u16Len += psTlvMsg->sTlvRecord[i].u16Len;
		printf("Len:%d\n", u16Len);
        uint8 *psTemp = (uint8*)realloc(psBuffer, u16Len);
        CHECK_POINTER(psTemp, E_TLV_STATUS_ERROR);
        psBuffer = psTemp;
		printf("%p\n", psBuffer);
        memcpy(&psBuffer[offset], psTlvMsg->sTlvRecord[i].psValue, psTlvMsg->sTlvRecord[i].u16Len);
        offset += psTlvMsg->sTlvRecord[i].u16Len;
    }
    *pu16Len = u16Len;
	*psBuf = psBuffer;
    return E_TLV_STATUS_OK;
}

int main()
{
	uint8 test1[] = {						0x06,0x01,0x01,0x03,0x20,0x7b,0xf5,0x00,0xd0,0xcd,0x36,0x06,0x1a,0x28,0x4b,0xa7,0xa1,0x99,0x47,0x49,0x2d,0xb1,0xee,0xfa,0x16,0x9c,0xe8,0x1c,0x59,0xa5,0xa4,0x54,0xa6,0xcd,0xb8,0x8d,0x2a
	};
	uint8 test2[] = {
0x06,0x01,0x03,
0x03,0xff,0xd2,0x19,0x0b,0x12,0x35,0x50,0x1c,0xe7,0x3b,0x17,0xf1,0xcb,0x72,0x11,0x43,0x78,0x7f,0xf4,0x8c,0x69,0xb7,0xcf,0x51,0xbd,0x67,0x9e,0xee,0x26,0x8e,0x6f,0xd9,0xd8,0xfd,0x00,0xdb,0x17,0x36,0x4a,0x4c,0x56,0xd2,0x50,0x1c,0xf7,0xc1,0xae,0x97,0x6b,0xec,0x26,0x47,0xbd,0x7c,0x5a,0x25,0x21,0x8f,0x6a,0xf0,0xf9,0x8c,0xa0,0x05,0x1c,0x31,0xb6,0xc5,0xc8,0xab,0xaf,0x51,0x85,0x7b,0x82,0x4b,0x8e,0x27,0x7d,0x2c,0xb4,0xc1,0x1f,0xb4,0x5c,0x19,0x5f,0xe7,0xbf,0x1c,0x2b,0x7c,0x22,0x69,0x4b,0xd6,0x98,0x04,0x47,0x95,0x48,0x92,0x62,0x0f,0x04,0x55,0x5e,0xa2,0xd1,0x2d,0xdc,0xce,0x90,0xb2,0x3e,0x37,0x06,0xd7,0xff,0xa3,0x1e,0x81,0x36,0xc0,0x7a,0x3a,0x0b,0x44,0x32,0xef,0x58,0x57,0x95,0x48,0x03,0x82,0xb4,0xa8,0x48,0x1b,0x07,0x81,0x2d,0x7e,0x4a,0x0e,0x9d,0xe4,0x3c,0x8e,0x53,0xed,0xf6,0x08,0x55,0xa8,0x33,0xa9,0x8d,0x27,0xca,0x8b,0x49,0xdc,0x35,0xcb,0x1b,0x84,0x06,0x5e,0xee,0x89,0xad,0xc7,0x98,0x90,0x0d,0xa1,0xac,0x85,0x42,0xd5,0xd4,0xff,0x3f,0xc4,0xb9,0x06,0x4a,0x7b,0x17,0x8e,0x60,0xd4,0x49,0x84,0x20,0x72,0x58,0xeb,0x4e,0xb4,0x9d,0x29,0x16,0x7f,0x21,0xe7,0xc8,0xe6,0xfe,0x19,0xae,0x6e,0x20,0x16,0x6c,0xea,0x1b,0x83,0x84,0xba,0x9b,0x54,0x81,0x37,0x9f,0x11,0xd3,0xd9,0xfb,0x15,0x22,0xf1,0xf1,0x23,0x5b,0x3c,0xcc,0x85,0x45,0xf7,0xa5,0x35,0x3f,0x35,0x49,0xca,0x97,0xe0,0x9d,0xe9,0x66,0xb2,0xae,0x11,0x03,0x81,0x48,0x29,0xb5,0x07,0x38,0x71,0xe8,0xe4,0x57,0xb7,0xf2,0x30,0x83,0x42,0x5b,0x1b,0xe4,0xc0,0xf2,0x50,0xc9,0x7b,0xe8,0x6e,0xa7,0x7e,0x42,0xf9,0xe3,0x70,0xde,0xdc,0x79,0xcb,0xcc,0x9c,0x7e,0xea,0x46,0x96,0x39,0x58,0xb4,0xf0,0xcf,0x95,0xc4,0xff,0x63,0x08,0x34,0xc6,0x4d,0x39,0x8e,0xd8,0xd9,0x48,0x0c,0x13,0xe7,0x22,0x51,0x98,0x90,0x35,0x81,0xbc,0xb8,0xe0,0xaf,0x1d,0xb4,0x89,0x2b,0x86,0x05,0xcd,0x33,0x4b,0xaa,0xe5,0x67,0x99,0x19,0xac,0xae,0x02,0x0e,0x6a,0xc9,0x31,0x19,0xe9,0x8a,0xe6,0x68,0xfa,0xca,0x72,0x7b,0xf0,0x2d,0xbc,0xe5,0x35,0x10,0xe1,0x0d,0x16,0x1f,0x0c,0x8c,0xc9,0xfe,0xf1,0xbd,0x48,0xe1,0x62,0x91,0x1f,0x11,0x44,0xac,0x91,0x58,0xbb,0x57,0x04,0x40,0xe4,0x0a,0xc5,0x52,0x10,0xb0,0xd7,0x0e,0x6e,0x0f,0x08,0x44,0xa4,0x85,0x8d,0xa3,0xec,0x17,0xb1,0xc0,0x4e,0x3c,0x85,0xba,0x1b,0x0e,0x38,0xe2,0xec,0xc9,0xd3,0x1a,0x83,0x5a,0xcd,0xf1,0x1a,0xc8,0xd9,0xfd,0xa6,0xaa,0xc6,0xba,0xf8,0xf6,0x09,0xe0,0x7b,0xd4,0xa3,0x87,0x2e,0xd6,0xe9,0xe5,0x7c,0x53,0x05,0xee,0x9f,0x3e,0x2e,0x67
	};	
	printf("Sizeof Test:%d\n", sizeof(test2));
	tsTlvMessage sTlvMsg;
	memset(&sTlvMsg, 0, sizeof(tsTlvMessage));
	eTlvMessageFormat(test2, sizeof(test2), &sTlvMsg);
	printf("Number Tlv:%d\n", sTlvMsg.u8RecordNum);	
	for(int i = 0; i < sTlvMsg.u8RecordNum; i++){
		PrintArray(1, sTlvMsg.sTlvRecord[i].psValue, sTlvMsg.sTlvRecord[i].u16Len);
	}
	

	printf("");
	uint8 *rep = NULL;
	uint16 Len = 0;
	eTlvMsgGetBinaryData(&sTlvMsg, &rep, &Len);
	printf("%p\n", rep);
	PrintArray(1,rep,Len);
	return 0;
}
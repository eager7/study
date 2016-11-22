#include <crc16.h>

int main()
{
    unsigned char data = {};

    printf("crc:0x%02x\n", CRC16(data, sizeof(data)));
    return 0;
}

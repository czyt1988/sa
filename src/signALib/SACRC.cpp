#include "SACRC.h"
#define CRC_POLY_16 0xA001
#define CRC_POLY_32 0xEDB88320L
#define CRC_POLY_CCITT 0x1021
#define CRC_POLY_DNP 0xA6BC
#define CRC_POLY_KERMIT 0x8408
#define CRC_POLY_STCK 0x8005

#define CRC_START_8 0x00
#define CRC_START_16 0x0000
#define CRC_START_MODBUS 0xFFFF
#define CRC_START_XMODEM 0x0000
#define CRC_START_CCITT_1DOF 0x1D0F

static unsigned short crc_tab16[256];
static bool crc_tab16_init = false;
SACRC::SACRC()
{

}

void SACRC::initCRC16Table()
{
    unsigned short i;
    unsigned short j;
    unsigned short crc;
    unsigned short c;
    for(i=0;i<256;++i)
    {
        crc = 0;
        c = i;
        for(j=0;j<8;++j)
        {
            if((crc ^ c) & 0x0001)
            {
                crc = (crc >> 1) ^ CRC_POLY_16;
            }
            else
            {
                crc = crc >> 1;
            }
            c = c >> 1;
        }
        crc_tab16[i] = crc;
    }
    crc_tab16_init = true;
}

unsigned short SACRC::crc16(const unsigned char *input_str, unsigned int num_bytes)
{
    unsigned short crc,tmp,short_c;
    const unsigned char * ptr;
    unsigned int a;
    if(!crc_tab16_init)
        initCRC16Table();

    crc = CRC_START_16;
    ptr = input_str;
    if(ptr != 0)
    {
        for(a = 0;a<num_bytes;++a)
        {
            short_c = 0x00ff & (unsigned short)*ptr;
            tmp = crc ^ short_c;
            crc = (crc >> 8) ^ crc_tab16[tmp & 0xff];
            ++ptr;
        }
    }
    return crc;
}

unsigned short SACRC::crc16(const QByteArray &data)
{
    return SACRC::crc16((const unsigned char *)data.data(),data.size());
}


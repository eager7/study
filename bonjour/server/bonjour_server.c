#include <stdio.h>
#include <stdlib.h>
#include "dns_sd.h"

static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags, DNSServiceErrorType errorCode,const char *name, const char *regtype, const char *domain, void *context) ; 

static const char TEXT[] = 
    "\x05" "v=1.0"  \
    "\x14""id=03:d2:24:a1:bd:75"    \
    "\04""c#=1" \
    "\04""s#=1" \
    "\04""sf=0" \
    "\04""ff=0" \
    "\07""md=test"  \
    "\04""ci=5";

typedef union { unsigned char b[2]; unsigned short NotAnInteger; } Opaque16; 

int main()
{
    printf("bonjour server\n");
#if 0
    //Opaque16 registerPort = { { 0x12, 0x34 } };
    static const char TXT[] = "\xC" "First String" "\xD" "Second String" "\xC" "Third String";

    DNSServiceRef                       psdRef;
    DNSServiceFlags                     flags = 1;
    uint32_t                            interfaceIndex = 0; 
    const char                          *name = "test";         /* may be NULL */
    const char                          *regtype = "_hap._tcp";
    const char                          *domain = NULL;       /* may be NULL */
    const char                          *host = NULL;       /* may be NULL */
    uint16_t                            port = 5353;
    uint16_t                            txtLen = sizeof(TEXT);
    const void                          *txtRecord = TEXT;    /* may be NULL */
    DNSServiceRegisterReply             callBack = NULL;      /* may be NULL */
    void                                *context= NULL;       /* may be NULL */
    DNSServiceErrorType ret = DNSServiceRegister(
        &psdRef,
        0,
        interfaceIndex,
        "test",
        "_hap._tcp.",
        "",
        NULL,
        0, //registerPort.NotAnInteger,
        0,
        "",
        NULL,
        NULL
    );
    printf("ret = %d\n", ret);

    ret = DNSServiceUpdateRecord(psdRef, NULL, 0, sizeof(TEXT)-1, TEXT, 0);

    printf("ret = %d\n", ret);
#else
    int pid = 1515;
    static uint32_t interface = kDNSServiceInterfaceIndexAny;
    static DNSServiceRef client  = NULL;
    static DNSRecordRef record = NULL;

    Opaque16 registerPort = { { pid >> 8, pid & 0xFF } };
    static const char TXT1[] = "\xC" "First String"  "\xD" "Second String" "\xC" "Third String";
    static const char TXT2[] = "\xD" "Fourth String" "\xC" "Fifth String"  "\xC" "Sixth String";
    printf("Registering Service Test._testdualtxt._tcp.local.\n");
    int err = DNSServiceRegister(&client, 0, interface, "Test", "_testdualtxt._tcp.", "", NULL, registerPort.NotAnInteger, sizeof(TXT1)-1, TXT1, NULL, NULL);
    if (!err) err = DNSServiceAddRecord(client, &record, 0, kDNSServiceType_TXT, sizeof(TXT2)-1, TXT2, 0);
    else printf("error:%d\n", err);
#endif
  
    while(1);

    return 0;
}

static void DNSSD_API reg_reply(DNSServiceRef client, DNSServiceFlags flags, DNSServiceErrorType errorCode,
                                    const char *name, const char *regtype, const char *domain, void *context)
    {
            (void)client;   // Unused
            (void)flags;    // Unused
            (void)context;  // Unused

            printf("Got a reply for %s.%s%s: ", name, regtype, domain);
            switch (errorCode)
                {
                            case kDNSServiceErr_NoError:      printf("Name now registered and active\n"); break;
                            case kDNSServiceErr_NameConflict: printf("Name in use, please choose another\n"); exit(-1);
                            default:                          printf("Error %d\n", errorCode); return;
                            }

            }


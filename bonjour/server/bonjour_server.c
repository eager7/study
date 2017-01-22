#include <stdio.h>
#include <stdlib.h>
#include "dns_sd.h"

static const char TEXT[] = 
    "\x05" "v=1.0"  \
    "\x14""id=03:d2:24:a1:bd:75"    \
    "\04""c#=1" \
    "\04""s#=1" \
    "\04""sf=0" \
    "\04""ff=0" \
    "\07""md=test"  \
    "\04""ci=5";

int main()
{
    printf("bonjour server\n");

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
    while(1);

    return 0;
}

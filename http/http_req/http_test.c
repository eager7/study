#include <stdio.h>
#include "ghttp.h"


#define UI_RED(x)       "\e[31;1m"x"\e[0m"
#define UI_GREEN(x)     "\e[32;1m"x"\e[0m"
#define UI_YELLOW(x)    "\e[33;1m"x"\e[0m"
#define UI_BLUE(x)      "\e[34;1m"x"\e[0m"
#define UI_PURPLE(x)    "\e[35;1m"x"\e[0m"

#define DBG_vPrintln(a,b,ARGS...)  \
    do {if (a) {printf(UI_BLUE   ("[%d]" b "\n"), __LINE__, ## ARGS);}} while(0)

#define ERR_vPrintln(a,b,ARGS...)  \
        do {if (a) {printf(UI_RED   ("[%d]" b "\n"), __LINE__, ## ARGS);}} while(0)

#define CHECK_RESULT(fun,value,ret) do{ if(value!=fun){ERR_vPrintln(1, "Error");return ret;}}while(0)


int main()
{
    printf("libhttpg test\n");
    char *uri = "panchangtao";
    ghttp_request *request = NULL;
    ghttp_status status;
    char *buf;
    int bytes_read;

    request = ghttp_request_new();
    CHECK_RESULT(ghttp_set_uri(request, uri), 0, -1);
    CHECK_RESULT( ghttp_set_type(request, ghttp_type_get), 0, -1);
   
    ghttp_prepare(request);
    ghttp_process(request);
    buf = ghttp_get_body(request);
    bytes_read = ghttp_get_body_len(request);
    printf("len:%d, buf:%s\n", bytes_read, buf);

    ghttp_request_destroy(request);

    return 0;
}

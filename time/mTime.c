#include<stdio.h>
#include<time.h>

void main()
{
    struct tm t = {0};
    t.tm_year = 70;
    t.tm_mon = 0;
    t.tm_mday = 1;
    t.tm_hour = 8;
    t.tm_min = 0;
    t.tm_sec = 0;

    printf("start:%d\n", mktime(&t));
    printf("t------%d-%d-%d-%d:%d:%d\n", 1900+t.tm_year, t.tm_mon+1, t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

    time_t timep;
    struct tm *p = NULL;
    time(&timep);
    printf("timep:%llu\n", timep);
    p = localtime(&timep);
    timep = mktime(p);
    printf("timep:%llu\n", timep);
    printf("p------%d-%d-%d-%d:%d:%d\n", 1900+p->tm_year, p->tm_mon+1, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

    printf("now:%lu\n", time(NULL));
}

#include<stdio.h>
#include<time.h>

static inline unsigned long mktime_m (unsigned int year, unsigned int mon,
    unsigned int day, unsigned int hour,
    unsigned int min, unsigned int sec)
{
    if (0 >= (int) (mon -= 2)){    /**//* 1..12 -> 11,12,1..10 */
         mon += 12;      /**//* Puts Feb last since it has leap day */
         year -= 1;
    }
 
    return (((
             (unsigned long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
             year*365 - 719499
          )*24 + hour /**//* now have hours */
       )*60 + min /**//* now have minutes */
    )*60 + sec; /**//* finally seconds */
}

void main()
{
    struct tm t = {0};
    t.tm_year = 1970;
    t.tm_mon = 1;
    t.tm_mday = 1;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;

    printf("start:%lu\n", (long unsigned int)mktime(&t));
    printf("start:%lu\n", (long unsigned int)mktime_m(t.tm_year, t.tm_mon, t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec));
    printf("t------%d-%d-%d-%d:%d:%d\n", t.tm_year, t.tm_mon, t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

    time_t timep;
    struct tm *p = NULL;
    time(&timep);
    printf("timep:%lu\n", timep);
    p = localtime(&timep);
    timep = mktime(p);
    printf("timep:%lu\n", timep);
    printf("p------%d-%d-%d-%d:%d:%d\n", 1900+p->tm_year, p->tm_mon+1, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

    printf("now:%lu\n", time(NULL));
}


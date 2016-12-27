#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

void main()
{
    openlog("mylog", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_DEBUG|LOG_USER,      "my syslog debug test\n");
    syslog(LOG_INFO|LOG_USER,       "my syslog info test\n");
    syslog(LOG_NOTICE|LOG_USER,     "my syslog notice test\n");
    syslog(LOG_WARNING|LOG_USER,    "my syslog warning test\n");
    syslog(LOG_ERR|LOG_USER,        "my syslog err test\n");
    syslog(LOG_CRIT|LOG_USER,       "my syslog crit test\n");
    syslog(LOG_ALERT|LOG_USER,      "my syslog alert test\n");
    syslog(LOG_EMERG|LOG_USER,      "my syslog emerg test\n");
}

#ifndef SRVCONF_H
#define SRVCONF

typedef unsigned int uint;

typedef struct 
{
    int maxListen;
    int sock;
    char hostname[128];
    int timeout;
    int maxChildren;
    char logFile[128];
    char configFile[128];
    int uid;
    int gid;
    int daemon;
} srvConf;

extern void loadConf(srvConf *conf, const char *filename);

#endif


#include "common.h"
#include "srvConf.h"

static int conf_readline(int fd, char *buff, int len)
{
    int n = -1;
    int i = 0;
    int begin = 0;

    /*清缓冲区*/
    memset(buff, 0, len);
    for(i =0; i<len;begin?i++:i)/*当开头部分不为'\r'或者'\n'时i计数*/
    {
        n = read(fd, buff+i, 1);/*读一个字符*/
        if(n == 0)/*文件末尾*/{
            *(buff+i) = '\0';
            break;
        }else if(*(buff+i) == '\r' ||*(buff+i) == '\n'){/*回车换行*/
            if(begin){/*为一行*/
                *(buff+i) = '\0';
                    break;
            }
        }else{
            begin = 1;
        }
    }

    return i;
}

void config_read_file(srvConf *conf, const char* filename)
{
#define LINELENGTH 256
    assert(conf);
    
    char line[LINELENGTH] = {0};
    char *name = NULL, *value = NULL;
    int n;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return;
    }

    char *ep;
    char *sp;
    char *cp;
    while ((n = conf_readline(fd, line, LINELENGTH)) != 0) {
        if(cp = strchr(line, '#'))
            *cp = '\0';
        
        if(!(ep = strchr(line, '='))) continue;
        
        sp    = line;
        name  = sp;

        value = ep + 1;
        while(sp != ep) {
            sp++;
        }
        
        *sp = '\0';

        int ivalue;
        if (!strncmp("hostname", name, 8)){
            memcpy(conf->hostname, value, strlen(value)+1);
        } else if (!strncmp("timeout", name, 7)) {
            ivalue = strtol(value, NULL, 10);
            conf->timeout = ivalue;
        } else if (!strncmp("maxchildren", name, 11)) {
            ivalue = strtol(value, NULL, 10);
            conf->maxChildren = ivalue;
        } else if (!strncmp("logfile", name, 7)) {
            memcpy(conf->logFile, value, strlen(value) + 1);
        } else if (!strncmp("uid",name,3)) {
            ivalue = strtol(value, NULL, 10);
            conf->uid = ivalue;
        } else if (!strncmp("gid",name,3)) {
            ivalue = strtol(value, NULL, 10);
            conf->gid = ivalue;
        } else if (!strncmp("daemon", name, 5)) {
            ivalue = strtol(value, NULL, 10);
            conf->daemon = ivalue ? 1 : 0;
        }
    }

    close(fd);
}

srvConf* config_new()
{
    srvConf* conf = calloc(1, sizeof(*conf));
    assert(conf);
    
    return conf;
}

void config_free(srvConf** conf)
{
    assert(conf && *conf);
    assert(*conf != 0x0);
    free(*conf);
    *conf = NULL;
}

int main(void)
{
    srvConf* conf = config_new();
    config_read_file(conf, "./httpd.conf");
    
    printf("hostname = %s\n", conf->hostname);
    printf("hostname = %d\n", conf->timeout);
    
    config_free(&conf);

    return 0;
}

#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/select.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdbool.h>
#include<signal.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<errno.h>
#include<sys/epoll.h>
#include<fcntl.h>

#define argsCheck(argc,n){ \
    if(argc!=n){ \
        fprintf(stderr,"args error:expected %d arguments\n",n); \
        exit(1); \
    } \
} 

#define errorCheck(retVal, val, msg){ \
    if(retVal==val){ \
        perror(msg); \
        exit(1); \
    } \
} 

#define threadErrCheck(retVal,msg){ \
    if(ret!=0){ \
        fprintf(stderr,"%s:%s\n",msg,strerror(retVal)); \
    }\
}

#include "ae.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Anti-warning macro... */
#define UNUSED(V) ((void) V)

int updateFlags(int fd, int flag) {
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1)
    {
        printf("[updateFlags] fcntl F_GETFL failed(%s)!\n", strerror(errno));
        return 0;
    }

    flags |= flag;
    if(fcntl(fd, F_SETFL, flags) == -1)
    {
        printf("[updateFlags] fcntl F_SETFL failed(%s)!\n", strerror(errno));
        return 0;
    }

    return 1;
}

int initTCPListenFd(short port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("[initTCPListenFd] socket failed(%s)!\n", strerror(errno));
        return 0;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("[initTCPListenFd] bind failed(%s)!\n", strerror(errno));
        return 0;
    }
    updateFlags(fd, O_NONBLOCK|FD_CLOEXEC);

    if(listen(fd, 10) == -1){
		printf("listen socket error: %ss\n", strerror(errno));
		return 0;
	}
    return fd;
}

void connSocketEventHandler(struct aeEventLoop *el, int fd, void *clientData, int mask)
{
    char buf[4096] = {0};
    int  buf_len = sizeof(buf);

    int ret = read(fd, buf, buf_len-1);
    if (!ret) {
        printf("connection(port: %d) closed.\n", (int)clientData);
        close(fd);
        aeDeleteFileEvent(el, fd, AE_ALL_EVENTS);
        return;
    } else if (ret < 0 && errno != EAGAIN) {
        printf("connection error: %s.\n", strerror(errno));
    }

    printf("[connSocketEventHandler] read(port: %d): %s\n", (int)clientData, buf);
}

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    UNUSED(el);
    UNUSED(mask);
    UNUSED(privdata);

    printf("acceptTcpHandler start...\n");
    int cport, cfd, max = 1000;
    char cip[46];
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);

    while(max--) {
        cfd = accept(fd, (struct sockaddr*)&remote_addr, &remote_addr_len);
        if (cfd == -1) {
            if (errno != EWOULDBLOCK) printf("Accepting client connection error: %s\n", strerror(errno));
            return;
        }
        inet_ntop(AF_INET,(void*)&(remote_addr.sin_addr), &cip, sizeof(cip));
        cport = ntohs(remote_addr.sin_port);
        printf("accept remote connection(%s:%d)...\n", cip, cport);
        updateFlags(cfd, FD_CLOEXEC);
        if (aeCreateFileEvent(el,cfd,AE_READABLE,connSocketEventHandler, (void*)cport) == AE_ERR) {
            printf("aeCreateFileEvent failed");
        }
    }
}

int main() {
    aeEventLoop* eventloop =  aeCreateEventLoop(1024);
    if (eventloop == NULL) {
        printf("aeCreateEventLoop failed.");
        return 0;
    }

    int fd = initTCPListenFd(8080);
    if (aeCreateFileEvent(eventloop, fd, AE_READABLE,acceptTcpHandler,NULL) == AE_ERR) {
        printf("Unrecoverable error creating fd file event.");
    } 

    aeMain(eventloop);
    aeDeleteEventLoop(eventloop);
    
    return 0;
}
#include "ae.h"
#include <stdio.h>
#include <stdlib.h>

/* Anti-warning macro... */
#define UNUSED(V) ((void) V)

void cleanClientData(struct aeEventLoop *eventLoop, void *clientData) {
    if (clientData != NULL) {
        long long* count = (long long*)clientData;
        printf("clean client data: %lld\r\n", *count);
        free(clientData);
    }
}
int serverCron(struct aeEventLoop *eventLoop, long long id, void *clientData) {
    UNUSED(eventLoop);
    long long* count = (long long*)clientData;
    ++(*count);

    if (*count > 10) {
        return AE_NOMORE; // 返回 -1 不再执行
    }

    printf("serverCron id: %lld  count: %lld...\n", id, *count);

    // 返回值是下次执行的时间间隔, 单位: ms
    return 1000;
}

int main() {
    aeEventLoop* eventloop =  aeCreateEventLoop(1024);
    if (eventloop == NULL) {
        printf("aeCreateEventLoop failed.");
        return 0;
    }
    long long* count = (long long*)malloc(sizeof(long long));
    *count = 0;

    // milliseconds: 第一次执行的时间
    if (aeCreateTimeEvent(eventloop, 1000, serverCron, count, cleanClientData) == AE_ERR) {
        printf("Can't create event loop timers.");
        return 0;
    }

    aeMain(eventloop);
    aeDeleteEventLoop(eventloop);
    
    return 0;
}
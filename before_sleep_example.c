#include "ae.h"
#include <stdio.h>

/* Anti-warning macro... */
#define UNUSED(V) ((void) V)

int cnt = 0;

void beforeSleepProc(struct aeEventLoop *eventLoop) {
    UNUSED(eventLoop);
    printf("\n[beforeSleepProc](%d)\n", ++cnt);
}

void afterSleepProc(struct aeEventLoop *eventLoop) {
    UNUSED(eventLoop);
    printf("[afterSleepProc](%d)\n", cnt);
}

int timeProc(struct aeEventLoop *eventLoop, long long id, void *clientData) {
    UNUSED(eventLoop);
    UNUSED(clientData);

    printf("timeProc id: %lld(%d)...\r\n", id, cnt);
    return 1000;
}

int main() {
    aeEventLoop* eventloop =  aeCreateEventLoop(1024);
    if (eventloop == NULL) {
        printf("aeCreateEventLoop failed.");
        return 0;
    }
    aeSetBeforeSleepProc(eventloop, beforeSleepProc);
    aeSetAfterSleepProc(eventloop, afterSleepProc);
    
    if (aeCreateTimeEvent(eventloop, 1000, timeProc, NULL, NULL) == AE_ERR) {
        printf("Can't create event loop timers.");
        return 0;
    }
    if (aeCreateTimeEvent(eventloop, 1000, timeProc, NULL, NULL) == AE_ERR) {
        printf("Can't create event loop timers.");
        return 0;
    }

    aeMain(eventloop);
    aeDeleteEventLoop(eventloop);
    
    return 0;
}
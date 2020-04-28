#ifndef DISPATCH_H
#define	DISPATCH_H

#include "util/inc/component/channel.h"
#include "util/inc/component/httpframe.h"

typedef struct UserMsg_t {
	ReactorCmd_t internal;
	Channel_t* channel;
	Sockaddr_t peer_addr;
	HttpFrame_t* httpframe;
	char rpc_status;
	int cmdid;
	int rpcid;
	size_t datalen;
	unsigned char data[1];
} UserMsg_t;

typedef int(*DispatchCallback_t)(UserMsg_t*);

int initDispatch(void);
int regStringDispatch(const char* str, DispatchCallback_t func);
int regNumberDispatch(int cmd, DispatchCallback_t func);
DispatchCallback_t getStringDispatch(const char* str);
DispatchCallback_t getNumberDispatch(int cmd);
void freeDispatchCallback(void);

#endif // !DISPATCH_H

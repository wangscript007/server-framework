#ifndef GLOBAL_H
#define	GLOBAL_H

#include "util/inc/all.h"
#include "channel_imp.h"
#include "dispatch.h"
#include "msg_struct.h"
#include "rpc_helper.h"
#include "session.h"
#include <stdlib.h>

extern volatile int g_Valid;
extern Thread_t* g_ReactorThreads;
extern Thread_t* g_ReactorAcceptThread;
extern Thread_t g_TaskThread;
extern Reactor_t* g_Reactors;
extern Reactor_t* g_ReactorAccept;
extern DataQueue_t g_DataQueue;
extern size_t g_ReactorCnt;
extern RBTimer_t g_Timer;
extern RBTimer_t g_TimerRpcTimeout;
extern DispatchCallback_t g_DefaultDispatchCallback;
extern Hashtable_t g_SessionTable;
extern RpcFiberCore_t* g_RpcFiberCore;
extern RpcAsyncCore_t* g_RpcAsyncCore;

#ifdef __cplusplus
extern "C" {
#endif

int initGlobalResource(void);
void freeGlobalResource(void);
__declspec_dll Reactor_t* selectReactor(size_t key);
__declspec_dll DataQueue_t* ptr_g_DataQueue(void);
__declspec_dll RBTimer_t* ptr_g_Timer(void);
__declspec_dll void set_g_DefaultDispatchCallback(DispatchCallback_t fn);
__declspec_dll Hashtable_t* ptr_g_SessionTable(void);
__declspec_dll RpcFiberCore_t* ptr_g_RpcFiberCore(void);
__declspec_dll RpcAsyncCore_t* ptr_g_RpcAsyncCore(void);

#ifdef __cplusplus
}
#endif

#endif // !GLOBAL_H
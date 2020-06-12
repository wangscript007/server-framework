#include "../BootServer/config.h"
#include "../BootServer/global.h"
#include "../ServiceCommCode/service_comm_cmd.h"
#include "../ServiceCommCode/service_comm_proc.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "ServiceCommCode.lib")
#endif

static int service_center_check_connection_timeout_callback(RBTimer_t* timer, RBTimerEvent_t* e) {	
	logInfo(ptr_g_Log(), __FUNCTION__);

	do {
		SendMsg_t msg;
		Channel_t* sc_channel;
		ClusterGroup_t* sc_grp;
		Cluster_t* sc_cluster;

		char* req_data;
		int req_datalen;

		sc_grp = getClusterGroup(ptr_g_ClusterTable(), "ServiceCenter");
		if (!sc_grp)
			break;
		sc_cluster = pod_container_of(sc_grp->clusterlist.head, Cluster_t, m_grp_listnode);
		sc_channel = clusterChannel(sc_cluster);
		if (!sc_channel)
			break;
		req_data = strFormat(&req_datalen, "{\"ip\":\"%s\",\"port\":%u,\"weight_num\":%d,\"connection_num\":%d}",
			getClusterSelf()->ip, getClusterSelf()->port, getClusterSelf()->weight_num, getClusterSelf()->connection_num);
		if (!req_data)
			break;
		makeSendMsg(&msg, CMD_CLUSTER_HEARTBEAT, req_data, req_datalen);
		channelSendv(sc_channel, msg.iov, sizeof(msg.iov) / sizeof(msg.iov[0]), NETPACKET_FRAGMENT);
		free(req_data);
	} while (0);

	e->timestamp_msec = gmtimeMillisecond() + 1000 * 60;
	rbtimerAddEvent(timer, e);
	return 1;
}

#ifdef __cplusplus
extern "C" {
#endif

__declspec_dllexport int init(TaskThread_t* thrd, int argc, char** argv) {
	ConfigConnectOption_t* option = NULL;
	Cluster_t* cluster;
	RBTimerEvent_t* timeout_ev;
	unsigned int i;

	// listen port
	for (i = 0; i < ptr_g_Config()->listen_options_cnt; ++i) {
		ConfigListenOption_t* option = ptr_g_Config()->listen_options + i;
		ReactorObject_t* o;
		if (!strcmp(option->protocol, "http")) {
			o = openListenerHttp(option->ip, option->port, NULL);
		}
		else {
			continue;
		}
		if (!o) {
			logErr(ptr_g_Log(), "listen failure, ip:%s, port:%u ......", option->ip, option->port);
			return 0;
		}
		reactorCommitCmd(ptr_g_ReactorAccept(), &o->regcmd);
	}
	//

	for (i = 0; i < ptr_g_Config()->connect_options_cnt; ++i) {
		option = ptr_g_Config()->connect_options + i;
		if (!strcmp(option->protocol, "ServiceCenter"))
			break;
	}
	if (!option) {
		logErr(ptr_g_Log(), "miss connect service config");
		return 0;
	}
	cluster = newCluster(option->socktype, option->ip, option->port);
	if (!cluster) {
		logErr(ptr_g_Log(), "ServiceCenter newCluster error");
		return 0;
	}
	if (!regCluster(ptr_g_ClusterTable(), option->protocol, cluster)) {
		logErr(ptr_g_Log(), "ServiceCenter regCluster error");
		return 0;
	}
	if (!regNumberDispatch(CMD_DISTRIBUTE_CLUSTER_LIST, distributeClusterList)) {
		logErr(ptr_g_Log(), "regNumberDispatch(CMD_DISTRIBUTE_CLUSTER_LIST, distributeClusterList) failure");
		return 0;
	}
	if (!rpcReqClusterList(thrd, cluster)) {
		logErr(ptr_g_Log(), "rpcReqClusterList failure, ip:%s, port:%u ......", cluster->ip, cluster->port);
		return 0;
	}
	timeout_ev = (RBTimerEvent_t*)malloc(sizeof(RBTimerEvent_t));
	if (!timeout_ev) {
		logErr(ptr_g_Log(), "malloc(sizeof(RBTimerEvent_t)) error");
		return 0;
	}
	timeout_ev->arg = NULL;
	timeout_ev->timestamp_msec = gmtimeMillisecond() + 1000 * 60;
	timeout_ev->callback = service_center_check_connection_timeout_callback;
	rbtimerAddEvent(&thrd->timer, timeout_ev);
	return 1;
}

__declspec_dllexport void destroy(void) {

}

#ifdef __cplusplus
}
#endif
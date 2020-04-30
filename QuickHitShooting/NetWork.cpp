#include "NetWork.h"
#include <cassert>

bool NetWork::Connect(std::vector<int>& ip)
{
	IPDATA data;
	int DataLength = 0;
	char StrBuffer[512];
	data.d1 = ip[0];
	data.d2 = ip[1];
	data.d3 = ip[2];
	data.d4 = ip[3];
	NetHandle = ConnectNetWork(data, Port);
	if (NetHandle != -1) {
		NetWorkSend(NetHandle, "Œq‚ª‚ê‚âƒSƒ‹ƒ@II", 19);

		while (!ProcessMessage()) {
			DataLength = GetNetWorkDataLength(NetHandle);
			if (DataLength != 0) break;
		}
		NetWorkRecv(NetHandle, StrBuffer, DataLength);
	}
	else {
		assert(false && "Connection failed.");
	}
	return false;
}


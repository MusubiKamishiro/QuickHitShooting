#include "NetWork.h"
#include <thread>
#include <iostream>

NetWork::NetWork() {
}

NetWork::NetWork(const NetWork&) {
}

void NetWork::SetIP(int* ip)
{
	Ip.d1 = ip[0];
	Ip.d2 = ip[1];
	Ip.d3 = ip[2];
	Ip.d4 = ip[3];
}

void NetWork::Send(SendData* data)
{
	NetHandle = ConnectNetWork(Ip, Port);
	if (NetHandle == -1) return;
	NetWorkSend(NetHandle, data, sizeof(SendData));
	while (!ProcessMessage()) {
		DataLength = GetNetWorkDataLength(NetHandle);
		if (DataLength != 0)break;
	}
	NetWorkRecv(NetHandle, dataBuffer, DataLength);
	if (dataBuffer->result) {
		std::cout << dataBuffer->Buffer << std::endl;
		dataBuffer->result = false;
		if (dataBuffer->Buffer != "") {
			CloseNetWork(NetHandle);
		}
	}
}

SendData NetWork::Recive()
{
	PreparationListenNetWork(Port);
	NetHandle = -1;
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		NetHandle = GetNewAcceptNetWork();
		if (NetHandle != -1) break;
	}

	if (NetHandle != -1) {
		StopListenNetWork();
		GetNetWorkIP(NetHandle, &Ip);
		while (!ProcessMessage()) {
			if (GetNetWorkDataLength(NetHandle) != 0)break;
		}

		DataLength = GetNetWorkDataLength(NetHandle);
		NetWorkRecv(NetHandle, dataBuffer, DataLength);
		if (!dataBuffer->result) {
			std::cout << dataBuffer->Buffer << std::endl;
			dataBuffer->result = true;
			dataBuffer->Buffer += "Success";
			NetWorkSend(NetHandle, dataBuffer, sizeof(SendData));
			while (!ProcessMessage()) {
				LostNetHandle = GetLostNetWork();
				if (LostNetHandle == NetHandle) break;
			}
		}
		return *dataBuffer;
	}
	else {
		return SendData{};
	}
}

NetWork::~NetWork()
{
	std::free(dataBuffer);
}

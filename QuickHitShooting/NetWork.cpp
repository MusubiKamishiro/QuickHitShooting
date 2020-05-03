#include "NetWork.h"
#include <cassert>

void NetWork::Connect(std::vector<int>& ip)
{
	IPDATA data;
	int DataLength = 0;
	char StrBuffer[512];
	data.d1 = ip[0];
	data.d2 = ip[1];
	data.d3 = ip[2];
	data.d4 = ip[3];
	NetHandle = ConnectNetWork(data, Port);		// 接続
}

// 接続待ち状態にする関数
void NetWork::Listen()
{
	PreparationListenNetWork(NetHandle);
}

// 接続を閉じる関数
void NetWork::Close()
{
	CloseNetWork(NetHandle);
}

void NetWork::ReciveProcess()
{
	while (!ProcessMessage()) {
		// データが送られてくるまで待機
		if (GetNetWorkDataLength(NetHandle) != 0) {
			break;
		}
	}
	// データサイズ取得
	auto dataLength = GetNetWorkDataLength(NetHandle);
	// データ受信
	NetWorkRecv(NetHandle, RcvBuffer, dataLength);
	DrawString(100, 100, (TCHAR*)RcvBuffer, 0xffffff);
}


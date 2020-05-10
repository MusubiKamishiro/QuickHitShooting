#include "NetWork.h"
#include <iostream>
#include <thread>
#include <future>

namespace {
	SendData* dataBuffer = new SendData();		// データバッファ
	IPDATA Ip = {};		// IPアドレス
	int NetHandle;		// ネットワークハンドル
	int LostNetHandle = 0;		// 切断されたネットワークのハンドル
	int DataLength = 0;		// 受信データ量保存変数
	int Port = 2222;		// 接続ポート番号
}

NetWork::NetWork() {
}

NetWork::NetWork(const NetWork&) {
}

// IPアドレスをセットする
void NetWork::SetIP(int* ip)
{
	Ip.d1 = ip[0];
	Ip.d2 = ip[1];
	Ip.d3 = ip[2];
	Ip.d4 = ip[3];
}

void ReciveProcess() {
	// 接続待ち状態にする
	PreparationListenNetWork(Port);
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		// 接続があるまで待機
		NetHandle = GetNewAcceptNetWork();
		if (NetHandle != -1) break;
	}

	if (NetHandle != -1) {
		// 接続待ち状態を解除する
		StopListenNetWork();
		// 相手のIPアドレスを取得する
		GetNetWorkIP(NetHandle, &Ip);
		while (!ProcessMessage()) {
			// 相手からデータを送られてくるまで待機
			if (GetNetWorkDataLength(NetHandle) != 0)break;
		}

		// 送られてきたデータの大きさを取得
		DataLength = GetNetWorkDataLength(NetHandle);
		// 送られてきたデータを取得
		NetWorkRecv(NetHandle, dataBuffer, DataLength);
		if (!dataBuffer->result) {
			std::cout << dataBuffer->Buffer << std::endl;
			dataBuffer->result = true;
			dataBuffer->Buffer += "Success";
			// 相手に受信が成功したことを送り返す
			NetWorkSend(NetHandle, dataBuffer, sizeof(SendData));
			while (!ProcessMessage()) {
				// 相手から接続を切られたら終了
				LostNetHandle = GetLostNetWork();
				if (LostNetHandle == NetHandle) break;
			}
		}
	}
}

void NetWork::Send(SendData* data)
{
	// 指定したIPアドレス、ポート番号で接続
	NetHandle = ConnectNetWork(Ip, Port);
	if (NetHandle == -1) return;
	// 引数でもらったデータを相手に送る
	NetWorkSend(NetHandle, data, sizeof(SendData));
	// 相手から返信があるまで待機
	while (!ProcessMessage()) {
		// 受信データの大きさを取得
		DataLength = GetNetWorkDataLength(NetHandle);
		if (DataLength != 0)break;
	}
	// 送られてきたデータを受信
	NetWorkRecv(NetHandle, dataBuffer, DataLength);
	if (dataBuffer->result) {
		std::cout << dataBuffer->Buffer << std::endl;
		dataBuffer->result = false;
		if (dataBuffer->Buffer != "") {
			// 接続を切る
			CloseNetWork(NetHandle);
		}
	}
}

SendData NetWork::Recive()
{
	std::thread reciveThread(ReciveProcess);
	reciveThread.join();
	return *dataBuffer;
}

NetWork::~NetWork()
{
	// データバッファ―を解放する
	std::free(dataBuffer);
}

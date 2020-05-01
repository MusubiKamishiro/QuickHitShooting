#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include <thread>

class NetWork
{
public:
	static NetWork& Instance() {
		static NetWork instance;
		return instance;
	}
	~NetWork() {};

	void Connect(std::vector<int>& ip);		// 接続状態にする
	void Listen();		// 受信できるようにする
	void Close();		// 接続を閉じる関数
	template <class X>void Send(X data);		// データを送る
	template <class X>X Recive();		// データを受け取る

private:
	NetWork() {};
	NetWork(const NetWork&) {};
	void operator=(const NetWork&) {};
	void ReciveProcess();		// データ受信処理

	IPDATA* ConnectionIP = nullptr;	// 接続先のIPアドレス
	void* RcvBuffer = nullptr;		// 受信データ格納変数
	int NetHandle = -1;		// ネットワークハンドル
	int Port = 2222;		// ポート番号
};

// データを送るための関数
// 引数に送りたいデータを入れる
template<class X>
inline void NetWork::Send(X data)
{
	if (NetHandle == -1) return;
	std::thread send(NetWorkSend,this,NetHandle,data,sizeof(data));
	send.join();
}

// データを受け取る関数
// 戻り値に受信したデータへのアドレスが帰ってくる
template<class X>
inline X NetWork::Recive()
{
	if (NetHandle == -1) return nullptr;
	std::thread recive(ReciveProcess);
	recive.join();
	return RcvBuffer;
}

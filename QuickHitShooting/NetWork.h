#pragma once
#include <string>
#include <vector>
#include <DxLib.h>

class NetWork
{
public:
	static NetWork& Instance() {
		static NetWork instance;
		return instance;
	}
	~NetWork() {};

	void Connect(std::vector<int>& ip);
	void Listen();
	void Close();
private:
	NetWork() {};
	NetWork(const NetWork&) {};
	void operator=(const NetWork&) {};

	int NetHandle = 0;		// ネットワークハンドル
	int Port = 2222;		// ポート番号
};


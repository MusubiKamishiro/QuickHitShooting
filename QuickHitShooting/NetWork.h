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

	void Connect(std::vector<int>& ip);		// �ڑ���Ԃɂ���
	void Listen();		// ��M�ł���悤�ɂ���
	void Close();		// �ڑ������֐�
	template <class X>void Send(X data);		// �f�[�^�𑗂�
	template <class X>X Recive();		// �f�[�^���󂯎��

private:
	NetWork() {};
	NetWork(const NetWork&) {};
	void operator=(const NetWork&) {};
	void ReciveProcess();		// �f�[�^��M����

	IPDATA* ConnectionIP = nullptr;	// �ڑ����IP�A�h���X
	void* RcvBuffer = nullptr;		// ��M�f�[�^�i�[�ϐ�
	int NetHandle = -1;		// �l�b�g���[�N�n���h��
	int Port = 2222;		// �|�[�g�ԍ�
};

// �f�[�^�𑗂邽�߂̊֐�
// �����ɑ��肽���f�[�^������
template<class X>
inline void NetWork::Send(X data)
{
	if (NetHandle == -1) return;
	std::thread send(NetWorkSend,this,NetHandle,data,sizeof(data));
	send.join();
}

// �f�[�^���󂯎��֐�
// �߂�l�Ɏ�M�����f�[�^�ւ̃A�h���X���A���Ă���
template<class X>
inline X NetWork::Recive()
{
	if (NetHandle == -1) return nullptr;
	std::thread recive(ReciveProcess);
	recive.join();
	return RcvBuffer;
}

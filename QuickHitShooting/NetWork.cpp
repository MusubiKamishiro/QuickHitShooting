#include "NetWork.h"
#include <iostream>
#include <thread>
#include <future>

namespace {
	SendData* dataBuffer = new SendData();		// �f�[�^�o�b�t�@
	IPDATA Ip = {};		// IP�A�h���X
	int NetHandle;		// �l�b�g���[�N�n���h��
	int LostNetHandle = 0;		// �ؒf���ꂽ�l�b�g���[�N�̃n���h��
	int DataLength = 0;		// ��M�f�[�^�ʕۑ��ϐ�
	int Port = 2222;		// �ڑ��|�[�g�ԍ�
}

NetWork::NetWork() {
}

NetWork::NetWork(const NetWork&) {
}

// IP�A�h���X���Z�b�g����
void NetWork::SetIP(int* ip)
{
	Ip.d1 = ip[0];
	Ip.d2 = ip[1];
	Ip.d3 = ip[2];
	Ip.d4 = ip[3];
}

void ReciveProcess() {
	// �ڑ��҂���Ԃɂ���
	PreparationListenNetWork(Port);
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		// �ڑ�������܂őҋ@
		NetHandle = GetNewAcceptNetWork();
		if (NetHandle != -1) break;
	}

	if (NetHandle != -1) {
		// �ڑ��҂���Ԃ���������
		StopListenNetWork();
		// �����IP�A�h���X���擾����
		GetNetWorkIP(NetHandle, &Ip);
		while (!ProcessMessage()) {
			// ���肩��f�[�^�𑗂��Ă���܂őҋ@
			if (GetNetWorkDataLength(NetHandle) != 0)break;
		}

		// �����Ă����f�[�^�̑傫�����擾
		DataLength = GetNetWorkDataLength(NetHandle);
		// �����Ă����f�[�^���擾
		NetWorkRecv(NetHandle, dataBuffer, DataLength);
		if (!dataBuffer->result) {
			std::cout << dataBuffer->Buffer << std::endl;
			dataBuffer->result = true;
			dataBuffer->Buffer += "Success";
			// ����Ɏ�M�������������Ƃ𑗂�Ԃ�
			NetWorkSend(NetHandle, dataBuffer, sizeof(SendData));
			while (!ProcessMessage()) {
				// ���肩��ڑ���؂�ꂽ��I��
				LostNetHandle = GetLostNetWork();
				if (LostNetHandle == NetHandle) break;
			}
		}
	}
}

void NetWork::Send(SendData* data)
{
	// �w�肵��IP�A�h���X�A�|�[�g�ԍ��Őڑ�
	NetHandle = ConnectNetWork(Ip, Port);
	if (NetHandle == -1) return;
	// �����ł�������f�[�^�𑊎�ɑ���
	NetWorkSend(NetHandle, data, sizeof(SendData));
	// ���肩��ԐM������܂őҋ@
	while (!ProcessMessage()) {
		// ��M�f�[�^�̑傫�����擾
		DataLength = GetNetWorkDataLength(NetHandle);
		if (DataLength != 0)break;
	}
	// �����Ă����f�[�^����M
	NetWorkRecv(NetHandle, dataBuffer, DataLength);
	if (dataBuffer->result) {
		std::cout << dataBuffer->Buffer << std::endl;
		dataBuffer->result = false;
		if (dataBuffer->Buffer != "") {
			// �ڑ���؂�
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
	// �f�[�^�o�b�t�@�\���������
	std::free(dataBuffer);
}

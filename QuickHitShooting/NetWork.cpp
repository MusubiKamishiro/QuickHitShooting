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
	NetHandle = ConnectNetWork(data, Port);		// �ڑ�
}

// �ڑ��҂���Ԃɂ���֐�
void NetWork::Listen()
{
	PreparationListenNetWork(NetHandle);
}

// �ڑ������֐�
void NetWork::Close()
{
	CloseNetWork(NetHandle);
}

void NetWork::ReciveProcess()
{
	while (!ProcessMessage()) {
		// �f�[�^�������Ă���܂őҋ@
		if (GetNetWorkDataLength(NetHandle) != 0) {
			break;
		}
	}
	// �f�[�^�T�C�Y�擾
	auto dataLength = GetNetWorkDataLength(NetHandle);
	// �f�[�^��M
	NetWorkRecv(NetHandle, RcvBuffer, dataLength);
	DrawString(100, 100, (TCHAR*)RcvBuffer, 0xffffff);
}


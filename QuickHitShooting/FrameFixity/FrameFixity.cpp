#include "FrameFixity.h"
#include <functional>
#include <chrono>
#include <thread>


FrameFixity::FrameFixity()
{
}

FrameFixity::~FrameFixity()
{
}

void FrameFixity::FFInitialize()
{
	// �t���[�����[�g�����n�ϐ�������
	hWnd = {};
	msg = {};
	hdc = GetDC(hWnd);
	end = GetCTime();
	next = end + (1000 * 1000 / fps);
}

bool FrameFixity::CheckReceiveMessage()
{
	// ������ꂽ���b�Z�[�W��ϐ�msg�Ɋi�[
	// ���b�Z�[�W���������ꍇ��true���A���Ă���
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// �擾�������b�Z�[�W�𕶎��񃁃b�Z�[�W�ɕϊ�
		// �ϊ��������b�Z�[�W���E�B���h�E�v���V�[�W���ɓ�����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		return true;
	}
	else
	{
		// ���b�Z�\�W���󂯎���Ă��Ȃ��Ƃ��ɂ�false��Ԃ�
		return false;
	}
}

MSG FrameFixity::GetReceiveMessage()
{
	return msg;
}

void FrameFixity::AdjustmentFrameLate()
{
		// �t���[�����[�g�����n����
	std::this_thread::sleep_for(std::chrono::milliseconds(5));	// �w�肵�����Ύ��Ԃ����X���b�h���u���b�N����
	end = GetCTime();
	if (end < next)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(next - end));
		next += (1000 * 1000 / fps);
	}
	else
	{
		next = end + (1000 * 1000 / fps);
	}
}

void FrameFixity::Terminate()
{
	// �f�o�C�X�R���e�L�X�g���
	ReleaseDC(hWnd, hdc);
}

// �E�B���h�E���瓊�����Ă��郁�b�Z�[�W���L���b�`���ď�������
// �L�[�A�}�E�X�A�`��Ȃǂ̗l�X�ȃ��b�Z�[�W�������ɓ�������
// message�ɃL���b�`�������b�Z�[�W�������Ă���
// WM_DESTROY�̓E�B���h�E����ʂ�������Ƃ��ɌĂ΂��
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_DESTROY:
//		// ���C���E�B���h�E�����ꂽ����A�A�v���P�[�V�������I������Ƃ���
//		// ���̊֐����Ă�ŏI������������
//		PostQuitMessage(0);
//		break;
//	default:
//		// �V�X�e���ɏ�����C���鎞�͂��̊֐���Ԃ�
//		// �����ŏ����������Ƃ���0��Ԃ�
//		return DefWindowProc(hwnd, message, wParam, lParam);
//	}
//	return 0L;
//}

long long FrameFixity::GetCTime()
{
	// ���݂̎��Ԃ�SystemClock����擾���Amicroseconds�ɕϊ����Ă��̒l��Ԃ�
	std::function<long long(void)> currentTimeMicro = []()
	{
		std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
	};
	return currentTimeMicro();
}

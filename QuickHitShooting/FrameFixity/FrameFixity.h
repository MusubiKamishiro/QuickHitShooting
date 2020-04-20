#pragma once
#include <windows.h>

// �t���[�����[�g�Œ�p�N���X
// windows�v���O�����Ȃ̂�winmain���L�q���Ȃ��Ƃ����Ȃ�
class FrameFixity
{
private:
	HWND hWnd;		// �E�B���h�E�n���h���p�ϐ�
	MSG msg;		// ���b�Z�[�W���󂯎��ϐ�
	HDC hdc;		// �R���e�L�X�g�f�o�C�X�̃n���h��

	int fps = 60;		// �t���[�����[�g
	long long end = 0;	// �t���[�����[�g�����p�ϐ�
	long long next = 0;	// �t���[�����[�g�����p�ϐ�

	FrameFixity();
	FrameFixity(const FrameFixity&) {};
	void operator=(const FrameFixity&) {};

	long long GetCTime();
public :
	// �C���X�^���X���擾����֐�
	static FrameFixity& Instance() {
		static FrameFixity instance;
		return instance;
	}
	~FrameFixity();

	// �e�ϐ��Ȃǂ̏���������
	// �ŏ��Ɉ�񂾂��Ă�
	void FFInitialize();

	// ���b�Z�[�W���󂯎�������ǂ������`�F�b�N����
	// ���C�����[�v���Ŗ���Ă�
	bool CheckReceiveMessage();

	// ���̊֐��ŋA���Ă������b�Z�[�W��WM_QUIT��������I��
	// CheckReceiveMessage()��true���A���Ă����ꍇ�Ɏg�p
	MSG GetReceiveMessage();

	// CheckReceiveMessage()��false���������Ɏg�p
	// ���̊֐��Ńt���[���̒������s��
	void AdjustmentFrameLate();

	// �㏈�����s��
	// ��̓I�ɂ͎g�p�����f�o�C�X�R���e�L�X�g��������Ă���
	void Terminate();
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////�L�q��/////////////////////////////////////////
/*
#include <windows.h>
#include <DxLib.h>
#include "FrameFixity.h"

using namespace DxLib;

int WINAPI WinMain(HISNTACE hInstance, HISNTACE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	FrameFixity& ff = FrameFixity::Instance();
	// ����������
	// �`���̏����������`////
	ff.FFInitialize();
	/////////////////////////

	while (ProcessMessage() == 0) {
		if (ff.CheckReceiveMessage()) {
			if (ff.GetReceiveMessage() == WM_QUIT) {
				break;
			}
		}
		else {
			///////// �`���C�������`/////////
			ff.AdjustmentFrameLate();
			/////////////////////////////////
		}
	}

	// �㏈��
	////////// �`���̌㏈���`////////////
	ff.Terminate();
	////////////////////////////////////
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
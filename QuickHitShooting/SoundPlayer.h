#pragma once
#include <map>
#include <string>


class SoundPlayer
{
private:
	///�T�E���h�̃e�[�u���}�b�v
	///@param string �T�E���h��
	///@param int �T�E���h�n���h��
	std::map<std::string, int> _table;

public:
	SoundPlayer();
	~SoundPlayer();

	///�g�p����T�E���h�̒ǉ�
	///@param soundname �T�E���h��
	///@param handle	�T�E���h�n���h��
	///@param volume	�{�����[��(�ő�100%)
	///@retval true ����
	///@retval false ���s
	bool AddSound(const std::string& soundname, const int& handle, const int& volume = 100);

	///�T�E���h���Đ�������
	///@param soundname �T�E���h��
	///@param loop		���[�v�t���O(true�Ń��[�v�Đ�)
	void PlaySound(const std::string& soundname, const bool& loop = false);

	///�T�E���h���~������
	///@param soundname �T�E���h��
	void StopSound(const std::string& soundname);
};


#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Gun.h"

#include "../Loader/StageLoader.h"

class TrimString;
class Menu;

class SelectScene : public Scene
{
private:
	void (SelectScene::*_updater)(const Peripheral& p);

	void FadeinUpdate(const Peripheral& p);	
	void FadeoutUpdate(const Peripheral& p);
	void WaitUpdate(const Peripheral& p);

	std::unique_ptr<TrimString> _trimString;
	std::shared_ptr<Menu> _menu;

	GunStatus _gunState;
	std::vector<StageData> _stageDatas;

	int _stageCnt, _stageCntMax;

	std::vector<GunStatus> _gunStatus;

	/// �{�[�h�p�̉摜 
	int _rankBd, _gunBd, _stageBd;	
	int _selectBg;

	const int _dightMax;			// �X�R�A�̌����ő�l(�Ƃ肠����6��)

	///�e�̃��j���[�̒ǉ�
	///@param GunState	�e�̃X�e�[�^�X
	///@param ltPos		�摜��\�����鍶����W
	///@param rbPos		�摜��\������E�����W
	///@param img		�摜�̃n���h��
	void AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img);
	
	// �X�e�[�W�p�̃��j���[�{�b�N�X�̕\��
	void StageInit();

	StageData GetStageData(const int& num);

	// �X�e�[�W�p�X�擾�p�֐�
	std::string GetStagePath(const int& num) const;
public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};


#pragma once
#include <memory>
#include <array>
#include <vector>
#include "Scene.h"
#include "../Gun.h"

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

	std::vector<GunStatus> _gunStatus;
	
	///�e�̃��j���[�̒ǉ�
	///@param GunState	�e�̃X�e�[�^�X
	///@param ltPos		�摜��\�����鍶����W
	///@param rbPos		�摜��\������E�����W
	///@param img		�摜�̃n���h��
	void AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img);

public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};


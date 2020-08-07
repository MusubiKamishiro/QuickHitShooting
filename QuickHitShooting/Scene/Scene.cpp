#include "Scene.h"
#include "../Game.h"

#include "../Loader/EffekseerLoader.h"
#include "../Loader/FileSystem.h"

#include "EffekseerForDXLib.h"

Scene::Scene()
{
	_pal	   = 255;
	_scrSize   = Game::Instance().GetScreenSize();
	_sceneTime = 0;

	/// �G�t�F�N�g���̎擾
	EffekseerData data;
	Game::Instance().GetFileSystem()->Load("Effect/shot.efk", data);
	_shotEffect = data.GetHandle();
	_playEffect = -1;
}

Scene::~Scene()
{
}

void Scene::DrawEffect()
{
	SetPosPlayingEffekseer2DEffect(_playEffect, _efkPos.x, _efkPos.y, 0);

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B	��
	UpdateEffekseer2D();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B	��
	DrawEffekseer2D();
}


void Scene::AddSceneTime()
{
	++_sceneTime;
}

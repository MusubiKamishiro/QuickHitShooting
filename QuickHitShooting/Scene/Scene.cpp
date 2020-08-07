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

	/// エフェクト情報の取得
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

	// Effekseerにより再生中のエフェクトを更新する。	◆
	UpdateEffekseer2D();

	// Effekseerにより再生中のエフェクトを描画する。	◆
	DrawEffekseer2D();
}


void Scene::AddSceneTime()
{
	++_sceneTime;
}

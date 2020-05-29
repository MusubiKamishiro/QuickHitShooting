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

	/// ボード用の画像 
	int _rankBd, _gunBd, _stageBd;	
	int _selectBg;

	const int _dightMax;			// スコアの桁数最大値(とりあえず6桁)

	///銃のメニューの追加
	///@param GunState	銃のステータス
	///@param ltPos		画像を表示する左上座標
	///@param rbPos		画像を表示する右下座標
	///@param img		画像のハンドル
	void AddGunMenu(const GunStatus& gunstate, const Vector2<int>& ltPos, const Vector2<int>& rbPos, const int& img);
	
	// ステージ用のメニューボックスの表示
	void StageInit();

	StageData GetStageData(const int& num);

	// ステージパス取得用関数
	std::string GetStagePath(const int& num) const;
public:
	SelectScene();
	~SelectScene();

	void Update(const Peripheral& p);
	void Draw();
};


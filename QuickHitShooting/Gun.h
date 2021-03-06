#pragma once
#include <string>
#include <memory>

class TrimString;

// 銃の種類
enum class GunType
{

};

// 銃の方式(撃ち方)
enum class ShootType
{

};

// 銃を構成する必要変数
struct GunStatus
{
	std::string name;			// 銃名
	GunType gunType;			// 銃の種類
	ShootType shootType;		// 銃の方式(撃ち方)
	int maxBullets;				// 最大の総弾数
	int remainingBullets;		// 残りの総弾数
	int maxBulletsInMagazine;	// マガジン内の最大弾数
	int BulletsInMagazine;		// 残りのマガジン内の弾数
};

class Gun
{
private:
	GunStatus _gun;
	std::unique_ptr<TrimString> _trimString;

public:
	Gun(const GunStatus& gunState);
	~Gun();

	///マガジンに弾を込める
	///マガジン内に弾が残っていても可能
	void Reload();

	///弾が発射されたらtrueを返す
	///@retval true 撃った
	///@retval false マガジン内に弾がない
	bool Shot();

	///残弾数等の描画
	void Draw();
};


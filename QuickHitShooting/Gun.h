#pragma once
#include <string>

// e‚Ìí—Ş
enum class GunType
{

};

// e‚Ì•û®(Œ‚‚¿•û)
enum class ShootType
{

};

// e‚ğ\¬‚·‚é•K—v•Ï”
struct GunStatus
{
	std::string name;			// e–¼
	GunType gunType;			// e‚Ìí—Ş
	ShootType shootType;		// e‚Ì•û®(Œ‚‚¿•û)
	int maxBullets;				// Å‘å‚Ì‘’e”
	int remainingBullets;		// c‚è‚Ì‘’e”
	int maxBulletsInMagazine;	// ƒ}ƒKƒWƒ““à‚ÌÅ‘å’e”
	int BulletsInMagazine;		// c‚è‚Ìƒ}ƒKƒWƒ““à‚Ì’e”
};

class Gun
{
private:
	GunStatus _gun;

public:
	Gun(const GunStatus& gunState);
	~Gun();

	///ƒ}ƒKƒWƒ“‚É’e‚ğ‚ß‚é
	///ƒ}ƒKƒWƒ““à‚É’e‚ªc‚Á‚Ä‚¢‚Ä‚à‰Â”\
	void Reload();

	///’e‚ª”­Ë‚³‚ê‚½‚çtrue‚ğ•Ô‚·
	///@retval true Œ‚‚Á‚½
	///@retval false ƒ}ƒKƒWƒ““à‚É’e‚ª‚È‚¢
	bool Shot();

	///c’e”“™‚Ì•`‰æ
	void Draw();
};


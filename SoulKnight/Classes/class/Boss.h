#ifndef _BOSS_
#define  _BOSS_

#include "Monster.h"
#include <vector>
#include <initializer_list>
#include <memory>

class Boss :public Monster {
public:
	static Boss *create(
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f);

	static Boss *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f);

	static Boss *createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		Weapon *weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f);

	virtual void setWeaponChance(float weaponChance) { _weaponChance->setValue(weaponChance); }
	virtual void setWeaponReservoir(std::initializer_list <Weapon*> ilist) {
		_weaponResrvoir = std::vector<Weapon*>(ilist);
	}
	virtual void addWeaponToReservoir(Weapon *weapon) { _weaponResrvoir.push_back(weapon); }

	virtual float getWeaponChance()const { return _weaponChance->getValue(); }

	virtual Attribute<float> *getWeaponChanceInstance()const { return _weaponChance; }

	/*void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;*/

protected:
	virtual ~Boss() = default;

	bool init(
		int HPMax, int MPMax, float speed,
		Weapon *weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed,
		Weapon *weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed,
		Weapon *weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance);

	Attribute<float> *_weaponChance;
	std::vector<Weapon*> _weaponResrvoir;

private:
	bool initMember(float weaponChance);
};

#endif // _BOSS_

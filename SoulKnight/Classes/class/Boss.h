#ifndef _BOSS_
#define  _BOSS_

#include "Monster.h"
#include <vector>
#include <initializer_list>

class Boss :public Monster {
public:
	static std::shared_ptr<Boss> create(
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f, float weapomChance = 0.0f);

	static std::shared_ptr<Boss> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f, float weapomChance = 0.0f);

	static std::shared_ptr<Boss> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HP = 0, int HPMax = 0, int shield = 0, int shieldMax = 0, int MP = 0, int MPMax = 0,
		float speed = 0.0f, std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f, float magicPotChance = 0.0f, float weapomChance = 0.0f);

	virtual void setWeaponChance(float weaponChance) { _weaponChance->setValue(weaponChance); }
	virtual void setWeaponReservoir(std::initializer_list <std::shared_ptr<Weapon>> ilist) {
		_weaponResrvoir = std::vector<std::shared_ptr<Weapon>>(ilist);
	}
	virtual void addWeaponToReservoir(std::shared_ptr<Weapon> weapon) { _weaponResrvoir.push_back(weapon); }

	virtual float getWeaponChance()const { return _weaponChance->getValue(); }

	void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;

protected:
	virtual ~Boss() = default;

	bool init(
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
		float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance, float weapomChance);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax,
		float speed, std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance, float weapomChance);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HP, int HPMax, int shield, int shieldMax, int MP, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance, float magicPotChance, float weapomChance);

	std::shared_ptr<Attribute<float>> _weaponChance;
	std::vector<std::shared_ptr<Weapon>> _weaponResrvoir;

private:
	bool initMember(float weaponChance);
};

#endif // _BOSS_

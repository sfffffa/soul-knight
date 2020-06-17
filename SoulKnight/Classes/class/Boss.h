#ifndef _BOSS_
#define  _BOSS_

#include "Monster.h"
#include <vector>
#include <initializer_list>
#include <memory>

class Boss :public Monster {
public:
	static std::shared_ptr<Boss> create(
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f,
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Boss> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f,
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Boss> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int MPMax = 0, float speed = 0.0f,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		float coinChance = 0.0f, float healthPotChance = 0.0f,
		float magicPotChance = 0.0f, float weapomChance = 0.0f,
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	virtual void setCD(float cd) { _coolDown = cd; }
	virtual void setSkill(std::function<void(void)> skill) { _skill = skill; }
	virtual void setWeaponChance(float weaponChance) { _weaponChance = weaponChance; }
	virtual void setWeaponReservoir(std::initializer_list <std::shared_ptr<Weapon>> ilist) {
		_weaponResrvoir = std::vector<std::shared_ptr<Weapon>>(ilist);
	}
	virtual void addWeaponToReservoir(std::shared_ptr<Weapon> weapon) { _weaponResrvoir.push_back(weapon); }

	virtual float getWeaponChance()const { return _weaponChance; }
	virtual float getCD()const { return _coolDown; }

	/*void move(Vec2 dir)override;

	void shoot()override;

	void beShot(int damage)override;

	void die()override;*/

protected:
	virtual ~Boss() = default;

	bool init(
		int HPMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon,
		float coinChance, float healthPotChance,
		float magicPotChance, float weapomChance,
		std::function<void(void)> skill, float cd);

	float _weaponChance;
	std::vector<std::shared_ptr<Weapon>> _weaponResrvoir;
	float _coolDown;//技能cd
	std::function<void(void)> _skill;//技能

private:
	bool initMember(float weaponChance,
		float coolDown, std::function<void(void)> skill);
};

#endif // _BOSS_

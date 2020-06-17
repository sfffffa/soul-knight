#ifndef _HERO_
#define _HERO_

#include "Character.h"
#include <functional>
#include <new>

class Hero :public Character {
public:
	static std::shared_ptr<Hero> create(
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Hero> createWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	static std::shared_ptr<Hero> createWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax = 0, int shieldMax = 0, int MPMax = 0, float speed = 0,
		std::shared_ptr<Weapon> weapon = CloseInWeapon::create(),
		std::shared_ptr<Weapon> offhandWeapon = CloseInWeapon::create(),
		std::function<void(void)> skill = [] {}, float cd = 0.0f);

	//you should set shieldMAX before setting shield
	virtual void setShield(int shield) { _shield = (shield > _shieldMAX) ? _shieldMAX : shield; }
	virtual void setShieldMax(int shieldMax) { _shield = shieldMax; }
	virtual void setOffhandWeapon(std::shared_ptr<Weapon> offhandWeapon) { _offhandWeapon = offhandWeapon; }
	virtual void setCD(float cd) { _coolDown = cd; }
	virtual void setSkill(std::function<void(void)> skill) { _skill = skill; }

	virtual int getShield()const { return _shield; }
	virtual int getShieldMax()const { return _shieldMAX; }
	virtual float getCD()const { return _coolDown; }
	virtual bool getWeaponStatus()const { return _weaponStatus; }
	virtual std::shared_ptr<Weapon> getOffhandWeaponInstance()const { return _offhandWeapon; }

	virtual Hero *clone()const override;

	virtual bool shoot() override;

	virtual bool beShot(int damage) override;

	virtual bool changeWeapon();
	//use move(Vec2(0,0)) to stop
	/*virtual void move(Vec2 dir) override;

	virtual void shoot() override;

	virtual void beShot(int damage) override;

	virtual void die() override;*/

	//destructor
	virtual ~Hero() = default;
protected:

	bool init(
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	bool initWithSpriteFrameName(const std::string& spriteFrameName,
		int HPMax, int shieldMax, int MPMax, float speed,
		std::shared_ptr<Weapon> weapon, std::shared_ptr<Weapon> offhandWeapon,
		std::function<void(void)> skill, float cd);

	int _shield;//盾
	int _shieldMAX;
	std::shared_ptr<Weapon> _offhandWeapon;//副武器
	bool _weaponStatus = 0;//显示正在使用主武器还是副武器
	bool resurrection = 1;//可复活
	float _coolDown;//技能cd
	std::function<void(void)> _skill;//技能

private:
	bool initMember(int shieldMax, std::shared_ptr<Weapon> offhandWeapon,
		float coolDown, std::function<void(void)> skill);
};

#endif // _HERO_
